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

#define TFT_MOSI 35
#define TFT_SCLK 36
#define TFT_CS   40
#define TFT_DC   41
#define TFT_RST  42
#define TFT_BL   38

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 170

// Inisialisasi objek display fisik
Arduino_DataBus *bus = new Arduino_ESP32SPI(11 /* DC */, 10 /* CS */, 12 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 1 /* RST */, 1 /* rotation */, true /* IPS */, SCREEN_HEIGHT, SCREEN_WIDTH, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

static lv_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf1;
static lv_display_t *disp_drv;

MutexManager mutex_manager;
lv_ui guider_ui; // Struktur UI didefinisikan pada file gui_guider.h

volatile uint16_t touch_x = 0, touch_y = 0;
volatile int touch_state = 0;

// --- Callback untuk flush display pada LVGL ---
void lvgl_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p) {
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)color_p, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    lv_display_flush_ready(disp);
}

// --- Callback untuk membaca input sentuhan ---
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
    if (touch_state == 1) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touch_x;
        data->point.y = touch_y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// --- Task untuk memindai touch panel ---
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

// --- Inisialisasi Display dan LVGL ---
void init_display() {
    gfx->begin();
    gfx->fillScreen(BLACK);
    gfx->invertDisplay(true);
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    lv_init();

    disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * SCREEN_WIDTH * 100, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    lv_draw_buf_init(&draw_buf, SCREEN_WIDTH, SCREEN_HEIGHT, LV_COLOR_FORMAT_RGB565, SCREEN_WIDTH, disp_draw_buf1, SCREEN_WIDTH * 20);

    disp_drv = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_flush_cb(disp_drv, lvgl_disp_flush);
    lv_display_set_buffers(disp_drv, disp_draw_buf1, NULL, SCREEN_WIDTH * 100, LV_DISPLAY_RENDER_MODE_PARTIAL);
}

// --- Inisialisasi Touchscreen ---
void init_touch() {
    CHSC6540_init();
    lv_indev_t *indev_touch = lv_indev_create();
    lv_indev_set_type(indev_touch, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touch, my_touchpad_read);
    Serial.println("Touchscreen initialized.");
}

// --- Inisialisasi UI ---
void init_ui() {
    setup_ui(&guider_ui);          
    events_init(&guider_ui);
    init_scr_del_flag(&guider_ui);
    setup_scr_screen_3(&guider_ui);

    Serial.println("UI initialized.");
}

// --- Task LVGL utama ---
void lvglTask(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    while (1) {
        lv_tick_inc(5);
        lv_timer_handler();
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(5));
    }
}

// --- Kode UART (penerimaan data) ---
uint8_t uartByteBuffer[4];
uint8_t uartByteCount = 0;
int trendent;
int tilting;
int statusInfrared = 0;

void decodeData(uint32_t encodedData) {
    trendent = ((encodedData >> 24) & 0xFF) - 40;
    tilting  = ((encodedData >> 16) & 0xFF) - 40;
    statusInfrared = (encodedData >> 14) & 0x1;

    Serial.printf("Decoded -> Trendent: %d, Tilting: %d, Infrared: ", trendent, tilting);
    Serial.println((statusInfrared == 1) ? "Error" : "Normal");
}

void update_labels_async(void * param) {
    int *values = (int *) param;
    char buf[16];

    sprintf(buf, "%d", values[0]);
    lv_label_set_text(guider_ui.screen_3_label_3, buf);

    sprintf(buf, "%d", values[1]);
    lv_label_set_text(guider_ui.screen_3_label_5, buf);

    lv_label_set_text(guider_ui.screen_3_label_7, values[2] == 1 ? "Error" : "Normal");

    free(values);
}

void uartTask(void *pvParameters) {
    Serial2.begin(9600, SERIAL_8N1, 44, 43); 
    Serial.println("UART Task started, menunggu data...");
    
    for (;;) {
        while (Serial2.available()) {
            uartByteBuffer[uartByteCount++] = Serial2.read();
            if (uartByteCount == 4) {
                uint32_t encodedData = ((uint32_t)uartByteBuffer[3] << 24) |
                                         ((uint32_t)uartByteBuffer[2] << 16) |
                                         ((uint32_t)uartByteBuffer[1] << 8)  |
                                         ((uint32_t)uartByteBuffer[0]);
                
                Serial.printf("Encoded Data: 0x%08X\n", encodedData);
                decodeData(encodedData);
                
                int *values = (int *)malloc(3 * sizeof(int));
                if (values) {
                    values[0] = tilting;
                    values[1] = trendent;
                    values[2] = statusInfrared;
                    lv_async_call(update_labels_async, (void *)values);
                } else {
                    Serial.println("Error alokasi memori untuk update UI!");
                }
                
                uartByteCount = 0;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// // --- Callback untuk menangani event sentuhan ---
// static void screen_3_btn_3 (lv_event_t * e) {
//     if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
//         Serial.println("Screen 3 Checking Update ditekan, mengirim data...");
//         uint8_t data[] = {0xFA, 0xFA, 0xFA};
//         Serial2.write(data, sizeof(data)); // Mengirim data hex
//         vTaskDelay(pdMS_TO_TICKS(300)); // Cek setiap 1 detik
//         Serial2.write(data, sizeof(data)); // Mengirim data hex
//         vTaskDelay(pdMS_TO_TICKS(300)); // Cek setiap 1 detik
//         Serial2.write(data, sizeof(data)); // Mengirim data hex
//     }
// }

// extern "C" void send_update_request() {
//     uint8_t data = 0xFA;
//     Serial2.write(data);
//     Serial.println("Data 0xFA dikirim dari event handler.");
// }

extern "C" void send_update_request() {
    uint8_t data = 0xFA;

    Serial2.write(data);
    Serial.println("Data 0xFA dikirim dari event handler.");
    vTaskDelay(pdMS_TO_TICKS(300)); // Delay 300ms

    Serial2.write(data);
    Serial.println("Data 0xFA dikirim dari event handler.");
    vTaskDelay(pdMS_TO_TICKS(300)); // Delay 300ms

    Serial2.write(data);
    Serial.println("Data 0xFA dikirim dari event handler.");
}


// --- Fungsi setup() ---
void setup() {
    // Inisialisasi I2C untuk touch (sesuaikan pin jika diperlukan)
    Wire.setPins(TOUCH_SDA, TOUCH_SCL);
    Wire.begin();

    Serial.begin(9600);
    mutex_manager.initMutex();
    
    init_display();
    init_touch();
    init_ui();
    
    // Buat task untuk LVGL, Touch, dan UART via FreeRTOS
    xTaskCreatePinnedToCore(lvglTask, "LVGL Task", 8192, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(touchTask, "Touch Task", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(uartTask, "UART Task", 4096, NULL, 1, NULL, 1);
    // xTaskCreatePinnedToCore(sendTask, "Send Task", 2048, NULL, 1, NULL, 1);
    
    Serial.println("Setup complete, running.");
}

// --- Fungsi loop() ---
void loop() {
    // Loop utama kosong karena FreeRTOS meng-handle semua task.
}