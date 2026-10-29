#pragma once
#include <Arduino.h>

// float trapMF(float x, float a, float b, float c, float d) {
//     if (x <= a || x >= d) return 0.0f; // Di luar batas → tidak masuk kelompok
//     if (x >= b && x <= c) return 1.0f; // Di zona puncak → sepenuhnya masuk
//     if (x < b) return (x - a) / (b - a); // Sisi naik
//     return (d - x) / (d - c);            // Sisi turun
// }

float trapMF(float x, float a, float b, float c, float d) {
    if (x < a || x > d) return 0.0f;

    if (x >= b && x <= c) return 1.0f;

    if (x > a && x < b) {
        if (b == a) return 1.0f;
        return (x - a) / (b - a);
    }

    if (x > c && x < d) {
        if (d == c) return 1.0f;
        return (d - x) / (d - c);
    }

    return 0.0f;
}

// float triMF(float x, float a, float b, float c) {
//     if (x <= a || x >= c) return 0.0f; // Di luar batas → tidak masuk
//     if (x == b) return 1.0f;           // Tepat di puncak → sepenuhnya masuk
//     if (x < b) return (x - a) / (b - a); // Sisi naik
//     return (c - x) / (c - b);            // Sisi turun
// }

float triMF(float x, float a, float b, float c) {
    if (x <= a || x >= c) return 0.0f;

    if (x == b) return 1.0f;

    if (x < b)
        return (x - a) / (b - a);

    return (c - x) / (c - b);
}

struct BPMFuzzy {
    float rendah;  // Derajat keanggotaan kelompok Rendah/Bradikardia
    float normal;  // Derajat keanggotaan kelompok Normal
    float tinggi;  // Derajat keanggotaan kelompok Tinggi/Takikardia
};

BPMFuzzy fuzzBPM(float bpm) {
    return {
        // RENDAH (Bradikardia): puncak di 30-55 bpm, habis di 62 bpm
        // Ref [2]: bradikardia < 60 bpm; overlap kecil sampai 62 untuk transisi halus
        trapMF(bpm, 30.0f, 30.0f, 55.0f, 62.0f),

        // NORMAL: mulai masuk di 55 bpm, penuh di 65-90 bpm, habis di 100 bpm
        // Ref [1][2]: normal dewasa 60-100 bpm; batas bawah 55 untuk atlet
        trapMF(bpm, 55.0f, 65.0f, 90.0f, 100.0f),

        // TINGGI (Takikardia): mulai masuk di 95 bpm, penuh di 105+ bpm
        // Ref [2]: takikardia > 100 bpm; overlap 95-105 untuk zona peralihan
        trapMF(bpm, 95.0f, 105.0f, 180.0f, 180.0f)
    };
}

struct SpO2Fuzzy {
    float rendah;  // Derajat keanggotaan Hipoksemia (berbahaya)
    float sedang;  // Derajat keanggotaan Perlu Perhatian
    float normal;  // Derajat keanggotaan Normal
};

SpO2Fuzzy fuzzSpO2(float spo2) {
    return {
        // RENDAH (Hipoksemia): puncak di 75-87%, habis di 91%
        // Ref [3]: < 90% = hipoksemia; < 85% = hipoksemia berat
        trapMF(spo2, 75.0f, 75.0f, 87.0f, 91.0f),

        // SEDANG (Perlu Perhatian): puncak di 92%, rentang 88-96%
        // Ref [3]: 90.1-94.9% = abnormal, perlu kewaspadaan
        triMF(spo2, 88.0f, 92.0f, 96.0f),

        // NORMAL: mulai masuk di 94%, penuh di 97-100%
        // Ref [1][3]: ≥95% normal; overlap mulai 94% untuk transisi
        trapMF(spo2, 94.0f, 97.0f, 100.0f, 100.0f)
    };
}

