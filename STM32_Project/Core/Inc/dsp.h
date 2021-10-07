/*
 * dsp.h
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */

#ifndef INC_DSP_H_
#define INC_DSP_H_

#include "system.h"

static const char* fx_list[] = { "None", "IIR Filter", "Reeverb", "Delay", "Sidechain", "Distortion", "FIR Filter" };
static const char *dsp_main_menu[] = { "New project", "Open project", "Save project", "Settings", "Exit" };

#define DSP_MAX_FX_COUNT 10
#define DSP_MAX_FX_SETTINGS 8
static lv_obj_t * dsp_fx_scr, * dsp_main_cont;
static lv_coord_t dsp_filter_response[200];
static lv_coord_t dsp_sidechain_response[100];
static uint16_t dsp_fx_settings[DSP_MAX_FX_COUNT][DSP_MAX_FX_SETTINGS + 1];

static double *impulseResponse;
static double *window;
static double *windowedImpulseResponse;
static double *frequencyVectorHz;
static double *winRespMag;
static lv_coord_t *dsp_fir_response;

void start_dsp();
static void reset_dsp();
static void dsp_menu_cb(lv_event_t* e);
static void dsp_new_project();
static void dsp_open_project();
static void dsp_change_fx(lv_event_t* e);
static void dsp_refresh_fx_slider(lv_event_t* e);
static void dsp_refresh_fx_list(lv_event_t* e);
static void dsp_refresh_fx_roller(lv_event_t* e);
static void dsp_open_edit(lv_event_t* e);
static void dsp_close_edit();
static void dsp_filter_response_refresh(uint8_t type, uint8_t order, float frq1, float frq2);
static void dsp_sidechain_response_refresh(uint8_t type, uint8_t width, uint8_t slope);
static void dsp_fir_calc(uint8_t FILT_TYPE, uint8_t WIN_TYPE, uint16_t NUM_TOTAL_SAMPLES, uint16_t NUM_SHIFT_SAMPLES, double SAMPLE_TIME_S, double CUTOFF_FREQUENCY_HZ, double CUTOFF_FREQUENCY2_HZ);

#endif /* INC_DSP_H_ */
