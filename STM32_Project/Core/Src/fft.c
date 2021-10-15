/*
 * fft.c
 *
 *  Created on: 3 ott 2021
 *      Author: Brian
 */


#include "fft.h"

void start_fft()
{
    for (uint16_t i = 0; i < FFT_POINTS; i++)
    fft_vectors[i] = 500 + 400.0 * sin(i / 200.0);

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

    lv_list_add_text(list, "Cursors");
    btn = lv_list_add_btn(list, NULL, "t1");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_t1");
    btn = lv_list_add_btn(list, NULL, "t2");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_t1");
    btn = lv_list_add_btn(list, NULL, "v1");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_v1");
    btn = lv_list_add_btn(list, NULL, "v2");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_v2");

    lv_list_add_text(list, "System");
    btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, NULL);
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "osc_settings");
    btn = lv_list_add_btn(list, LV_SYMBOL_POWER, NULL);
    lv_obj_add_event_cb(btn, reset_app, LV_EVENT_CLICKED, NULL);

    chart = lv_chart_create(app_scr);
    lv_obj_set_size(chart, 360, 300);
    lv_obj_align(chart, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, FFT_POINTS/2);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1000);
    lv_chart_set_div_line_count(chart, 11, 11);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_point_count(chart, FFT_POINTS/2);
    lv_chart_set_zoom_x(chart, 256 * 4);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)fft_graph);
    lv_obj_set_style_bg_color(chart, lv_color_make(30, 30, 30), LV_PART_MAIN);

    sample_callback = &fft_sample_callback;

}

void fft_sample_callback(){

	static uint16_t index = 0;
	static uint32_t elapsed = 0;

	dac_input[0] = adc_output[0];
	dac_input[1] = adc_output[1];
	HAL_I2S_Receive(&hi2s2, (uint16_t*)adc_output, 2, 0);
	HAL_I2S_Transmit(&hi2s1, (uint16_t*)adc_output, 2, 0);

	if (elapsed >= SAMPLE_FREQ / 5.0){
		if (adc_output[0] < 0x800000) timevalues[index] = adc_output[0]; else timevalues[index] = adc_output[0] - (float)(0xFFFFFF) - 1.0;
		fft_vectors[index] = timevalues[index] / 0.8388608 + 0.0*I;


		if (index == FFT_POINTS - 1){
			index = 0;
			elapsed = 0;
			FFT(fft_vectors, FFT_POINTS, 0.02 / (float)SAMPLE_FREQ);
			for (uint32_t n = 0; n < FFT_POINTS/2; n++) fft_graph[n] = 333.3 * log(sqrt((creal(fft_vectors[n])*creal(fft_vectors[n])) + (cimag(fft_vectors[n])*cimag(fft_vectors[n])))+1);
			lv_chart_refresh(chart);
		}else{
			index++;
		}
	}

	elapsed++;

}
