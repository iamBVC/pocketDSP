/*
 * freq_response.c
 *
 *  Created on: 4 ott 2021
 *      Author: Brian
 */


#include "freq_response.h"

static lv_coord_t freq_response[500];
static uint32_t sample_period = 0;
static double temp,input,output,frequency,t,i,n = 0;

void start_freqresp()
{
    sys_status_refresh();
    lv_obj_align(sys_status, LV_ALIGN_TOP_LEFT, 0, 0);

    list = lv_list_create(app_scr);
    lv_obj_set_size(list, 120, 320);
    lv_obj_set_align(list, LV_ALIGN_TOP_RIGHT);

    lv_list_add_text(list, "System");
    btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, NULL);
    btn = lv_list_add_btn(list, LV_SYMBOL_POWER, NULL);
    lv_obj_add_event_cb(btn, reset_app, LV_EVENT_CLICKED, NULL);

    chart = lv_chart_create(app_scr);
    lv_obj_set_size(chart, 360, 300);
    lv_obj_align(chart, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 500);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1200);
    lv_chart_set_div_line_count(chart, 13, 6);
    lv_chart_set_zoom_x(chart,256*1);
    lv_chart_set_point_count(chart, 500);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_YELLOW), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)freq_response);
    lv_obj_set_style_bg_color(chart, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_CIRCULAR);

    sample_callback = &freqresp_sample_callback;

}

void freqresp_sample_callback(){
	HAL_I2S_Receive(&hi2s2, (uint16_t*)adc_output, 2, 0);
	HAL_I2S_Transmit(&hi2s1, (uint16_t*)dac_input, 2, 0);

	output = 0.01 * sin(2.0 * 3.1416 * t / (float)SAMPLE_FREQ);
	dac_input[1] = float_to_int24(output);
	input = int24_to_float(adc_output[0]);
	temp += pow(input, 2.0);
	frequency = pow(10, n/100);
	if (frequency > SAMPLE_FREQ/2) t = 0; else t += frequency;
	if (t >= SAMPLE_FREQ) t -= SAMPLE_FREQ;
	sample_period = 5.0 * SAMPLE_FREQ / frequency;
	if (sample_period > SAMPLE_FREQ / 5.0) sample_period = SAMPLE_FREQ / 5.0;
	if (sample_period < SAMPLE_FREQ / 50.0) sample_period = SAMPLE_FREQ / 50.0;

	if (i >= sample_period - 1){
		temp = 117000.0 * sqrt(temp / (double)sample_period);
		lv_chart_set_next_value(chart, ser, temp);
		i = 0;
		temp = 0;
		if (n + 1 >= 500){
			n = 0;
		}else{
			n++;
		}
	}else{
		i++;
	}


}
