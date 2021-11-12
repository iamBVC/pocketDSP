/*
 * wah.c
 *
 *  Created on: Jul 23, 2021
 *      Author: Brian
 */


#include "wah.h"

void Wah_Init(WahFilter *wah){
	wah->bufIndex = 0;
	wah->RangeMin = 0;
	wah->RangeMax = 0;
	wah->Bandwidth = 0;
	wah->MaxAmplitude = 0.0f;
	wah->out = 0.0f;
	wah->filterLenght = WAH_FILTER_LENGHT;
	for (uint16_t n = 0; n < wah->filterLenght; n++) wah->buf[n] = 0.0f;
}

float Wah_Update(WahFilter *wah, float inp){
	wah->buf[wah->bufIndex] = inp;
	wah->out = 0.0f;
	uint16_t sumIndex = wah->bufIndex;

	for (uint16_t n = 0; n < wah->filterLenght; n++){
		if (sumIndex > 0) sumIndex--; else sumIndex = wah->filterLenght - 1;
		wah->out += wah->WAH_IMPULSE_RESPONSE[n] * wah->buf[sumIndex];
	}

	wah->bufIndex++;
	if (wah->bufIndex >= wah->filterLenght) wah->bufIndex = 0;

	return wah->out;
}


void Wah_CalcCoeff(WahFilter *wah, float SAMPLE_TIME_S){

	float window[NUM_TOTAL_SAMPLES];

    for (uint16_t n = 0; n < NUM_TOTAL_SAMPLES; n++){

		if (n != NUM_SHIFT_SAMPLES)
           wah->WAH_IMPULSE_RESPONSE[n] = (sin(2.0 * M_PI * CUTOFF_FREQUENCY2_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES)) - sin(2.0 * M_PI * CUTOFF_FREQUENCY_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES))) / (M_PI * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES));
       else
           wah->WAH_IMPULSE_RESPONSE[n] = 2.0 * CUTOFF_FREQUENCY2_HZ - 2.0 * CUTOFF_FREQUENCY_HZ;
        
		wah->WAH_IMPULSE_RESPONSE[n] *= SAMPLE_TIME_S;
		window[n] = (25.0 / 46.0) - (21.0 / 46.0) * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		wah->WAH_IMPULSE_RESPONSE[n] *= window[n];
    }

}
