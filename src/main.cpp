// ========== Header dan Library ==========
#include <Arduino.h>
#include <lvgl.h>
#include "gui_guider.h"
#include "events_init.h"
#include "Arduino_GFX_Library.h"
#include "touch.h"
#include <MutexManager.h>
#include <semphr.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "fuzzy_kardiovaskular.h"

//========== PIN & UKURAN LAYAR ==========
#define TFT_BL 38

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 170

#define MEASURE_DURATION 60

//========== PIN I2C ==========
#define SDA 6
#define SCL 15

//========== INISIALISASI I2C TERLEBIH DAHULU ==========
TwoWire I2C_sensor = TwoWire(1);

//========== INISIALISASI DISPLAY & LVGL ==========
Arduino_DataBus *bus = new Arduino_ESP32SPI(11, 10, 12, 13, GFX_NOT_DEFINED);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 1, 1, true, SCREEN_HEIGHT, SCREEN_WIDTH, 35, 0, 35, 0);
// Arduino_GFX *gfx = new Arduino_GC9A01(bus, 1, 1, true, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 35, 0, 35);

static lv_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf1;
static lv_display_t *disp_drv;

SemaphoreHandle_t labelMutex;
SemaphoreHandle_t i2cMutex;  
SemaphoreHandle_t mpuMutex;

MutexManager mutex_manager;
lv_ui guider_ui; 

volatile bool mlxEnable = false;
volatile bool mpuCalibrated = false;
volatile bool mpuEnable = false;
volatile bool maxEnable = false;
volatile bool maxVitalsEnable = false;

// ========== TIMER MAX ==========
volatile bool maxFingerActive = false;
unsigned long maxTimerStart = 0;
bool maxTimerRunning = false;
bool maxTimerFinished = false;
bool fuzzyDone = false; 
char timerBuf[32];

MAX30105 particleSensor;

// ========== DETEKSI STABILITAS JARI ==========
volatile bool fingerStable = true;
volatile bool maxFingerGood = false;

float lastPitch = 0.0;
float lastRoll  = 0.0;

// ========== KONSTANTA BARU ==========
#define STABLE_THRESHOLD     2.5f   // Derajat — lebih toleran terhadap noise
#define UNSTABLE_COUNT_MAX   3      // Harus goyang 3x berturut-turut → tidak stabil
#define STABLE_COUNT_MAX     5      // Harus diam 5x berturut-turut → stabil kembali

// Shared data untuk label update
struct {
    char suhuBuf[32];
    bool needsUpdate;
} labelData = {"", false};

// Shared data untuk MPU6050
struct {
    char pitchBuf[32];
    char rollBuf[32];
    bool needsUpdate;
} mpuData = {"", "", false};

// Shared data untuk MAX30105
struct {
    char bpmBuf[16];
    char avgBuf[16];
    char spo2Buf[16];
    bool needsUpdate;
} maxData = {"", "", "", false};

// Tambahkan struct global untuk hasil fuzzy
struct {
    char statusBuf[16];
    bool needsUpdate;
} fuzzyData;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_MPU6050 mpu; // Alamat I2C default MPU6050

// ========== MPU Calibration Reference ==========
float roll_ref = 0.0f;
float pitch_ref = 0.0f;

//=== MPU Filter Buffer ===
#define FILTER_SIZE 10
float roll_buf[FILTER_SIZE] = {0};
float pitch_buf[FILTER_SIZE] = {0};
int filter_idx = 0;
bool filter_ready = false;

// === SpO2 Calibration ===
float spo2_gain   = 1.0f;
float spo2_offset = 3.0f;

// ========== KONTROL SUHU LCD ==========
volatile bool suhuLCDDone = false;

// ========== RESET PENGUKURAN MAX ==========
void resetMaxMeasurement()
{
    maxFingerActive   = false;
    maxTimerRunning   = false;
    maxTimerFinished  = false;
    maxFingerGood     = false;
    fuzzyDone         = false;

    // Reset pengambilan suhu untuk LCD
    suhuLCDDone = false;

    maxTimerStart = 0;
    strcpy(timerBuf, "00:01:00");

    if (xSemaphoreTake(labelMutex, pdMS_TO_TICKS(50)) == pdTRUE)
    {
        strcpy(maxData.bpmBuf,  "--");
        strcpy(maxData.avgBuf,  "--");
        strcpy(maxData.spo2Buf, "--");
        maxData.needsUpdate = true;

        strcpy(fuzzyData.statusBuf, "--");
        fuzzyData.needsUpdate = false;

        xSemaphoreGive(labelMutex);
    }

    fingerStable = true;

    Serial.println("[RESET] MAX measurement di-reset");
}
// ==========================================

//========== Fungsi LVGL Flush Display ==========
void lvgl_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p) {
    gfx->draw16bitRGBBitmap(
        area->x1, area->y1,
        (uint16_t *)color_p,
        area->x2 - area->x1 + 1,
        area->y2 - area->y1 + 1);

    lv_display_flush_ready(disp);
}

