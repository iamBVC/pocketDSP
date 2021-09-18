/*
 * system.h
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include <stdio.h>
#include <math.h>
#include "stm32h7xx_hal.h"
#include "lvgl/lvgl.h"
#include "adc.h"
#include "dac.h"
#include "gui.h"
#include "dsp.h"
#include "oscilloscope.h"


#define FFT_POINTS 256
lv_coord_t fft_vectors[FFT_POINTS];
lv_coord_t oscilloscope_buffer_r[FFT_POINTS], oscilloscope_buffer_l[FFT_POINTS];

lv_obj_t *label, *btn, *cont, *menu, *list, *slider, *mbox, *chart, *roller, *app_scr;
lv_obj_t* sys_status;
lv_chart_series_t* ser;
uint8_t battery_charge;
uint8_t is_active_usb;
uint8_t is_active_sd;
extern const char *app_list[];

#define DSP_MAX_FX_COUNT 16
#define DSP_MAX_FX_SETTINGS 10
lv_obj_t * dsp_fx_scr, * dsp_main_cont;
lv_coord_t dsp_filter_response[200];
lv_coord_t dsp_sidechain_response[100];
uint8_t dsp_fx_count;
uint16_t dsp_fx_settings[DSP_MAX_FX_COUNT][DSP_MAX_FX_SETTINGS + 1];

double *impulseResponse;
double *window;
double *windowedImpulseResponse;
double *frequencyVectorHz;
double *winRespMag;
lv_coord_t *dsp_fir_response;



void reset_scr();
void reset_app();
void start_main();
void start_app(lv_event_t* e);
void sys_status_refresh();

#endif /* INC_SYSTEM_H_ */
