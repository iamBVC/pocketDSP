/*
 * system.c
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */
#include "system.h"


const char *app_list[] = { "DSP", "Synthesizer", "Oscilloscope", "FFT", "", "" };
uint8_t battery_charge = 100;
uint8_t is_active_usb = 1;
uint8_t is_active_sd = 1;
uint8_t dsp_fx_count = 0;



void reset_app()
{
    dsp_fx_count = 0;
    for (uint8_t i = 0; i < DSP_MAX_FX_COUNT; i++)
        for (uint8_t j = 0; j <= DSP_MAX_FX_SETTINGS; j++)
            dsp_fx_settings[i][j] = 0;

    reset_scr();
    start_main();
}

void reset_scr()
{
    if (lv_obj_is_valid(app_scr)) lv_obj_del_async(app_scr);
    app_scr = lv_obj_create(NULL);
    lv_scr_load(app_scr);
    //lv_scr_load_anim(app_scr, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, 1);
}


void start_main()
{
    sys_status_refresh();
    lv_obj_align(sys_status, LV_ALIGN_TOP_LEFT, 0, 0);
    cont = lv_obj_create(app_scr);
    lv_obj_set_size(cont, 480, 250);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    for (uint8_t i = 0; i < 6; i++) {
        btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 140, 50);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_add_event_cb(btn, start_app, LV_EVENT_CLICKED, app_list[i]);
        label = lv_label_create(btn);
        lv_label_set_text(label, app_list[i]);
        lv_obj_center(label);
    }
    label = lv_label_create(app_scr);
    lv_label_set_text(label, "pocketDSP by BVC");
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_set_style_bg_color(cont, lv_color_make(200, 200, 200), LV_PART_MAIN);
}

void start_app(lv_event_t* e)
{
    char* id = lv_event_get_user_data(e);
    reset_scr();
    if (id == app_list[0]) start_dsp();
    if (id == app_list[1]) start_synthesizer();
    if (id == app_list[2]) start_oscilloscope();
    if (id == app_list[3]) start_fft();
}

void sys_status_refresh()
{
	extern ADC_HandleTypeDef hadc1;
	ADC_ChannelConfTypeDef sConfig = { 0 };
	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	HAL_ADC_Start(&hadc1);
	while (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) != HAL_OK);
	battery_charge = HAL_ADC_GetValue(&hadc1) / 40.95;


    if(lv_obj_is_valid(sys_status)) lv_obj_del_async(sys_status);
    sys_status = lv_obj_create(app_scr);
    lv_obj_set_size(sys_status, 20, 120);
    lv_obj_remove_style_all(sys_status);

    label = lv_label_create(sys_status);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 20, 2);
    if (battery_charge >= 0 && battery_charge < 20) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_EMPTY " %d%%", battery_charge);
    else if (battery_charge >= 20 && battery_charge < 40) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_1 " %d%%", battery_charge);
    else if (battery_charge >= 40 && battery_charge < 60) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_2 " %d%%", battery_charge);
    else if (battery_charge >= 60 && battery_charge < 80) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_3 " %d%%", battery_charge);
    else if (battery_charge >= 80 && battery_charge <= 100) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_FULL " %d%%", battery_charge);

    label = lv_label_create(sys_status);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 80, 2);
    if (is_active_sd) lv_label_set_text(label, LV_SYMBOL_SD_CARD);

    label = lv_label_create(sys_status);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 100, 2);
    if (is_active_usb) lv_label_set_text(label, LV_SYMBOL_USB);
}
