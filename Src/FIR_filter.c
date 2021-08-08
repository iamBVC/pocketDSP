/*
 * FIR_filter.c
 *
 *  Created on: Jul 23, 2021
 *      Author: Brian
 */


#include "FIR_filter.h"

void FIRFilter_Init(FIRFilter *fir, float *coeff){

	fir->bufIndex = 0;
	fir->out = 0.0f;

	for (uint16_t n = 0; n < FILTER_LENGHT; n++){
		fir->buf[n] = 0.0f;
		fir->FIR_IMPULSE_RESPONSE[n] = coeff[n];
	}

}

float FIRFilter_Update(FIRFilter *fir, float inp){
	fir->buf[fir->bufIndex] = inp;

	fir->out = 0.0f;
	uint16_t sumIndex = fir->bufIndex;

	for (uint16_t n = 0; n < FILTER_LENGHT; n++){
		if (sumIndex > 0) sumIndex--; else sumIndex = FILTER_LENGHT - 1;
		fir->out += fir->FIR_IMPULSE_RESPONSE[n] * fir->buf[sumIndex];
	}

	fir->bufIndex++;
	if (fir->bufIndex == FILTER_LENGHT){
		fir->bufIndex = 0;
	}

	return fir->out;
}