struct SuhuFuzzy {
    float hipo;   // Derajat keanggotaan Hipotermia
    float normal; // Derajat keanggotaan Normal
    float demam;  // Derajat keanggotaan Demam/Pyrexia
};

SuhuFuzzy fuzzSuhu(float suhu) {
    return {
        // HIPOTERMIA: puncak di 30-33°C, habis di 35°C
        trapMF(suhu, 33.0f, 33.0f, 34.5f, 35.0f),

        // NORMAL: mulai masuk di 34.5°C, penuh di 36.5-37.5°C, habis di 38°C
        trapMF(suhu, 34.5f, 36.5f, 37.5f, 38.0f),

        // DEMAM: mulai masuk di 37.5°C, penuh di 38-42°C
        trapMF(suhu, 37.5f, 38.0f, 42.0f, 42.0f)
    };
}

struct FuzzyOutput {
    float normal;  // Derajat aktifnya output NORMAL
    float waspada; // Derajat aktifnya output WASPADA
    float bahaya;  // Derajat aktifnya output BAHAYA
};

FuzzyOutput evalRules(BPMFuzzy b, SpO2Fuzzy s, SuhuFuzzy t) {

    FuzzyOutput out = {0.0f, 0.0f, 0.0f};

    // Fungsi pembantu
    // mn3 = AND operator (ambil nilai minimum dari 3 input)
    auto mn3 = [](float a, float b, float c) -> float {
        float m = (a < b) ? a : b;
        return (m < c) ? m : c;
    };
    // mx = MAX operator untuk agregasi (ambil nilai terbesar)
    auto mx = [](float a, float b) -> float {
        return (a > b) ? a : b;
    };

    // ============================================================
    //  RULE 1-9: BPM NORMAL
    //  Jika jantung berdetak normal (60-100 bpm),
    //  hasil akhir bergantung pada SpO2 dan Suhu
    // ============================================================

    // Rule 1: BPM Normal + SpO2 Normal + Suhu Normal → NORMAL
    // Semua parameter baik → kondisi sehat
    out.normal = mx(out.normal, mn3(b.normal, s.normal, t.normal));

    // Rule 2: BPM Normal + SpO2 Normal + Suhu Hipotermia → WASPADA
    // Suhu terlalu rendah walau jantung dan oksigen baik → perlu perhatian
    out.waspada = mx(out.waspada, mn3(b.normal, s.normal, t.hipo));

    // Rule 3: BPM Normal + SpO2 Normal + Suhu Demam → WASPADA
    // Suhu terlalu tinggi walau jantung dan oksigen baik → perlu perhatian
    out.waspada = mx(out.waspada, mn3(b.normal, s.normal, t.demam));

    // Rule 4: BPM Normal + SpO2 Sedang + Suhu Normal → WASPADA
    // SpO2 sedikit rendah (90-95%) → perlu dipantau
    out.waspada = mx(out.waspada, mn3(b.normal, s.sedang, t.normal));

    // Rule 5: BPM Normal + SpO2 Sedang + Suhu Hipotermia → WASPADA
    // Dua parameter kurang baik tapi belum kritis
    // out.waspada = mx(out.waspada, mn3(b.normal, s.sedang, t.hipo));

    // Rule 5: BPM Normal + SpO2 Sedang + Suhu Hipotermia → BAHAYA
    out.bahaya = mx(out.bahaya,mn3(b.normal, s.sedang, t.hipo));

    // Rule 6: BPM Normal + SpO2 Sedang + Suhu Demam → BAHAYA
    // SpO2 rendah + demam → kombinasi berbahaya (bisa infeksi berat)
    out.bahaya = mx(out.bahaya, mn3(b.normal, s.sedang, t.demam));

    // Rule 7: BPM Normal + SpO2 Rendah + Suhu Normal → BAHAYA
    // SpO2 < 90% = hipoksemia → selalu BAHAYA [Ref 3]
    out.bahaya = mx(out.bahaya, mn3(b.normal, s.rendah, t.normal));

    // Rule 8: BPM Normal + SpO2 Rendah + Suhu Hipotermia → BAHAYA
    // Hipoksemia + hipotermia → kondisi kritis
    out.bahaya = mx(out.bahaya, mn3(b.normal, s.rendah, t.hipo));

    // Rule 9: BPM Normal + SpO2 Rendah + Suhu Demam → BAHAYA
    // Hipoksemia + demam → kondisi sangat kritis
    out.bahaya = mx(out.bahaya, mn3(b.normal, s.rendah, t.demam));

    // ============================================================
    //  RULE 10-18: BPM RENDAH (Bradikardia < 60 bpm)
    //  Ref [2]: bradikardia bisa normal pada atlet,
    //  tapi kombinasi dengan parameter lain yang buruk = bahaya
    // ============================================================

    // Rule 10: BPM Rendah + SpO2 Normal + Suhu Normal → WASPADA
    // Bradikardia saja belum tentu bahaya, tapi perlu dipantau
    out.waspada = mx(out.waspada, mn3(b.rendah, s.normal, t.normal));

    // Rule 11: BPM Rendah + SpO2 Normal + Suhu Hipotermia → BAHAYA
    // Bradikardia + hipotermia → tanda syok atau gagal jantung
    out.bahaya = mx(out.bahaya, mn3(b.rendah, s.normal, t.hipo));

    // Rule 12: BPM Rendah + SpO2 Normal + Suhu Demam → BAHAYA
    // Bradikardia + demam tinggi → tidak normal, butuh evaluasi segera
    out.bahaya = mx(out.bahaya, mn3(b.rendah, s.normal, t.demam));

    // Rule 13: BPM Rendah + SpO2 Sedang + Suhu Normal → BAHAYA
    // Dua parameter bermasalah → BAHAYA
    out.bahaya = mx(out.bahaya, mn3(b.rendah, s.sedang, t.normal));

    // Rule 14: BPM Rendah + SpO2 Sedang + Suhu Hipotermia → BAHAYA
    out.bahaya = mx(out.bahaya, mn3(b.rendah, s.sedang, t.hipo));

    // Rule 15: BPM Rendah + SpO2 Sedang + Suhu Demam → BAHAYA
    out.bahaya = mx(out.bahaya, mn3(b.rendah, s.sedang, t.demam));

    // Rule 16: BPM Rendah + SpO2 Rendah + Suhu Normal → BAHAYA
    // Bradikardia + hipoksemia → kondisi kritis [Ref 3]
    out.bahaya = mx(out.bahaya, mn3(b.rendah, s.rendah, t.normal));

    // Rule 17: BPM Rendah + SpO2 Rendah + Suhu Hipotermia → BAHAYA
    out.bahaya = mx(out.bahaya, mn3(b.rendah, s.rendah, t.hipo));

    // Rule 18: BPM Rendah + SpO2 Rendah + Suhu Demam → BAHAYA
    out.bahaya = mx(out.bahaya, mn3(b.rendah, s.rendah, t.demam));

    // ============================================================
    //  RULE 19-27: BPM TINGGI (Takikardia > 100 bpm)
    //  Ref [2]: takikardia bisa normal saat aktivitas,
    //  tapi saat istirahat + parameter lain buruk = berbahaya
    // ============================================================

    // Rule 19: BPM Tinggi + SpO2 Normal + Suhu Normal → WASPADA
    // Takikardia saat istirahat → perlu perhatian tapi belum bahaya
    out.waspada = mx(out.waspada, mn3(b.tinggi, s.normal, t.normal));

    // Rule 20: BPM Tinggi + SpO2 Normal + Suhu Hipotermia → BAHAYA
    // Takikardia + hipotermia → tanda syok [Ref 4]
    out.bahaya = mx(out.bahaya, mn3(b.tinggi, s.normal, t.hipo));

    // Rule 21: BPM Tinggi + SpO2 Normal + Suhu Demam → BAHAYA
    // Takikardia + demam → respons infeksi serius
    out.bahaya = mx(out.bahaya, mn3(b.tinggi, s.normal, t.demam));

    // Rule 22: BPM Tinggi + SpO2 Sedang + Suhu Normal → BAHAYA
    // Dua parameter bermasalah → BAHAYA
    out.bahaya = mx(out.bahaya, mn3(b.tinggi, s.sedang, t.normal));

    // Rule 23: BPM Tinggi + SpO2 Sedang + Suhu Hipotermia → BAHAYA
    out.bahaya = mx(out.bahaya, mn3(b.tinggi, s.sedang, t.hipo));

    // Rule 24: BPM Tinggi + SpO2 Sedang + Suhu Demam → BAHAYA
    out.bahaya = mx(out.bahaya, mn3(b.tinggi, s.sedang, t.demam));

    // Rule 25: BPM Tinggi + SpO2 Rendah + Suhu Normal → BAHAYA
    // Takikardia + hipoksemia → kondisi kritis [Ref 3]
    out.bahaya = mx(out.bahaya, mn3(b.tinggi, s.rendah, t.normal));

    // Rule 26: BPM Tinggi + SpO2 Rendah + Suhu Hipotermia → BAHAYA
    out.bahaya = mx(out.bahaya, mn3(b.tinggi, s.rendah, t.hipo));

    // Rule 27: BPM Tinggi + SpO2 Rendah + Suhu Demam → BAHAYA
    out.bahaya = mx(out.bahaya, mn3(b.tinggi, s.rendah, t.demam));

    return out;
}


