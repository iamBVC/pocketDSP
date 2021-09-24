/*
 * signal_generator.h
 *
 *  Created on: Sep 24, 2021
 *      Author: Brian
 */

#ifndef INC_SIGNAL_GENERATOR_H_
#define INC_SIGNAL_GENERATOR_H_

#include "system.h"

static const char *waveform_list[] = { "None", "Sine", "Saw", "Triangle", "Square", "Noise" };

lv_obj_t *label_frq1, *label_phs1, *label_amp1, *roller_wave1, *slider_frq1, *slider_phs1, *slider_amp1;
lv_obj_t *label_frq2, *label_phs2, *label_amp2, *roller_wave2, *slider_frq2, *slider_phs2, *slider_amp2;

uint8_t waveform[2];
uint16_t frequency[2];
uint16_t amplitude[2];
uint16_t phase[2];
float output[2];


void start_signgen();
void event_cb(lv_event_t* e);

#endif /* INC_SIGNAL_GENERATOR_H_ */
