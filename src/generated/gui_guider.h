/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


typedef struct
{
  
	lv_obj_t *screen_1;
	bool screen_1_del;
	lv_obj_t *screen_1_img_1;
	lv_obj_t *screen_1_img_11;
	lv_obj_t *screen_1_img_10;
	lv_obj_t *screen_1_img_9;
	lv_obj_t *screen_1_img_2;
	lv_obj_t *screen_1_img_3;
	lv_obj_t *screen_1_img_4;
	lv_obj_t *screen_1_label_1;
	lv_obj_t *screen_1_label_2;
	lv_obj_t *screen_1_label_3;
	lv_obj_t *screen_1_label_4;
	lv_obj_t *screen_1_label_5;
	lv_obj_t *screen_1_label_6;
	lv_obj_t *screen_1_img_5;
	lv_obj_t *screen_1_img_6;
	lv_obj_t *screen_1_img_7;
	lv_obj_t *screen_1_img_8;
	lv_obj_t *screen_1_label_7;
	lv_obj_t *screen_1_btn_1;
	lv_obj_t *screen_1_btn_1_label;
	lv_obj_t *screen_1_max30102;
	lv_obj_t *screen_1_header_max;
	lv_obj_t *screen_1_btn_4;
	lv_obj_t *screen_1_btn_4_label;
	lv_obj_t *screen_1_sensor_berhasil;
	lv_obj_t *screen_1_label_10;
	lv_obj_t *screen_1_awal_sensor;
	lv_obj_t *screen_1_btn_5;
	lv_obj_t *screen_1_btn_5_label;
	lv_obj_t *screen_1_label_12;
	lv_obj_t *screen_1_img_15;
	lv_obj_t *screen_1_kalibrasi;
	lv_obj_t *screen_1_kal_selesai;
	lv_obj_t *screen_1_img_12;
	lv_obj_t *screen_1_label_9;
	lv_obj_t *screen_1_kal_mulai;
	lv_obj_t *screen_1_label_8;
	lv_obj_t *screen_1_btn_3;
	lv_obj_t *screen_1_btn_3_label;
	lv_obj_t *screen_1_header_kal;
	lv_obj_t *screen_1_btn_2;
	lv_obj_t *screen_1_btn_2_label;
	lv_obj_t *screen_2;
	bool screen_2_del;
	lv_obj_t *screen_2_img_1;
	lv_obj_t *screen_2_img_18;
	lv_obj_t *screen_2_img_13;
	lv_obj_t *screen_2_img_14;
	lv_obj_t *screen_2_img_2;
	lv_obj_t *screen_2_img_3;
	lv_obj_t *screen_2_img_16;
	lv_obj_t *screen_2_img_5;
	lv_obj_t *screen_2_img_6;
	lv_obj_t *screen_2_img_7;
	lv_obj_t *screen_2_img_8;
	lv_obj_t *screen_2_img_10;
	lv_obj_t *screen_2_label_1;
	lv_obj_t *screen_2_label_2;
	lv_obj_t *screen_2_line_1;
	lv_obj_t *screen_2_line_2;
	lv_obj_t *screen_2_label_4;
	lv_obj_t *screen_2_label_9;
	lv_obj_t *screen_2_label_5;
	lv_obj_t *screen_2_label_10;
	lv_obj_t *screen_2_label_6;
	lv_obj_t *screen_2_label_11;
	lv_obj_t *screen_2_label_14;
	lv_obj_t *screen_2_label_15;
	lv_obj_t *screen_2_img_17;
	lv_obj_t *screen_2_img_19;
	lv_obj_t *screen_2_label_16;
	lv_obj_t *screen_2_label_17;
	lv_obj_t *screen_2_line_3;
	lv_obj_t *screen_2_label_19;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen_1(lv_ui *ui);
void setup_scr_screen_2(lv_ui *ui);
LV_IMAGE_DECLARE(_Background1_RGB565A8_320x170);
LV_IMAGE_DECLARE(_Kotak2_RGB565A8_269x28);
LV_IMAGE_DECLARE(_Line_RGB565A8_160x160);
LV_IMAGE_DECLARE(_Kotak2_RGB565A8_129x109);
LV_IMAGE_DECLARE(_Kotak2_RGB565A8_265x24);
LV_IMAGE_DECLARE(_Logo_Accel_RGB565A8_22x20);
LV_IMAGE_DECLARE(_Gyro_RGB565A8_42x36);
LV_IMAGE_DECLARE(_Heart_RGB565A8_42x28);
LV_IMAGE_DECLARE(_Fingerprint_RGB565A8_61x65);
LV_IMAGE_DECLARE(_DoneDL_RGB565A8_39x38);
LV_IMAGE_DECLARE(_Line_RGB565A8_106x149);
LV_IMAGE_DECLARE(_Line_RGB565A8_125x168);
LV_IMAGE_DECLARE(_Kotak2_RGB565A8_100x115);
LV_IMAGE_DECLARE(_Kotak2_RGB565A8_234x28);
LV_IMAGE_DECLARE(_Kotak2_RGB565A8_230x24);
LV_IMAGE_DECLARE(_UNY_RGB565A8_32x32);
LV_IMAGE_DECLARE(_Lingkaran_RGB565A8_88x90);
LV_IMAGE_DECLARE(_Heart_RGB565A8_50x25);
LV_IMAGE_DECLARE(_Temperatur_RGB565A8_38x38);
LV_IMAGE_DECLARE(_Kotak2_RGB565A8_85x100);

LV_FONT_DECLARE(lv_font_Acme_Regular_16)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_Acme_Regular_14)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_13)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_17)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_12)
LV_FONT_DECLARE(lv_font_Enriqueta_SemiBold_11)
LV_FONT_DECLARE(lv_font_Enriqueta_SemiBold_12)
LV_FONT_DECLARE(lv_font_Enriqueta_SemiBold_16)
LV_FONT_DECLARE(lv_font_Acme_Regular_15)
LV_FONT_DECLARE(lv_font_Acme_Regular_12)
LV_FONT_DECLARE(lv_font_Acme_Regular_11)


#ifdef __cplusplus
}
#endif
#endif