// ================================================================
//  TAHAP 3+4 — DEFUZZIFIKASI (Centroid of Area / COA)
//
//  Metode: Centroid (pusat massa area)
//  Ref [6]: metode defuzzifikasi umum pada sistem fuzzy medis
//
//  Cara kerja:
//  1. Sampling output universe dari 0 sampai 100 (per 1 langkah)
//  2. Setiap titik z, hitung tinggi MF output masing-masing
//  3. Clipping: potong MF output sebatas derajat aktifnya (Mamdani)
//  4. Agregasi: ambil MAX dari ketiga MF yang sudah di-clip
//  5. Hitung centroid: Σ(z × agg) / Σ(agg)
//
//  Output universe MF:
//  NORMAL  : trapMF(0, 0, 25, 40)   → puncak di 0-25
//  WASPADA : triMF(30, 50, 70)      → puncak di 50
//  BAHAYA  : trapMF(60, 75, 100,100)→ puncak di 75-100
// ================================================================

float defuzzify(FuzzyOutput fo) {

    float numerator   = 0.0f; // Σ(z × agregasi)
    float denominator = 0.0f; // Σ(agregasi)

    for (int z = 0; z <= 100; z++) {

        // Hitung tinggi MF output di titik z
        float mfNormal  = trapMF((float)z,  0.0f,  0.0f, 25.0f, 40.0f);
        float mfWaspada = triMF ((float)z, 30.0f, 50.0f, 70.0f);
        float mfBahaya  = trapMF((float)z, 60.0f, 75.0f, 100.0f, 100.0f);

        // Clipping Mamdani: potong MF sebatas derajat aktifnya
        // Contoh: fo.normal=0.6, mfNormal di z=10 adalah 1.0
        //         setelah clipping → min(1.0, 0.6) = 0.6
        float yN = (mfNormal  < fo.normal)  ? mfNormal  : fo.normal;
        float yW = (mfWaspada < fo.waspada) ? mfWaspada : fo.waspada;
        float yB = (mfBahaya  < fo.bahaya)  ? mfBahaya  : fo.bahaya;

        // Agregasi: ambil nilai tertinggi dari ketiga (MAX)
        float agg = yN;
        if (yW > agg) agg = yW;
        if (yB > agg) agg = yB;

        // Akumulasi untuk centroid
        numerator   += (float)z * agg;
        denominator += agg;
    }

    // Jika semua MF = 0 (tidak ada rule aktif sama sekali) → kembalikan 0
    // if (denominator == 0.0f) return 0.0f;
    if (denominator == 0.0f) return -1.0f;

    return numerator / denominator;
}


