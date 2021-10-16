/*
 * fft.c
 *
 *  Created on: 3 ott 2021
 *      Author: Brian
 */


#include "fft.h"

#define FFT_POINTS (1 << 9)
static lv_coord_t fft_graph[FFT_POINTS];
static double complex fft_vectors[FFT_POINTS];
static uint8_t stop_fft = 0;

void start_fft()
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
    lv_obj_add_event_cb(btn, exit_fft, LV_EVENT_CLICKED, NULL);

    chart = lv_chart_create(app_scr);
    lv_obj_set_size(chart, 360, 300);
    lv_obj_align(chart, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, FFT_POINTS/2);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1000);
    lv_chart_set_div_line_count(chart, 11, 11);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_point_count(chart, FFT_POINTS/2);
    lv_chart_set_zoom_x(chart, 256 * 1);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)fft_graph);
    lv_obj_set_style_bg_color(chart, lv_color_make(0, 0, 0), LV_PART_MAIN);

    timer = lv_timer_create(timer_fft, 100, NULL);
    lv_timer_set_repeat_count(timer, -1);
    lv_timer_ready(timer);

    sample_callback = &fft_sample_callback;

}
void exit_fft(){
	lv_timer_set_repeat_count(timer, 0);
	reset_app();
}

void timer_fft(lv_timer_t* timer){
	stop_fft = 1;
	FFT(fft_vectors, FFT_POINTS, (double)SAMPLE_FREQ);
	stop_fft = 0;
	for (uint32_t n = 0; n < FFT_POINTS/2; n++)
		fft_graph[n] = 50.0 * log(sqrt((creal(fft_vectors[n])*creal(fft_vectors[n])) + (cimag(fft_vectors[n])*cimag(fft_vectors[n]))));
	lv_chart_refresh(chart);
}

void fft_sample_callback(){

	static uint16_t index = 0;

	dac_input[0] = adc_output[0];
	dac_input[1] = adc_output[1];
	HAL_I2S_Receive(&hi2s2, (uint16_t*)adc_output, 2, 0);
	HAL_I2S_Transmit(&hi2s1, (uint16_t*)adc_output, 2, 0);


	if (stop_fft == 0) fft_vectors[index] = int24_to_float(adc_output[0]) + 0.0*I;
	if (index == FFT_POINTS - 1 || stop_fft == 1) index = 0; else index++;





}
