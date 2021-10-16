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
#include <math.h>

#define FIR_FILTER_LENGHT 256

typedef struct {
	float buf[FIR_FILTER_LENGHT];
	float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGHT];
	float FIR_FREQUENCY_RESPONSE[FIR_FILTER_LENGHT];
	uint16_t bufIndex;
	uint16_t filterLenght;
	float out;
} FIRFilter;
void FIRFilter_Init(FIRFilter *fir);
float FIRFilter_Update(FIRFilter *fir, float inp);
void FIRFilter_CalcCoeff(FIRFilter *fir, uint8_t FILT_TYPE, uint8_t WIN_TYPE, uint16_t NUM_TOTAL_SAMPLES, uint16_t NUM_SHIFT_SAMPLES, float SAMPLE_TIME_S, float CUTOFF_FREQUENCY_HZ, float CUTOFF_FREQUENCY2_HZ);

#endif /* INC_FIR_FILTER_H_ */
