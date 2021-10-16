/*
 * dsp.h
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */

#ifndef INC_DSP_H_
#define INC_DSP_H_

#include "system.h"

static const char* fx_list[] = { "Distortion", "Phaser", "Chorus", "FIR Filter", "Delay", "Reeverb", "Compressor", "Sidechain"};
static const char *dsp_main_menu[] = { "New project", "Open project", "Save project", "Settings", "Exit" };

#define DSP_MAX_FX_COUNT 8
#define DSP_MAX_FX_SETTINGS 10
static lv_obj_t * dsp_fx_scr, * dsp_main_cont;
static lv_coord_t dsp_sidechain_response[100];
static lv_coord_t dsp_filter_response[FIR_FILTER_LENGHT];
static uint16_t dsp_fx_settings[DSP_MAX_FX_COUNT][DSP_MAX_FX_SETTINGS + 1];

static void dsp_fx_setup(uint8_t id);
void dsp_fx_init();
void DSP_sample_callback();
static float dsp_fx_output[DSP_MAX_FX_COUNT];
static float dsp_fx_sample(uint8_t id, float input);

void start_dsp();
static void reset_dsp();
static void dsp_menu_cb(lv_event_t* e);
static void dsp_new_project();
static void dsp_open_project();
static void dsp_refresh_fx_slider(lv_event_t* e);
static void dsp_refresh_fx_list(lv_event_t* e);
static void dsp_refresh_fx_roller(lv_event_t* e);
static void dsp_open_edit(lv_event_t* e);
static void dsp_close_edit();
static void dsp_sidechain_response_refresh(uint8_t type, uint8_t width, uint8_t slope);

#endif /* INC_DSP_H_ */
