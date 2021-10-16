/*
 * oscilloscope.c
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */


#include "oscilloscope.h"

void start_oscilloscope()
{

    sys_status_refresh();
    lv_obj_align(sys_status, LV_ALIGN_TOP_LEFT, 0, 0);

    list = lv_list_create(app_scr);
    lv_obj_set_size(list, 120, 320);
    lv_obj_set_align(list, LV_ALIGN_TOP_RIGHT);

    lv_list_add_text(list, "Channels");
    btn = lv_list_add_btn(list, LV_SYMBOL_VOLUME_MAX, "R CH");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "config_r_ch");
    btn = lv_list_add_btn(list, LV_SYMBOL_VOLUME_MAX, "L CH");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "config_l_ch");

    lv_list_add_text(list, "Trigger");
    btn = lv_list_add_btn(list, NULL, "Thresh");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_trigger_threshold");
    btn = lv_list_add_btn(list, NULL, "Type");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_trigger_type");
    btn = lv_list_add_btn(list, NULL, "Mode");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_trigger_mode");

    lv_list_add_text(list, "Cursors");
    btn = lv_list_add_btn(list, NULL, "t1");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_t1");
    btn = lv_list_add_btn(list, NULL, "t2");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_t1");
    btn = lv_list_add_btn(list, NULL, "v1");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_v1");
    btn = lv_list_add_btn(list, NULL, "v2");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_v2");

    lv_list_add_text(list, "Signal Gen");
    btn = lv_list_add_btn(list, NULL, "R CH");
    btn = lv_list_add_btn(list, NULL, "L CH");

    lv_list_add_text(list, "System");
    btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, NULL);
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "osc_settings");
    btn = lv_list_add_btn(list, LV_SYMBOL_POWER, NULL);
    lv_obj_add_event_cb(btn, reset_app, LV_EVENT_CLICKED, NULL);

    chart = lv_chart_create(app_scr);
    lv_obj_set_size(chart, 360, 300);
    lv_obj_align(chart, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, OSC_BUF_SIZE);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -1000, 1000);
    lv_chart_set_div_line_count(chart, 9, 5);
    lv_chart_set_zoom_x(chart,256*1);
    lv_chart_set_point_count(chart, 256);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)oscilloscope_buffer_r);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)oscilloscope_buffer_l);
    lv_obj_set_style_bg_color(chart, lv_color_make(0, 0, 0), LV_PART_MAIN);

    sample_callback = &osc_sample_callback;

}

void osc_sample_callback(){

	static uint16_t index = 0;
	static uint32_t elapsed = 0;
	static uint32_t prev = 0;
	static uint8_t triggered = 0;

	prev = adc_output[0];
	HAL_I2S_Receive(&hi2s2, (uint16_t*)adc_output, 2, 0);
	adc_output[0] = adc_output[0] / 8388.608;
	adc_output[1] = adc_output[1] / 8388.608;

	if (elapsed >= SAMPLE_FREQ / 10 && adc_output[0] < 1000 && prev == 0 && adc_output[0] > prev && triggered == 0) triggered = 1;

	if (elapsed >= SAMPLE_FREQ / 10 && triggered == 1){

		if (adc_output[0] < 1000) oscilloscope_buffer_r[index] = adc_output[0]; else oscilloscope_buffer_r[index] = adc_output[0] - 2000;
		if (adc_output[1] < 1000) oscilloscope_buffer_l[index] = adc_output[1]; else oscilloscope_buffer_l[index] = adc_output[1] - 2000;

		if (index == OSC_BUF_SIZE - 1){
			triggered = 0;
			index = 0;
			elapsed = 0;
			lv_chart_refresh(chart);
		}else{
			index++;
		}
	}

	elapsed++;

}
