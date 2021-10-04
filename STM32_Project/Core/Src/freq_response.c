/*
 * freq_response.c
 *
 *  Created on: 4 ott 2021
 *      Author: Brian
 */


#include "freq_response.h"

static const uint32_t peak_value = 8080000;
static const uint32_t sample_period = 10;
static double temp,input,output,frequency,t,i = 0;

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
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 20000);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1000);
    lv_chart_set_div_line_count(chart, 11, 21);
    lv_chart_set_zoom_x(chart,256*1);
    lv_chart_set_point_count(chart, 20000);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)freq_response);
    lv_obj_set_style_bg_color(chart, lv_color_make(30, 30, 30), LV_PART_MAIN);
    lv_chart_refresh(chart);

    sample_callback = &freqresp_sample_callback;

}

void freqresp_sample_callback(){
	HAL_I2S_Receive(&hi2s2, (uint16_t*)adc_output, 2, 0);
	HAL_I2S_Transmit(&hi2s1, (uint16_t*)dac_input, 2, 0);

	output = peak_value * sin(2.0 * 3.1416 * t / 192000.0);
	if (output >= 0) dac_input[1] = output; else dac_input[1] = 0xFFFFFF + output + 1;
	t += frequency;
	if (t >= 192000) t -= 192000;


	if (adc_output[0] >= 0x800000) input = adc_output[0] - 0xFFFFFF - 1; else input = adc_output[0];
	input = input / 8388607.0;
	temp += pow(input, 2.0);
	if (i >= sample_period - 1){
		temp = sqrt(temp / (double)sample_period);
		freq_response[(uint16_t)frequency] = temp;
		i = 0;
		temp = 0;
		if (frequency >= 19999){
			frequency = 0;
			lv_chart_refresh(chart);
		}else{
			frequency++;
		}
	}else{
		i++;
	}


}