// ================================================================
//  FUNGSI UTAMA — fuzzyAnalyze()
//
//  Memanggil keempat tahap secara berurutan dan mengembalikan hasil.
//
//  Batas keputusan akhir (thresholding):
//  crisp < 42  → NORMAL  (zona aman, centroid condong ke kiri)
//  42-58       → WASPADA (centroid di tengah, zona peralihan)
//  crisp ≥ 58  → BAHAYA  (centroid condong ke kanan)
//
//  Mengapa 42 dan 58, bukan 33 dan 66?
//  Karena centroid COA tidak pernah mencapai 0 atau 100 murni
//  akibat overlapping MF. Batas 42/58 dikalibrasi agar:
//  - BPM=75, SpO2=98, Suhu=36.8 → pasti NORMAL
//  - BPM=110, SpO2=96, Suhu=38.5 → pasti BAHAYA
//  Pendekatan ini umum digunakan dalam sistem fuzzy embedded
//  (Ref [6][7]).
// ================================================================

struct FuzzyResult {
    float  crispValue; // Nilai defuzzifikasi mentah (0-100)
    String status;     // "NORMAL" / "WASPADA" / "BAHAYA"
    float  muNormal;   // Derajat aktif output Normal (untuk laporan)
    float  muWaspada;  // Derajat aktif output Waspada (untuk laporan)
    float  muBahaya;   // Derajat aktif output Bahaya (untuk laporan)
};

