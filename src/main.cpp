//========== HEADER DAN DEFINISI ==========
#include <Arduino.h>
#include <lvgl.h>
#include "gui_guider.h"
#include "widgets_init.h"
#include "events_init.h"
#include "Arduino_GFX_Library.h"
#include "CustomRTOS/MutexManager.h"
#include "MutexManager.h"
#include "config.h"
#include "touch.h"

//========== DEFINISI PIN DAN KONFIGURASI ==========
#define TFT_MOSI 35
#define TFT_SCLK 36
#define TFT_CS   40
#define TFT_DC   41
#define TFT_RST  42
#define TFT_BL   38
#define MOSFET_PIN_1  4
#define MOSFET_PIN_2  5 
#define MOSFET_PIN_3  6
#define MOSFET_PIN_4  7 

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 170

//========== INISIALISASI OBJEK DAN VARIABEL ==========
Arduino_DataBus *bus = new Arduino_ESP32SPI(11 /* DC */, 10 /* CS */, 12 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 1 /* RST */, 1 /* rotation */, true /* IPS */, SCREEN_HEIGHT, SCREEN_WIDTH, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

static lv_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf1;
static lv_display_t *disp_drv;

MutexManager mutex_manager;
lv_ui guider_ui;

volatile uint16_t touch_x = 0, touch_y = 0;
volatile int touch_state = 0;

bool send_hex_flag = false;
bool isCharging = false;

TaskHandle_t uartTaskHandle = NULL;  
SemaphoreHandle_t labelMutex;

//========== CALLBACK UNTUK DISPLAY DAN TOUCH ==========
void lvgl_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p) {
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)color_p, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    lv_display_flush_ready(disp);
}

void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
    if (touch_state == 1) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touch_x;
        data->point.y = touch_y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

