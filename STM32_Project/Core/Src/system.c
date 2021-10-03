/*
 * system.c
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */
#include "system.h"


const char *app_list[] = { "DSP", "Synthesizer", "Oscilloscope", "FFT", "Signal Gen.", "" };
uint8_t battery_charge = 0;
uint8_t is_active_usb = 1;
uint8_t is_active_sd = 1;
uint8_t dsp_fx_count = 0;

void empty_void(){
	HAL_I2S_Receive(&hi2s2, (uint16_t*)adc_output, 2, 0);
	HAL_I2S_Transmit(&hi2s1, (uint16_t*)dac_input, 2, 0);
}
void direct_pass(){
	dac_input[0] = adc_output[0];
	dac_input[1] = adc_output[1];
	HAL_I2S_Receive(&hi2s2, (uint16_t*)adc_output, 2, 0);
	HAL_I2S_Transmit(&hi2s1, (uint16_t*)dac_input, 2, 0);
}

void Serial_printstr(char* text){
	char buf[strlen(text)+1];
	sprintf(buf, "%s", text);
	CDC_Transmit_FS(buf, strlen(buf));
}

void Serial_printint(int32_t value){
	char buf[16];
	sprintf(buf,"%d", value);
	CDC_Transmit_FS(buf, strlen(buf));
}

void Serial_printlnstr(char* text){
	char buf[strlen(text)+1];
	sprintf(buf, "%s\r\n", text);
	CDC_Transmit_FS(buf, strlen(buf));
}

void Serial_printlnint(int32_t value){
	char buf[16];
	sprintf(buf,"%d\r\n", value);
	CDC_Transmit_FS(buf, strlen(buf));
}

void reset_app()
{
	sample_callback = &empty_void;
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
    if (id == app_list[4]) start_signgen();
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
	battery_voltage = HAL_ADC_GetValue(&hadc1) * 3.3 * 1.407 / 4095.0;


    if(lv_obj_is_valid(sys_status)) lv_obj_del_async(sys_status);
    sys_status = lv_obj_create(app_scr);
    lv_obj_set_size(sys_status, 20, 120);
    lv_obj_remove_style_all(sys_status);

    label = lv_label_create(sys_status);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 2);
    if (battery_voltage < 3.0) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_EMPTY " %.2fV", battery_voltage);
    else if (battery_voltage >= 3.0 && battery_voltage < 3.2) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_1 " %.2fV", battery_voltage);
    else if (battery_voltage >= 3.2 && battery_voltage < 3.4) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_2 " %.2fV", battery_voltage);
    else if (battery_voltage >= 3.4 && battery_voltage < 3.6) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_3 " %.2fV", battery_voltage);
    else if (battery_voltage >= 3.6) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_FULL " %.2fV", battery_voltage);

    label = lv_label_create(sys_status);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 80, 2);
    if (is_active_sd) lv_label_set_text(label, LV_SYMBOL_SD_CARD);

    label = lv_label_create(sys_status);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 100, 2);
    if (is_active_usb) lv_label_set_text(label, LV_SYMBOL_USB);
}
