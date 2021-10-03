/*
 * lowpass_filter.h
 *
 *  Created on: Jul 23, 2021
 *      Author: Brian
 */

#ifndef INC_FIR_FILTER_H_
#define INC_FIR_FILTER_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FILTER_LENGHT 1000

typedef struct {
	float buf[FILTER_LENGHT];
	float FIR_IMPULSE_RESPONSE[FILTER_LENGHT];
	uint16_t bufIndex;
	float out;
} FIRFilter;
void FIRFilter_Init(FIRFilter *fir, float *coeff);
float FIRFilter_Update(FIRFilter *fir, float inp);

#endif /* INC_FIR_FILTER_H_ */