//========== TASK UNTUK TOUCH PANEL ==========
void touchTask(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(mutex_manager.myMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            int ret = CHSC6540_Scan((uint16_t *)&touch_x, (uint16_t *)&touch_y);
            touch_state = (ret == 1) ? 1 : 0;
            xSemaphoreGive(mutex_manager.myMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

//========== INISIALISASI DISPLAY DAN TOUCH ==========
void init_display() {
    gfx->begin();
    gfx->fillScreen(BLACK);
    gfx->invertDisplay(true);
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    lv_init();

    disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * SCREEN_WIDTH * 100, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    lv_draw_buf_init(&draw_buf, SCREEN_WIDTH, SCREEN_HEIGHT, LV_COLOR_FORMAT_RGB565, SCREEN_WIDTH, disp_draw_buf1, SCREEN_WIDTH * 100);

    disp_drv = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_flush_cb(disp_drv, lvgl_disp_flush);
    lv_display_set_buffers(disp_drv, disp_draw_buf1, NULL, SCREEN_WIDTH * 100, LV_DISPLAY_RENDER_MODE_PARTIAL);
}

void init_touch() {
    CHSC6540_init();
    lv_indev_t *indev_touch = lv_indev_create();
    lv_indev_set_type(indev_touch, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touch, my_touchpad_read);
}

//========== TASK UNTUK KONTROL MOSFET ==========
void mosfetAlternatingTask(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(labelMutex, portMAX_DELAY)) {
            digitalWrite(MOSFET_PIN_1, HIGH);
            digitalWrite(MOSFET_PIN_2, LOW);
            digitalWrite(MOSFET_PIN_3, HIGH);
            digitalWrite(MOSFET_PIN_4, LOW);
            isCharging = true;
            Serial.println("MOSFET 1 ON, MOSFET 2 OFF, MOSFET 3 ON, MOSFET 4 OFF");
            xSemaphoreGive(labelMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(5000));

        if (xSemaphoreTake(labelMutex, portMAX_DELAY)) {
            digitalWrite(MOSFET_PIN_1, LOW);
            digitalWrite(MOSFET_PIN_2, HIGH);
            digitalWrite(MOSFET_PIN_3, LOW);
            digitalWrite(MOSFET_PIN_4, HIGH);
            isCharging = false;
            Serial.println("MOSFET 1 OFF, MOSFET 2 ON, MOSFET 3 OFF, MOSFET 4 ON");
            xSemaphoreGive(labelMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//========== CALLBACK UNTUK EVENT TOMBOL ==========
void btn3_event_handler(lv_event_t *e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        if (!send_hex_flag) {
            uint8_t data = 0xFA;
            for (int i = 0; i < 3; i++) {
                Serial2.write(data);
                vTaskDelay(pdMS_TO_TICKS(300));  // Tetap gunakan delay di sini
            }
            Serial.println("=== DATA DIKIRIM ===");
            send_hex_flag = true;
        }
    } else {
        send_hex_flag = false;
    }
}

//========== TASK UNTUK MONITOR SCREEN ==========
void screenMonitorTask(void *pvParameters) {
    lv_obj_t *lastScreen = NULL;

    while (1) {
        lv_obj_t *currentScreen = lv_scr_act();  // Ambil screen aktif saat ini

        // Cek apakah screen yang aktif berbeda dengan sebelumnya
        if (currentScreen != lastScreen) {
            lastScreen = currentScreen;  // Update lastScreen

            // Cek layar aktif dan tampilkan status di Serial Monitor
            if (currentScreen == guider_ui.screen_1) {
                Serial.println("SCREEN 1");
                if (uartTaskHandle != NULL) {
                    vTaskSuspend(uartTaskHandle);  // Suspend UART task
                }
            } else if (currentScreen == guider_ui.screen_2) {
                Serial.println("SCREEN 2");
                if (uartTaskHandle != NULL) {
                    vTaskSuspend(uartTaskHandle);  // Suspend UART task
                }
            } else if (currentScreen == guider_ui.screen_3) {
                Serial.println("SCREEN 3");

                // Pastikan tombol memiliki event handler yang benar
                lv_obj_t *btn3 = guider_ui.screen_3_btn_3;  // Ambil objek tombol
                if (btn3 != NULL) {
                    // Tambahkan event handler jika belum ada
                    lv_obj_add_event_cb(btn3, btn3_event_handler, LV_EVENT_ALL, NULL);
                }

                if (uartTaskHandle != NULL) {
                    vTaskResume(uartTaskHandle);  // Resume UART task
                }
            } else {
                Serial.println("Aktif: Screen tidak diketahui");
                if (uartTaskHandle != NULL) {
                    vTaskSuspend(uartTaskHandle);  // Suspend UART task
                }
            }
        }

        // Update label berdasarkan layar aktif
        if (xSemaphoreTake(labelMutex, portMAX_DELAY)) {
            if (currentScreen == guider_ui.screen_1) {
                if (lv_obj_is_valid(guider_ui.screen_1_label_6)) {
                    lv_label_set_text(guider_ui.screen_1_label_6, isCharging ? "CHARGE" : "DISCHARGE");
                }
            } else if (currentScreen == guider_ui.screen_2) {
                if (lv_obj_is_valid(guider_ui.screen_2_label_6)) {
                    lv_label_set_text(guider_ui.screen_2_label_6, isCharging ? "DISCHARGE" : "CHARGE");
                }
            }
            xSemaphoreGive(labelMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(500)); // Delay 500 ms (0.5 detik)
    }
}

//========== FUNGSI UNTUK UPDATE LABEL ==========
void update_labels_async(void *param) {
    int *values = (int *)param;
    char buf[16];

    // Update label tilting (Screen 3)
    sprintf(buf, "%d", values[0]);
    lv_label_set_text(guider_ui.screen_3_label_3, buf);
    
    // Update label trendent (Screen 3)
    sprintf(buf, "%d", values[1]);
    lv_label_set_text(guider_ui.screen_3_label_5, buf);

    // Update label infrared (Screen 3)
    if (values[2] == 1) {
        lv_label_set_text(guider_ui.screen_3_label_7, "Error");
    } else {
        lv_label_set_text(guider_ui.screen_3_label_7, "Normal");
    }

    free(values);
}

//========== INISIALISASI UI ==========
void init_ui() {
    setup_ui(&guider_ui);          
    events_init(&guider_ui);
    init_scr_del_flag(&guider_ui);
}

//========== TASK UTAMA LVGL ==========
void lvglTask(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    while (1) {
        lv_tick_inc(10);
        lv_timer_handler();
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10));
    }
}

//========== KODE PENERIMA DATA UART ==========

// Buffer untuk data UART
uint8_t uartByteBuffer[4];
uint8_t uartByteCount = 0;

// Variabel untuk menyimpan nilai decoded
int trendent;
int tilting;
int current; 
int idleStatus = 0;
int statusInclino = 0;
int statusInfrared = 0;
int outputA =  0;
int outputB =  0;
int motorC =  0;

// Fungsi decode data  
void decodeData(uint32_t encodedData) {
    trendent = ((encodedData >> 24) & 0xFF) - 40;
    tilting  = ((encodedData >> 16) & 0xFF) - 40;
    statusInfrared = (encodedData >> 14) & 0x1;
    current = (encodedData & 0x1F) + 1;
    if (current > 22) {
        current = 0;
    }
    statusInclino = (encodedData >> 15) & 0x1;
    idleStatus = (encodedData >> 13) & 0x1;
    outputA = (encodedData >> 11) & 0x3;
    outputB = (encodedData >> 9) & 0x3;
    motorC = (encodedData >> 8) & 0x1;

    Serial.printf("Decoded -> Trendent: %d, Tilting: %d, Current: %d, Infrared: %s, Inclino: %s\n",
                  trendent, tilting, current,
                  statusInfrared == 1 ? "Error" : "Normal",
                  statusInclino == 1 ? "Error" : "Normal");

    Serial.printf("IdleStatus: %s, OutputA: %d, OutputB: %d, MotorC: %s\n",
                  idleStatus == 1 ? "ON" : "OFF",
                  outputA, outputB,
                  motorC == 1 ? "On" : "Off");
}

void uartTask(void *pvParameters) {
    // Inisialisasi Serial2 untuk komunikasi UART
    Serial2.begin(9600, SERIAL_8N1, 44, 43); 
    Serial.println("UART Task started, menunggu data...");
    
    for (;;) {
        // Cek ketersediaan data
        while (Serial2.available()) {
            uartByteBuffer[uartByteCount++] = Serial2.read();
            // Jika buffer sudah penuh, proses data
            if (uartByteCount == 4) {
                uint32_t encodedData = ((uint32_t)uartByteBuffer[0] << 24) |
                                         ((uint32_t)uartByteBuffer[3] << 16) |
                                         ((uint32_t)uartByteBuffer[2] << 8)  |
                                         ((uint32_t)uartByteBuffer[1]); 
                
                Serial.printf("Encoded Data: 0x%08X\n", encodedData);
                decodeData(encodedData);
                
                // Alokasikan array untuk 5 integer (tilting, trendent, infrared, MOSFET Screen 1, MOSFET Screen 2)
                int *values = (int *)malloc(5 * sizeof(int));
                if (values) {
                    values[0] = tilting;
                    values[1] = trendent;
                    values[2] = statusInfrared;
                    // values[3] = digitalRead(MOSFET_PIN_1); // Status MOSFET Screen 1
                    // values[4] = digitalRead(MOSFET_PIN_2); // Status MOSFET Screen 2
                    // values[5] = digitalRead(MOSFET_PIN_3); // Status MOSFET Screen 2
                    // values[6] = digitalRead(MOSFET_PIN_4); // Status MOSFET Screen 2

                    lv_async_call(update_labels_async, (void *)values);
                } else {
                    Serial.println("Error alokasi memori untuk update UI!");
                }
                
                uartByteCount = 0;  // Reset buffer
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

//========== SETUP DAN LOOP ==========
void setup() {
    Wire.setPins(TOUCH_SDA, TOUCH_SCL);
    Wire.begin();
    Serial.begin(9600);
    mutex_manager.initMutex();
    
    pinMode(MOSFET_PIN_1, OUTPUT);  
    digitalWrite(MOSFET_PIN_1, LOW);  

    pinMode(MOSFET_PIN_2, OUTPUT);  
    digitalWrite(MOSFET_PIN_2, LOW);
    
    pinMode(MOSFET_PIN_3, OUTPUT);
    digitalWrite(MOSFET_PIN_3, LOW);

    pinMode(MOSFET_PIN_4, OUTPUT);
    digitalWrite(MOSFET_PIN_4, LOW);
    
    init_display();
    init_touch();
    init_ui();
    
    labelMutex = xSemaphoreCreateMutex();

    // Buat task LVGL, touch, UART, dan screen monitor pada core yang sesuai
    xTaskCreatePinnedToCore(lvglTask, "LVGL Task", 8192, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(touchTask, "Touch Task", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(uartTask, "UART Task", 4096, NULL, 1, &uartTaskHandle, 1);  
    xTaskCreatePinnedToCore(screenMonitorTask, "Screen Monitor Task", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(mosfetAlternatingTask, "MOSFET Alternating Task", 2048, NULL, 1, NULL, 1);

    Serial.println("Setup complete, running.");
}

void loop() {
    //kosongkan loop() 
}