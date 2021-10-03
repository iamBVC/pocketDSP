/*
 * oscilloscope.h
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */

#ifndef INC_OSCILLOSCOPE_H_
#define INC_OSCILLOSCOPE_H_

#include "system.h"

#define OSC_BUF_SIZE 1000
lv_coord_t oscilloscope_buffer_r[OSC_BUF_SIZE], oscilloscope_buffer_l[OSC_BUF_SIZE];

void start_oscilloscope();
void osc_sample_callback();

#endif /* INC_OSCILLOSCOPE_H_ */
