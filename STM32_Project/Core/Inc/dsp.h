/*
 * dsp.h
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */

#ifndef INC_DSP_H_
#define INC_DSP_H_

#include "system.h"

void start_dsp();
void dsp_menu_cb(lv_event_t* e);
void dsp_new_project();
void dsp_open_project();
void dsp_add_fx(lv_event_t* e);
void dsp_change_fx(lv_event_t* e);
void dsp_refresh_fx_slider(lv_event_t* e);
void dsp_refresh_fx_list(lv_event_t* e);
void dsp_refresh_fx_roller(lv_event_t* e);
void dsp_open_edit(lv_event_t* e);
void dsp_close_edit();
void dsp_filter_response_refresh(uint8_t type, uint8_t order, float frq1, float frq2);
void dsp_sidechain_response_refresh(uint8_t type, uint8_t width, uint8_t slope);
void dsp_fir_calc(uint8_t FILT_TYPE, uint8_t WIN_TYPE, uint16_t NUM_TOTAL_SAMPLES, uint16_t NUM_SHIFT_SAMPLES, double SAMPLE_TIME_S, double CUTOFF_FREQUENCY_HZ, double CUTOFF_FREQUENCY2_HZ);

#endif /* INC_DSP_H_ */
