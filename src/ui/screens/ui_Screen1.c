// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: squareline

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_time_label = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_time_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_time_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_time_label, 0);
    lv_obj_set_y(ui_time_label, -107);
    lv_obj_set_align(ui_time_label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_time_label, "12:25:45");
    lv_obj_set_style_text_color(ui_time_label, lv_color_hex(0x293062), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_time_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_time_label, &ui_font_clock_80, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_date_label = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_date_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_date_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_date_label, 0);
    lv_obj_set_y(ui_date_label, -59);
    lv_obj_set_align(ui_date_label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_date_label, "Mon 26 Dec 2023");
    lv_obj_set_style_text_color(ui_date_label, lv_color_hex(0x9C9CD9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_date_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_date_label, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_logo_button = lv_btn_create(ui_Screen1);
    lv_obj_set_width(ui_logo_button, 140);
    lv_obj_set_height(ui_logo_button, 140);
    lv_obj_set_x(ui_logo_button, 2);
    lv_obj_set_y(ui_logo_button, 31);
    lv_obj_set_align(ui_logo_button, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_logo_button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_logo_button, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_logo_button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_logo_button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_logo_button, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_logo_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_logo_image = lv_img_create(ui_logo_button);
    lv_img_set_src(ui_logo_image, &ui_img_sls_logo_png);
    lv_obj_set_width(ui_logo_image, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_logo_image, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_logo_image, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_logo_image, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_logo_image, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_logo_image, 280);

    ui_count_button = lv_btn_create(ui_Screen1);
    lv_obj_set_width(ui_count_button, 191);
    lv_obj_set_height(ui_count_button, 39);
    lv_obj_set_x(ui_count_button, 5);
    lv_obj_set_y(ui_count_button, 129);
    lv_obj_set_align(ui_count_button, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_count_button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_count_button, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_count_button, lv_color_hex(0x293062), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_count_button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_count_label = lv_label_create(ui_count_button);
    lv_obj_set_width(ui_count_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_count_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_count_label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_count_label, "Button Pressed: 0");

}