// FuzzyResult fuzzyAnalyze(float bpm, float spo2, float suhu) {

//     // ── Tahap 1: Fuzzifikasi ──────────────────────────────────
//     BPMFuzzy  b = fuzzBPM(bpm);
//     SpO2Fuzzy s = fuzzSpO2(spo2);
//     SuhuFuzzy t = fuzzSuhu(suhu);

//     // ── Tahap 2+3: Inferensi & Agregasi ──────────────────────
//     FuzzyOutput fo = evalRules(b, s, t);

//     // ── Tahap 4: Defuzzifikasi ────────────────────────────────
//     float crisp = defuzzify(fo);

//     // ── Keputusan Akhir ───────────────────────────────────────
//     String status;
//     if      (crisp < 42.0f) status = "NORMAL";
//     else if (crisp < 58.0f) status = "WASPADA";
//     else                    status = "BAHAYA";

//     return {
//         crisp,
//         status,
//         fo.normal,
//         fo.waspada,
//         fo.bahaya
//     };
// }

FuzzyResult fuzzyAnalyze(float bpm, float spo2, float suhu) {

    // Jaga nilai tetap di dalam universe MF (di luar itu semua MF = 0)
    bpm  = constrain(bpm,  30.0f, 180.0f);
    spo2 = constrain(spo2, 75.0f, 100.0f);
    suhu = constrain(suhu, 33.0f, 42.0f);

    BPMFuzzy  b = fuzzBPM(bpm);
    SpO2Fuzzy s = fuzzSpO2(spo2);
    SuhuFuzzy t = fuzzSuhu(suhu);

    FuzzyOutput fo = evalRules(b, s, t);
    float crisp = defuzzify(fo);

    String status;
    if      (crisp < 0.0f)  status = "TIDAK VALID";
    else if (crisp < 42.0f) status = "NORMAL";
    else if (crisp < 58.0f) status = "WASPADA";
    else                    status = "BAHAYA";

    return { crisp, status, fo.normal, fo.waspada, fo.bahaya };
}