//========== Fungsi Touchpad Read ==========
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
    uint16_t touch_x;
    uint16_t touch_y;
    int st = CHSC6540_Scan(&touch_x, &touch_y);

    if (st == 1) {
        data->state = LV_INDEV_STATE_PRESSED;

        uint16_t mapped_x = SCREEN_WIDTH  - 1 - touch_x;
        uint16_t mapped_y = SCREEN_HEIGHT - 1 - touch_y;

        data->point.x = mapped_x;
        data->point.y = mapped_y;
    } 
    else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// ========== Inisialisasi Touch ==========
void init_touch() {
    CHSC6540_init();
    lv_indev_t *indev_touch = lv_indev_create();
    lv_indev_set_type(indev_touch, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touch, my_touchpad_read);
}

// ========= Inisialisasi Display dan LVGL ==========
void init_display() {
    gfx->begin();
    gfx->fillScreen(BLACK);
    gfx->invertDisplay(true);
    gfx->setRotation(3);
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    lv_init();

    disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * SCREEN_WIDTH * 100, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    lv_draw_buf_init(&draw_buf, SCREEN_WIDTH, SCREEN_HEIGHT, LV_COLOR_FORMAT_RGB565, SCREEN_WIDTH, disp_draw_buf1, SCREEN_WIDTH * 100);

    disp_drv = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_flush_cb(disp_drv, lvgl_disp_flush);
    lv_display_set_buffers(disp_drv, disp_draw_buf1, NULL, SCREEN_WIDTH * 100, LV_DISPLAY_RENDER_MODE_PARTIAL);
}

// ========== Kategori Suhu MLX ==========
String kategoriSuhu(float suhu) {
    if (suhu < 36.5) {
        return "HIPOTERMIA";
    } 
    else if (suhu >= 36.5 && suhu < 37.2) {
        return "NORMAL";
    } 
    else if (suhu >= 37.2 && suhu <= 38.0) {
        return "DEMAM RINGAN";
    } 
    else {
        return "DEMAM TINGGI";
    }
}

