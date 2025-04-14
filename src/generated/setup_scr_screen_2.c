/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen_2(lv_ui *ui)
{
    //Write codes screen_2
    ui->screen_2 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_2, 320, 170);
    lv_obj_set_scrollbar_mode(ui->screen_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_img_5
    ui->screen_2_img_5 = lv_image_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_img_5, 11, 43);
    lv_obj_set_size(ui->screen_2_img_5, 68, 115);
    lv_obj_add_flag(ui->screen_2_img_5, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_2_img_5, &_Kotak2_RGB565A8_68x115);
    lv_image_set_pivot(ui->screen_2_img_5, 50,50);
    lv_image_set_rotation(ui->screen_2_img_5, 0);

    //Write style for screen_2_img_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_2_img_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_2_img_5, lv_color_hex(0x1B1C1F), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_2_img_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_img_20
    ui->screen_2_img_20 = lv_image_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_img_20, 198, 103);
    lv_obj_set_size(ui->screen_2_img_20, 110, 55);
    lv_obj_add_flag(ui->screen_2_img_20, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_2_img_20, &_Kotak2_RGB565A8_110x55);
    lv_image_set_pivot(ui->screen_2_img_20, 50,50);
    lv_image_set_rotation(ui->screen_2_img_20, 0);

    //Write style for screen_2_img_20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_2_img_20, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_2_img_20, lv_color_hex(0x1B1C1F), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_2_img_20, 199, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_img_18
    ui->screen_2_img_18 = lv_image_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_img_18, 198, 43);
    lv_obj_set_size(ui->screen_2_img_18, 110, 55);
    lv_obj_add_flag(ui->screen_2_img_18, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_2_img_18, &_Kotak2_RGB565A8_110x55);
    lv_image_set_pivot(ui->screen_2_img_18, 50,50);
    lv_image_set_rotation(ui->screen_2_img_18, 0);

    //Write style for screen_2_img_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_2_img_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_2_img_18, lv_color_hex(0x1B1C1F), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_2_img_18, 199, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_img_19
    ui->screen_2_img_19 = lv_image_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_img_19, 84, 103);
    lv_obj_set_size(ui->screen_2_img_19, 110, 55);
    lv_obj_add_flag(ui->screen_2_img_19, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_2_img_19, &_Kotak2_RGB565A8_110x55);
    lv_image_set_pivot(ui->screen_2_img_19, 50,50);
    lv_image_set_rotation(ui->screen_2_img_19, 0);

    //Write style for screen_2_img_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_2_img_19, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_2_img_19, lv_color_hex(0x1B1C1F), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_2_img_19, 199, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_img_15
    ui->screen_2_img_15 = lv_image_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_img_15, 84, 43);
    lv_obj_set_size(ui->screen_2_img_15, 110, 55);
    lv_obj_add_flag(ui->screen_2_img_15, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_2_img_15, &_Kotak2_RGB565A8_110x55);
    lv_image_set_pivot(ui->screen_2_img_15, 50,50);
    lv_image_set_rotation(ui->screen_2_img_15, 0);

    //Write style for screen_2_img_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_2_img_15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_2_img_15, lv_color_hex(0x1B1C1F), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_2_img_15, 199, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_1
    ui->screen_2_label_1 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_1, 11, 12);
    lv_obj_set_size(ui->screen_2_label_1, 76, 20);
    lv_label_set_text(ui->screen_2_label_1, "Battery II");
    lv_label_set_long_mode(ui->screen_2_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_1, &lv_font_MonaSans_Variable_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_2
    ui->screen_2_label_2 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_2, 120, 54);
    lv_obj_set_size(ui->screen_2_label_2, 71, 17);
    lv_label_set_text(ui->screen_2_label_2, "12h 10m");
    lv_label_set_long_mode(ui->screen_2_label_2, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_2, &lv_font_MonaSans_Variable_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_3
    ui->screen_2_label_3 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_3, 120, 78);
    lv_obj_set_size(ui->screen_2_label_3, 68, 13);
    lv_label_set_text(ui->screen_2_label_3, "Remain Time");
    lv_label_set_long_mode(ui->screen_2_label_3, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_3, &lv_font_MonaSans_Variable_9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_6
    ui->screen_2_label_6 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_6, 94, 16);
    lv_obj_set_size(ui->screen_2_label_6, 90, 17);
    lv_label_set_text(ui->screen_2_label_6, "Charging / Idle");
    lv_label_set_long_mode(ui->screen_2_label_6, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_6, lv_color_hex(0x00ff71), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_6, &lv_font_MonaSans_Variable_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_img_6
    ui->screen_2_img_6 = lv_image_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_img_6, 25, 51);
    lv_obj_set_size(ui->screen_2_img_6, 42, 84);
    lv_obj_add_flag(ui->screen_2_img_6, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_2_img_6, &_Battery2_RGB565A8_42x84);
    lv_image_set_pivot(ui->screen_2_img_6, 50,50);
    lv_image_set_rotation(ui->screen_2_img_6, 0);

    //Write style for screen_2_img_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_2_img_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_2_img_6, lv_color_hex(0x6f6f6f), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_2_img_6, 110, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_bar_2
    ui->screen_2_bar_2 = lv_bar_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_bar_2, 26, 58);
    lv_obj_set_size(ui->screen_2_bar_2, 40, 75);
    lv_obj_set_style_anim_duration(ui->screen_2_bar_2, 1000, 0);
    lv_bar_set_mode(ui->screen_2_bar_2, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_2_bar_2, 0, 100);
    lv_bar_set_value(ui->screen_2_bar_2, 50, LV_ANIM_ON);

    //Write style for screen_2_bar_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_2_bar_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_2_bar_2, lv_color_hex(0x6f6f6f), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_2_bar_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_bar_2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_bar_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_2_bar_2, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_2_bar_2, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_2_bar_2, lv_color_hex(0x3300d6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_2_bar_2, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_bar_2, 3, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_2_label_8
    ui->screen_2_label_8 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_8, 27, 91);
    lv_obj_set_size(ui->screen_2_label_8, 39, 15);
    lv_label_set_text(ui->screen_2_label_8, "100%");
    lv_label_set_long_mode(ui->screen_2_label_8, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_8, &lv_font_MonaSans_Variable_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_9
    ui->screen_2_label_9 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_9, 243, 55);
    lv_obj_set_size(ui->screen_2_label_9, 38, 25);
    lv_label_set_text(ui->screen_2_label_9, "80%");
    lv_label_set_long_mode(ui->screen_2_label_9, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_9, &lv_font_MonaSans_Variable_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_12
    ui->screen_2_label_12 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_12, 235, 113);
    lv_obj_set_size(ui->screen_2_label_12, 56, 17);
    lv_label_set_text(ui->screen_2_label_12, "1246");
    lv_label_set_long_mode(ui->screen_2_label_12, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_12, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_12, &lv_font_MonaSans_Variable_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_13
    ui->screen_2_label_13 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_13, 125, 137);
    lv_obj_set_size(ui->screen_2_label_13, 47, 13);
    lv_label_set_text(ui->screen_2_label_13, "Voltage");
    lv_label_set_long_mode(ui->screen_2_label_13, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_13, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_13, &lv_font_MonaSans_Variable_9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_14
    ui->screen_2_label_14 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_14, 128, 113);
    lv_obj_set_size(ui->screen_2_label_14, 45, 17);
    lv_label_set_text(ui->screen_2_label_14, "14.1 v");
    lv_label_set_long_mode(ui->screen_2_label_14, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_14, &lv_font_MonaSans_Variable_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_img_12
    ui->screen_2_img_12 = lv_image_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_img_12, 92, 54);
    lv_obj_set_size(ui->screen_2_img_12, 27, 37);
    lv_obj_add_flag(ui->screen_2_img_12, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_2_img_12, &_BatteryT_RGB565A8_27x37);
    lv_image_set_pivot(ui->screen_2_img_12, 50,50);
    lv_image_set_rotation(ui->screen_2_img_12, 0);

    //Write style for screen_2_img_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_2_img_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_2_img_12, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_2_img_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_img_13
    ui->screen_2_img_13 = lv_image_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_img_13, 90, 107);
    lv_obj_set_size(ui->screen_2_img_13, 30, 43);
    lv_obj_add_flag(ui->screen_2_img_13, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_2_img_13, &_BatteryV_RGB565A8_30x43);
    lv_image_set_pivot(ui->screen_2_img_13, 50,50);
    lv_image_set_rotation(ui->screen_2_img_13, 0);

    //Write style for screen_2_img_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_2_img_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_2_img_13, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_2_img_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_17
    ui->screen_2_label_17 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_17, 11, 141);
    lv_obj_set_size(ui->screen_2_label_17, 68, 13);
    lv_label_set_text(ui->screen_2_label_17, "Capacity");
    lv_label_set_long_mode(ui->screen_2_label_17, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_17, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_17, &lv_font_MonaSans_Variable_9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_17, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_img_21
    ui->screen_2_img_21 = lv_image_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_img_21, 204, 52);
    lv_obj_set_size(ui->screen_2_img_21, 24, 35);
    lv_obj_add_flag(ui->screen_2_img_21, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_2_img_21, &_BatteryH_RGB565A8_24x35);
    lv_image_set_pivot(ui->screen_2_img_21, 50,50);
    lv_image_set_rotation(ui->screen_2_img_21, 0);

    //Write style for screen_2_img_21, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_2_img_21, 252, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_2_img_21, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_2_img_21, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_18
    ui->screen_2_label_18 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_18, 229, 78);
    lv_obj_set_size(ui->screen_2_label_18, 68, 13);
    lv_label_set_text(ui->screen_2_label_18, "Battery Health");
    lv_label_set_long_mode(ui->screen_2_label_18, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_18, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_18, &lv_font_MonaSans_Variable_9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_img_22
    ui->screen_2_img_22 = lv_image_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_img_22, 206, 108);
    lv_obj_set_size(ui->screen_2_img_22, 28, 43);
    lv_obj_add_flag(ui->screen_2_img_22, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_2_img_22, &_BatteryC_RGB565A8_28x43);
    lv_image_set_pivot(ui->screen_2_img_22, 50,50);
    lv_image_set_rotation(ui->screen_2_img_22, 0);

    //Write style for screen_2_img_22, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_2_img_22, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->screen_2_img_22, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_2_img_22, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_label_19
    ui->screen_2_label_19 = lv_label_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_label_19, 230, 136);
    lv_obj_set_size(ui->screen_2_label_19, 68, 13);
    lv_label_set_text(ui->screen_2_label_19, "Cycle Count");
    lv_label_set_long_mode(ui->screen_2_label_19, LV_LABEL_LONG_WRAP);

    //Write style for screen_2_label_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_label_19, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_label_19, &lv_font_MonaSans_Variable_9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_label_19, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_2_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_2_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_label_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_line_1
    ui->screen_2_line_1 = lv_line_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_line_1, 12, 37);
    lv_obj_set_size(ui->screen_2_line_1, 294, 3);
    static lv_point_precise_t screen_2_line_1[] = {{300, 0},{0, 0}};
    lv_line_set_points(ui->screen_2_line_1, screen_2_line_1, 2);

    //Write style for screen_2_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_2_line_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_2_line_1, lv_color_hex(0x757575), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_2_line_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_2_line_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_2.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_2);

    //Init events for screen.
    events_init_screen_2(ui);
}
