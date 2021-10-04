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
#include <complex.h>
#include "stm32h7xx_hal.h"
#include "lvgl/lvgl.h"
#include "adc.h"
#include "dac.h"
#include "dsp.h"
#include "oscilloscope.h"
#include "signal_generator.h"

#include "FX/allpass_filter.h"
#include "FX/comb_filter.h"
#include "FX/reverb.h"
#include "FX/FIR_filter.h"
#include "FX/phaser.h"
#include "FX/delay.h"
#include "FX/distorsion.h"

lv_obj_t *label, *btn, *cont, *menu, *list, *slider, *mbox, *chart, *roller, *app_scr;
lv_timer_t *timer;
lv_obj_t* sys_status;
lv_chart_series_t* ser;
float battery_voltage;
uint8_t is_active_usb;
uint8_t is_active_sd;
extern const char *app_list[];


volatile uint32_t dac_input[2];
volatile uint32_t adc_output[2];

void empty_void();
void direct_pass();
void (*sample_callback)(void);

void reset_scr();
void reset_app();
void start_main();
void start_app(lv_event_t* e);
void sys_status_refresh();

void Serial_printstr(char* text);
void Serial_printlnstr(char* text);
void Serial_printint(int32_t value);
void Serial_printlnint(int32_t value);

#endif /* INC_SYSTEM_H_ */