//========== MLX90614 Task ==========
void mlxTask(void *pvParameters)
{
    Serial.println("[MLX] Task started");

    const int MLX_AVG_SAMPLES = 5;
    const int MLX_SAMPLE_DELAY = 100;

    const float MLX_OFFSET = 2.1f;

    while (1)
    {
        // ==================================================
        // MLX tetap membaca selama screen 2 aktif
        // ==================================================
        if (!mlxEnable)
        {
            vTaskDelay(pdMS_TO_TICKS(300));
            continue;
        }

        float sampleSum = 0.0f;
        int validCount = 0;
        bool i2cFailed = false;
        bool bodyDetected = false;

        // ==================================================
        // AMBIL 5 SAMPEL
        // ==================================================
        for (int i = 0; i < MLX_AVG_SAMPLES; i++)
        {
            float raw = 0.0f;

            if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100)) == pdTRUE)
            {
                raw = mlx.readObjectTempC();
                xSemaphoreGive(i2cMutex);
            }
            else
            {
                Serial.println("[MLX] Gagal ambil i2cMutex");
                i2cFailed = true;
                break;
            }

        if (!isnan(raw))
        {
            // Serial Monitor tetap menampilkan semua pembacaan
            Serial.printf(
                "[MLX] Sampel %d = %.2f C\n",
                i,
                raw
            );

            sampleSum += raw;
            validCount++;

            // Deteksi tubuh manusia
            if (raw >= 30.0f && raw <= 42.5f)
            {
                bodyDetected = true;
            }
        }
        else
        {
            Serial.printf(
                "[MLX] Sampel %d = NaN\n",
                i
            );
        }

            vTaskDelay(pdMS_TO_TICKS(MLX_SAMPLE_DELAY));
        }

        if (i2cFailed)
        {
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        if (validCount == 0)
        {
            Serial.println("[MLX] Tidak ada data suhu valid");
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        // ==================================================
        // RATA-RATA 5 SAMPEL
        // ==================================================
        float suhuAvg =
            sampleSum / (float)validCount;

        // ==================================================
        // OFFSET KALIBRASI
        // ==================================================
        float suhuKalibrasi =
            suhuAvg + MLX_OFFSET;

        // ==================================================
        // SERIAL MONITOR SELALU LANJUT
        // ==================================================
        Serial.printf(
            "[MLX] Avg=%.2f | Kalibrasi=%.2f | LCD=%s\n",
            suhuAvg,
            suhuKalibrasi,
            suhuLCDDone ? "SUDAH DIAMBIL" : "MENUNGGU"
        );

        // ==================================================
        // LCD HANYA DIUPDATE 1 KALI
        // ==================================================
        if (!suhuLCDDone && bodyDetected)
        {
            char suhuBuf[32];

            snprintf(
                suhuBuf,
                sizeof(suhuBuf),
                "%.1f C",
                suhuKalibrasi
            );

            if (xSemaphoreTake(
                    labelMutex,
                    pdMS_TO_TICKS(50)) == pdTRUE)
            {
                strncpy(
                    labelData.suhuBuf,
                    suhuBuf,
                    sizeof(labelData.suhuBuf) - 1
                );

                labelData.suhuBuf[
                    sizeof(labelData.suhuBuf) - 1
                ] = '\0';

                labelData.needsUpdate = true;

                xSemaphoreGive(labelMutex);
            }

            // Kunci setelah suhu berhasil dikirim ke LCD
            suhuLCDDone = true;

            Serial.printf(
                "[MLX] SUHU LCD DIAMBIL 1X = %.1f C\n",
                suhuKalibrasi
            );
        }

        // ==================================================
        // TUNGGU SEBELUM PEMBACAAN BERIKUTNYA
        // Serial tetap berjalan
        // ==================================================
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


// =========== MPU6050 Task — VERSI FINAL ==========
void mpuTask(void *pvParameters) {

    int unstableCount = 0;
    int stableCount   = 0;

    while (1) {

        if (!mpuEnable) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        // ── Baca sensor ──
        if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        xSemaphoreGive(i2cMutex);

        float ax = a.acceleration.x;
        float ay = a.acceleration.y;
        float az = a.acceleration.z;

        // ── Formula pitch/roll ──
        float pitch_raw = atan2(-ax, sqrt(ay*ay + az*az)) * 180.0f / M_PI;
        float roll_raw  = atan2(ay, az) * 180.0f / M_PI;

        // ── Ambil mpuMutex untuk akses buffer & global ──
        if (xSemaphoreTake(mpuMutex, pdMS_TO_TICKS(50)) != pdTRUE) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        // ── Masukkan ke filter buffer ──
        roll_buf[filter_idx]  = roll_raw;
        pitch_buf[filter_idx] = pitch_raw;
        filter_idx = (filter_idx + 1) % FILTER_SIZE;
        if (filter_idx == 0) filter_ready = true;

        // ── Hitung rata-rata buffer ──
        int   count     = filter_ready ? FILTER_SIZE : (filter_idx == 0 ? 1 : filter_idx);
        float roll_avg  = 0.0f;
        float pitch_avg = 0.0f;
        for (int i = 0; i < count; i++) {
            roll_avg  += roll_buf[i];
            pitch_avg += pitch_buf[i];
        }
        roll_avg  /= count;
        pitch_avg /= count;

        // ── Kurangi referensi kalibrasi ──
        // Sebelum kalibrasi: roll_ref=0, pitch_ref=0 → tampil nilai asli
        // Setelah kalibrasi: tampil relatif terhadap posisi saat kalibrasi
        float roll_cal  = roll_avg  - roll_ref;
        float pitch_cal = pitch_avg - pitch_ref;

        // ── Simpan ke global (nilai sudah difilter + dikalibrasi) ──
        // ── Deteksi stabilitas dengan hysteresis ──
        float dPitch = fabsf(pitch_cal - lastPitch);
        float dRoll  = fabsf(roll_cal  - lastRoll);
        lastPitch = pitch_cal;
        lastRoll  = roll_cal;

        if (dPitch > STABLE_THRESHOLD || dRoll > STABLE_THRESHOLD) {
            stableCount = 0;
            if (++unstableCount >= UNSTABLE_COUNT_MAX) {
                fingerStable  = false;
                unstableCount = UNSTABLE_COUNT_MAX;
            }
        } else {
            unstableCount = 0;
            if (++stableCount >= STABLE_COUNT_MAX) {
                fingerStable = true;
                stableCount  = STABLE_COUNT_MAX;
            }
        }

        xSemaphoreGive(mpuMutex);

        // ── Update label UI ──
        if (xSemaphoreTake(labelMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
            // label_5 = Roll, label_6 = Pitch
            snprintf(mpuData.rollBuf,  sizeof(mpuData.rollBuf),  "%.1f", roll_cal);
            snprintf(mpuData.pitchBuf, sizeof(mpuData.pitchBuf), "%.1f", pitch_cal);
            mpuData.needsUpdate = true;
            xSemaphoreGive(labelMutex);
        }

        Serial.printf("[MPU] raw P=%.2f R=%.2f | cal P=%.2f R=%.2f | stable=%d\n",
                      pitch_raw, roll_raw, pitch_cal, roll_cal, (int)fingerStable);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// =========== MAX30105 Task ==========
void maxTask(void *pvParameters)
{
    // ==============================
    // KONSTANTA
    // ==============================
    const long     IR_THRESHOLD    = 5000;
    const int      SPO2_BUF_SIZE   = 200;   // ~1 detik @ 200Hz (lebih stabil)
    // const int      BPM_WIN_SIZE    = 8;     // Rata-rata 8 beat terakhir
    // const float    EXP_ALPHA       = 0.05f; // Smoothing kuat (makin kecil makin halus)
    const float EXP_ALPHA = 0.15f;  // dari 0.05f → konvergen lebih cepat dalam 30 detik
    const int BPM_WIN_SIZE = 5;  // window lebih pendek, lebih cepat mengikuti kondisi terkini

    // ==============================
    // SpO2 — buffer lebih besar
    // ==============================
    long irBuffer[SPO2_BUF_SIZE];
    long redBuffer[SPO2_BUF_SIZE];
    int  spo2Index  = 0;
    bool spo2Ready  = false;
    float spo2      = 0.0f;
    float spo2Smooth = 0.0f;    // Nilai SpO2 setelah dihaluskan
    bool spo2Init   = false;    // Apakah spo2Smooth sudah diinisialisasi

    // ==============================
    // BPM — window buffer 8 beat
    // ==============================
    float bpmWindow[BPM_WIN_SIZE] = {0};  // Simpan 8 interval beat terakhir
    int   bpmWinIdx  = 0;
    int   bpmWinFill = 0;        // Berapa beat sudah masuk window
    float bpm        = 0.0f;    // BPM realtime dari 1 beat
    float avgBpm     = 0.0f;    // BPM rata-rata dari window
    float bpmSmooth  = 0.0f;    // BPM setelah exponential smoothing
    bool  bpmInit    = false;   // Apakah bpmSmooth sudah diinisialisasi
    unsigned long lastBeat = 0;

    // ==============================
    // STATUS
    // ==============================
    bool avgReady = false;

    unsigned long fingerStartTime = 0;

    while (1)
    {
        if (!maxEnable)
        {
            vTaskDelay(pdMS_TO_TICKS(300));
            continue;
        }

        if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100)) == pdTRUE)
        {
            particleSensor.check();
            xSemaphoreGive(i2cMutex);
        }

        while (particleSensor.available())
        {
            long irValue = particleSensor.getIR();

            // ================= STATUS JARI =================
            // IR > 6000 = jari menempel dengan baik
            maxFingerGood = (irValue > 6000);

            if (!maxVitalsEnable)
            {
                particleSensor.nextSample();
                continue;
            }

            long redValue = particleSensor.getRed();

            // =========================
            // FINGER DETECTION
            // =========================
            if (irValue > IR_THRESHOLD)
            {
                // ================= TIMER START =================
                if (!maxFingerActive)
                {
                    maxFingerActive  = true;
                    maxTimerStart    = millis();
                    maxTimerRunning  = true;
                    maxTimerFinished = false;
                    fingerStartTime  = millis(); // ← FIX: catat saat jari mulai

                    // Reset semua akumulator saat jari baru ditempel
                    bpmWinFill = 0;
                    bpmWinIdx  = 0;
                    bpmInit    = false;
                    spo2Init   = false;
                    avgReady   = false;
                    lastBeat   = 0;
                    spo2Index  = 0;
                    spo2Ready  = false;

                    fuzzyDone = false;

                    if (xSemaphoreTake(labelMutex, pdMS_TO_TICKS(50)) == pdTRUE)
                    {
                        strcpy(fuzzyData.statusBuf, "--");
                        fuzzyData.needsUpdate = true;   // ← paksa label ke-update jadi "--"

                        // Opsional: bersihkan juga BPM/SpO2/AVG biar tampilan benar-benar "fresh"
                        strcpy(maxData.bpmBuf,  "--");
                        strcpy(maxData.avgBuf,  "--");
                        strcpy(maxData.spo2Buf, "--");
                        maxData.needsUpdate = true;

                        xSemaphoreGive(labelMutex);
                    }

                    Serial.println("[MAX] Jari terdeteksi, timer mulai");
                }

                // =========================
                // HITUNG BPM DARI BEAT
                // =========================
                if (checkForBeat(irValue))
                {
                    unsigned long now = millis();

                    if (lastBeat != 0)
                    {
                        float delta      = (now - lastBeat) / 1000.0f;
                        float currentBpm = 60.0f / delta;

                        // Tolak spike: hanya terima 40-180 bpm
                        // Ref: AHA - normal sinus rhythm 60-100, max reasonable 180
                        if (currentBpm >= 40.0f && currentBpm <= 180.0f)
                        {
                            // Simpan ke window buffer (rolling average 8 beat)
                            bpmWindow[bpmWinIdx] = currentBpm;
                            bpmWinIdx = (bpmWinIdx + 1) % BPM_WIN_SIZE;
                            if (bpmWinFill < BPM_WIN_SIZE) bpmWinFill++;

                            // Hitung rata-rata dari window
                            float sum = 0;
                            for (int i = 0; i < bpmWinFill; i++) sum += bpmWindow[i];
                            float windowAvg = sum / bpmWinFill;

                            // Exponential smoothing di atas window average
                            // Alpha kecil (0.05) = perubahan sangat halus
                            // Mirip perilaku oximeter medis
                            if (!bpmInit)
                            {
                                bpmSmooth = windowAvg; // Inisialisasi pertama kali
                                bpmInit   = true;
                            }
                            else
                            {
                                bpmSmooth = (1.0f - EXP_ALPHA) * bpmSmooth
                                          + EXP_ALPHA * windowAvg;
                            }

                            bpm    = currentBpm; // Nilai mentah 1 beat
                            avgBpm = bpmSmooth;  // Nilai halus untuk ditampilkan
                        }
                    }
                    lastBeat = now;
                }

                // =========================
                // BUFFER SpO2 (rolling)
                // =========================
                irBuffer[spo2Index]  = irValue;
                redBuffer[spo2Index] = redValue;
                spo2Index = (spo2Index + 1) % SPO2_BUF_SIZE;

                // Hitung SpO2 setiap buffer penuh 1 kali
                // (spo2Index kembali ke 0 = 1 putaran selesai)
                if (spo2Index == 0) spo2Ready = true;

                if (spo2Ready)
                {
                    spo2Ready = false;

                    // ── DC component (rata-rata sinyal) ──
                    long irDC = 0, redDC = 0;
                    for (int i = 0; i < SPO2_BUF_SIZE; i++)
                    {
                        irDC  += irBuffer[i];
                        redDC += redBuffer[i];
                    }
                    irDC  /= SPO2_BUF_SIZE;
                    redDC /= SPO2_BUF_SIZE;

                    // ── AC component (variasi dari rata-rata) ──
                    long irAC = 0, redAC = 0;
                    for (int i = 0; i < SPO2_BUF_SIZE; i++)
                    {
                        irAC  += abs(irBuffer[i]  - irDC);
                        redAC += abs(redBuffer[i] - redDC);
                    }
                    irAC  /= SPO2_BUF_SIZE;
                    redAC /= SPO2_BUF_SIZE;

                    if (irAC > 0 && irDC > 0 && redAC > 0 && redDC > 0)
                    {
                        // Formula R = (RedAC/RedDC) / (IrAC/IrDC)
                        // Semakin tinggi R → semakin rendah SpO2
                        float R = ((float)redAC / (float)redDC)
                                / ((float)irAC  / (float)irDC);

                        // Koefisien yang lebih sesuai MAX30102
                        // Ref: Maxim App Note AN6409; digunakan juga di
                        // Nurhadiansyah et al. (2022) IJAIR
                        // Formula: SpO2 = 104 - 17 * R  (lebih akurat dari 110-25R)
                        float rawSpo2 = 104.0f - 17.0f * R;
                        rawSpo2 = constrain(rawSpo2, 70.0f, 100.0f);

                        // Tolak hasil yang tidak masuk akal
                        // (di bawah 70% hampir mustahil pada orang sadar)
                        if (rawSpo2 >= 70.0f)
                        {
                            // Exponential smoothing SpO2
                            // Alpha 0.1 = perubahan lambat dan halus
                            if (!spo2Init)
                            {
                                spo2Smooth = rawSpo2;
                                spo2Init   = true;
                            }
                            else
                            {
                                spo2Smooth = 0.90f * spo2Smooth
                                           + 0.10f * rawSpo2;
                            }
                            // spo2 = spo2Smooth;

                            float spo2Calibrated = spo2Smooth * spo2_gain + spo2_offset;

                            // Batasi agar tetap berada pada rentang fisiologis
                            spo2Calibrated = constrain(spo2Calibrated, 70.0f, 100.0f);

                            spo2 = spo2Calibrated;
                        }
                    }
                }

                // =========================
                // AVG READY setelah 5 detik
                // dari saat jari ditempel
                // =========================
                if (!avgReady &&
                    millis() - fingerStartTime >= 6000UL &&
                    bpmWinFill >= 2)  // Minimal 3 beat sudah masuk
                {
                    avgReady = true;
                    Serial.printf("[MAX] Stabil. BPM=%.1f SpO2=%.1f\n",
                                  avgBpm, spo2);
                }

                // Di bagian KIRIM KE UI, tambahkan kondisi:
                if (xSemaphoreTake(labelMutex, pdMS_TO_TICKS(10)) == pdTRUE)
                {
                    snprintf(maxData.bpmBuf, sizeof(maxData.bpmBuf), "%.0f", bpm);
                    
                    if (avgReady) {
                        snprintf(maxData.avgBuf, sizeof(maxData.avgBuf), "%.0f", avgBpm);
                    } else {
                        strcpy(maxData.avgBuf, "--");  // ← belum cukup data, jangan tampilkan angka mentah
                    }
                    
                    snprintf(maxData.spo2Buf, sizeof(maxData.spo2Buf), "%.0f", spo2);
                    maxData.needsUpdate = true;
                    xSemaphoreGive(labelMutex);
                }

                Serial.printf("[MAX] BPM=%.1f | AvgBPM=%.1f | SpO2=%.1f%%\n",
                              bpm, avgBpm, spo2);
            }
            else
            {
                // ================= JARI DILEPAS =================
                if (maxFingerActive)
                {
                    maxFingerActive  = false;
                    maxTimerRunning  = false;
                    maxTimerFinished = false;

                    // Reset semua state
                    bpmWinFill  = 0;
                    bpmWinIdx   = 0;
                    bpmInit     = false;
                    spo2Init    = false;
                    avgReady    = false;
                    lastBeat    = 0;
                    spo2Index   = 0;
                    spo2Ready   = false;
                    spo2        = 0.0f;
                    spo2Smooth  = 0.0f;
                    bpm         = 0.0f;
                    avgBpm      = 0.0f;
                    bpmSmooth   = 0.0f;

                    strcpy(timerBuf, "00:01:00");
                    Serial.println("[MAX] Jari dilepas, timer reset");
                }
            }

            particleSensor.nextSample();
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

//========== TASK UNTUK MONITOR SCREEN ==========
void screenMonitorTask(void *pvParameters) {
    lv_obj_t *lastScreen = NULL;
    Serial.println("screenMonitorTask started");

    while (1) {
        lv_obj_t *currentScreen = lv_scr_act();  // Screen aktif saat ini

        // Jika screen berubah
        if (currentScreen != lastScreen) {
            lastScreen = currentScreen;

            // SCREEN 1
            if (currentScreen == guider_ui.screen_1) {
                Serial.println("SCREEN 1 AKTIF");

                mpuEnable = true;
                mlxEnable = false;
                maxEnable = true;
                maxVitalsEnable = false;

                resetMaxMeasurement(); // Optional
            }

            // SCREEN 2
            else if (currentScreen == guider_ui.screen_2) {
                Serial.println("SCREEN 2 AKTIF");

                mpuEnable = false;
                mlxEnable = true;
                maxEnable = true;
                maxVitalsEnable = true;

                // 🔴 RESET DATA SAAT MASUK SCREEN 2
                resetMaxMeasurement();
            }


        }
        

        // Di dalam screenMonitorTask, bagian SCREEN 1
        if (currentScreen == guider_ui.screen_1) {

            if (mpuData.needsUpdate && xSemaphoreTake(labelMutex, pdMS_TO_TICKS(10)) == pdTRUE) {

                // label_5 = Roll
                if (guider_ui.screen_1_label_5 != NULL) {
                    lv_label_set_text(guider_ui.screen_1_label_5, mpuData.rollBuf);
                }
                // label_6 = Pitch
                if (guider_ui.screen_1_label_6 != NULL) {
                    lv_label_set_text(guider_ui.screen_1_label_6, mpuData.pitchBuf);
                }

                const char* statusKal = mpuCalibrated ? "Kalibrasi Berhasil" : "Belum Kalibrasi";

                Serial.printf("[SCR1] Roll: %s | Pitch: %s | Status: %s\n",
                            mpuData.rollBuf, mpuData.pitchBuf, statusKal);

                mpuData.needsUpdate = false;
                xSemaphoreGive(labelMutex);
            }

            // Status jari MAX
            if (guider_ui.screen_1_label_10 != NULL) {
                lv_label_set_text(
                    guider_ui.screen_1_label_10,
                    maxFingerGood ? "Posisi Jari Sesuai" : "Posisi Jari Belum Tepat"
                );
            }
        }        

        // SCREEN 2 - Update MLX + MAX data
        else if (currentScreen == guider_ui.screen_2) {

            // --- MLX ---
            if (labelData.needsUpdate && xSemaphoreTake(labelMutex, 10) == pdTRUE) {
                lv_label_set_text(guider_ui.screen_2_label_11, labelData.suhuBuf);

                // 🟢 PRINT MLX KE SERIAL
                Serial.printf("[MLX] Suhu: %s\n", labelData.suhuBuf);

                labelData.needsUpdate = false;
                xSemaphoreGive(labelMutex);
            }

            // --- MAX30102 ---
            if (maxData.needsUpdate && xSemaphoreTake(labelMutex, 10) == pdTRUE) {
                lv_label_set_text(guider_ui.screen_2_label_9,  maxData.spo2Buf); // SpO2
                lv_label_set_text(guider_ui.screen_2_label_10, maxData.avgBuf);  // AVG BPM

                // 🔵 PRINT MAX KE SERIAL
                Serial.printf(
                    "[MAX] SpO2: %s | BPM: %s | AVG BPM: %s\n",
                    maxData.spo2Buf,
                    maxData.bpmBuf,
                    maxData.avgBuf
                );

                maxData.needsUpdate = false;
                xSemaphoreGive(labelMutex);
            }

            // ========== TIMER MAX UPDATE ==========
            if (maxTimerRunning)
            {
                unsigned long elapsed = (millis() - maxTimerStart) / 1000;
                int remaining = MEASURE_DURATION - elapsed;

                if (remaining <= 0)
                {
                    maxTimerRunning = false;
                    maxTimerFinished = true;

                    lv_label_set_text(
                        guider_ui.screen_2_label_16,
                        "Pengukuran\nSelesai"
                    );

                    Serial.println("⏱️ Timer selesai");
                }
                else
                {
                    int mm = 0;
                    int ss = remaining;

                    int total = remaining;

                    int hours = total / 3600;
                    int minutes = (total % 3600) / 60;
                    int seconds = total % 60;

                    sprintf(timerBuf, "%02d:%02d:%02d", hours, minutes, seconds);

                    lv_label_set_text(
                        guider_ui.screen_2_label_16,
                        timerBuf
                    );
                }
            }

            // Jika belum mulai & belum selesai → tampil default
            else if (!maxTimerFinished)
            {
                lv_label_set_text(
                    guider_ui.screen_2_label_16,
                    "00:01:00"
                );
            }

            // ========== STATUS STABILITAS JARI ==========
            if (maxFingerGood)
            {
                lv_label_set_text(
                    guider_ui.screen_2_label_19,
                    "Jari Stabil"
                );
            }
            else
            {
                lv_label_set_text(
                    guider_ui.screen_2_label_19,
                    "Jari Tidak Stabil"
                );
            }
        
            // ========== FUZZY — HITUNG SEKALI SAAT TIMER SELESAI ==========
            if (maxTimerFinished && !fuzzyDone)
            {
                float bpmVal  = atof(maxData.avgBuf);
                float spo2Val = atof(maxData.spo2Buf);
                float suhuVal = 0.0f;

                // Ambil suhu dengan aman
                if (xSemaphoreTake(labelMutex, pdMS_TO_TICKS(20)) == pdTRUE) {
                    suhuVal = atof(labelData.suhuBuf);
                    xSemaphoreGive(labelMutex);
                }

                // Hanya proses jika data valid
                if (bpmVal > 0.0f && spo2Val > 0.0f && suhuVal > 0.0f)
                {
                    FuzzyResult hasil = fuzzyAnalyze(bpmVal, spo2Val, suhuVal);

                    if (xSemaphoreTake(labelMutex, pdMS_TO_TICKS(20)) == pdTRUE) {
                        strncpy(fuzzyData.statusBuf, hasil.status.c_str(), sizeof(fuzzyData.statusBuf) - 1);
                        fuzzyData.needsUpdate = true;
                        xSemaphoreGive(labelMutex);
                    }

                    Serial.printf("[FUZZY] BPM=%.1f | SpO2=%.1f | Suhu=%.2f → %s (%.1f)\n",
                        bpmVal, spo2Val, suhuVal,
                        hasil.status.c_str(), hasil.crispValue);

                    fuzzyDone = true;  // ← Kunci agar tidak dipanggil lagi
                }
            }

            // Update label fuzzy ke layar (terpisah dari kalkulasi)
            if (fuzzyData.needsUpdate && xSemaphoreTake(labelMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
                lv_label_set_text(guider_ui.screen_2_label_15, fuzzyData.statusBuf);
                fuzzyData.needsUpdate = false;
                xSemaphoreGive(labelMutex);
            }        
        }

        vTaskDelay(pdMS_TO_TICKS(300)); // cek tiap 300 ms
    }
}

// ========== btn3_event_handler — VERSI FINAL ==========
extern "C" void btn3_event_handler(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    Serial.println("[CAL] Tombol kalibrasi ditekan");

    // ── Langkah 1: Hentikan mpuTask sementara ──
    // Wajib! Tanpa ini mpuTask bisa timpa data di tengah proses kalibrasi
    mpuEnable = false;
    vTaskDelay(pdMS_TO_TICKS(150));  // Tunggu 1 siklus mpuTask selesai (100ms + buffer)

    // ── Langkah 2: Baca sensor FRESH langsung dari hardware ──
    // Gunakan pembacaan baru dari sensor, bukan nilai yang mungkin sudah usang
    sensors_event_t a, g, temp_ev;
    bool readOk = false;

    if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(300)) == pdTRUE) {
        mpu.getEvent(&a, &g, &temp_ev);
        readOk = true;
        xSemaphoreGive(i2cMutex);
    }

    if (!readOk) {
        Serial.println("[CAL] Gagal baca sensor, kalibrasi dibatalkan");
        mpuEnable = true;
        return;
    }

    float ax = a.acceleration.x;
    float ay = a.acceleration.y;
    float az = a.acceleration.z;

    // ── Langkah 3: Hitung pitch/roll posisi saat ini ──
    float pitch_now = atan2(-ax, sqrt(ay*ay + az*az)) * 180.0f / M_PI;
    float roll_now  = atan2(ay, az) * 180.0f / M_PI;

    Serial.printf("[CAL] Posisi saat ini → pitch=%.3f | roll=%.3f\n",
                  pitch_now, roll_now);

    // ── Langkah 4: Set referensi & flush buffer ──
    if (xSemaphoreTake(mpuMutex, pdMS_TO_TICKS(200)) == pdTRUE) {

        // Referensi = posisi fisik saat tombol ditekan
        pitch_ref = pitch_now;
        roll_ref  = roll_now;

        // Flush buffer dengan nilai referensi
        // → Saat mpuTask jalan lagi: avg = ref → cal = avg - ref = 0.0
        for (int i = 0; i < FILTER_SIZE; i++) {
            pitch_buf[i] = pitch_now;
            roll_buf[i]  = roll_now;
        }
        filter_idx   = 0;
        filter_ready = true;

        // Reset hysteresis agar tidak false trigger setelah kalibrasi
        lastPitch     = 0.0f;
        lastRoll      = 0.0f;
        fingerStable  = true;

        mpuCalibrated = true;

        xSemaphoreGive(mpuMutex);
    } else {
        Serial.println("[CAL] Gagal mengunci data MPU, kalibrasi dibatalkan");
        mpuEnable = true;
        return;
    }

    // ── Langkah 5: Paksa label tampil 0.0 SEKARANG ──
    // Tanpa ini label baru update saat screenMonitorTask baca mpuData
    // yang baru di-set oleh mpuTask di siklus berikutnya
    if (xSemaphoreTake(labelMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        strcpy(mpuData.rollBuf,  "0.0");
        strcpy(mpuData.pitchBuf, "0.0");
        mpuData.needsUpdate = true;
        xSemaphoreGive(labelMutex);
    }

    Serial.printf("[CAL] Selesai → pitch_ref=%.3f | roll_ref=%.3f\n",
                  pitch_ref, roll_ref);
    Serial.println("[CAL] Label seharusnya menampilkan 0.0 sekarang");

    // ── Langkah 6: Aktifkan kembali mpuTask ──
    mpuEnable = true;
}

// ========== Inisialisasi UI ==========
void init_ui() {
    strcpy(fuzzyData.statusBuf, "--");
    setup_ui(&guider_ui);          // Memanggil setup dari gui_guider.c
    events_init(&guider_ui);       // Inisialisasi event handler
    lv_obj_add_event_cb(guider_ui.screen_1_btn_3, btn3_event_handler, LV_EVENT_CLICKED, NULL);
    init_scr_del_flag(&guider_ui); // Inisialisasi flag screen delete
}

// ========== LVGL Task ==========
void lvglTask(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    while (1) {
        lv_tick_inc(10);
        lv_timer_handler();
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10));
    }
}

//========== Setup dan Loop Utama ==========
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n\n=== SETUP START ===");
    
    TouchWire.begin(TOUCH_SDA, TOUCH_SCL);
    I2C_sensor.begin(SDA, SCL);
    mutex_manager.initMutex();

    init_display();
    init_touch();
    init_ui();

    delay(500);

    // Inisialisasi MLX90614 dengan I2C_sensor (Wire 1)
    if (!mlx.begin(0x5A, &I2C_sensor)) {
        Serial.println("MLX90614 tidak terdeteksi! Lanjut tanpa MLX...");
    } else {
        Serial.println("MLX90614 initialized successfully");
    }

    // Inisialisasi MPU6050 dengan I2C_sensor (Wire 1)
    if (!mpu.begin(0x68, &I2C_sensor)) {
        Serial.println("MPU6050 tidak terdeteksi! Lanjut tanpa MPU...");
    } else {
        Serial.println("MPU6050 initialized successfully");
        mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
        mpu.setGyroRange(MPU6050_RANGE_500_DEG);
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    }

    // ========== INIT MAX30102 ==========
    if (!particleSensor.begin(I2C_sensor, I2C_SPEED_STANDARD))
    {
        Serial.println("❌ MAX30102 tidak terdeteksi!");
    }
    else
    {
        Serial.println("✅ MAX30102 initialized");

        particleSensor.setup(
            0x1F,  // LED Brightness (NAIK)
            1,     // Sample average (WAJIB 1)
            2,     // RED + IR
            200,   // Sample rate (NAIK)
            215,   // Pulse width (LEBIH TAJAM)
            16384  // ADC range (LEBAR)
        );
        particleSensor.setPulseAmplitudeRed(0x10);
        particleSensor.setPulseAmplitudeIR(0x1F);
        particleSensor.setPulseAmplitudeGreen(0);

    }

    // Create I2C mutex untuk synchronization
    i2cMutex = xSemaphoreCreateMutex();

    labelMutex = xSemaphoreCreateMutex();

    // Create MPU mutex untuk calibration
    mpuMutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(lvglTask, "LVGL Task", 8192, NULL, 3, NULL, 1);
    // xTaskCreatePinnedToCore(touchTask, "Touch Task", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(screenMonitorTask, "Screen Monitor Task", 8192, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(mlxTask, "MLX Task", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(mpuTask, "MPU Task", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(maxTask, "MAX Task", 8192, NULL, 1, NULL, 1);

    Serial.println("Setup complete, running.");
}

void loop() {
    // Kosong, semua ditangani oleh task FreeRTOS
}