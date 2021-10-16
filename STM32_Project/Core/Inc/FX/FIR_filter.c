/*
 * FIR_filter.c
 *
 *  Created on: Jul 23, 2021
 *      Author: Brian
 */


#include "FIR_filter.h"

void FIRFilter_Init(FIRFilter *fir){
	fir->bufIndex = 0;
	fir->out = 0.0f;
	fir->filterLenght = FIR_FILTER_LENGHT;
	for (uint16_t n = 0; n < fir->filterLenght; n++) fir->buf[n] = 0.0f;
}

float FIRFilter_Update(FIRFilter *fir, float inp){
	fir->buf[fir->bufIndex] = inp;
	fir->out = 0.0f;
	uint16_t sumIndex = fir->bufIndex;

	for (uint16_t n = 0; n < fir->filterLenght; n++){
		if (sumIndex > 0) sumIndex--; else sumIndex = fir->filterLenght - 1;
		fir->out += fir->FIR_IMPULSE_RESPONSE[n] * fir->buf[sumIndex];
	}

	fir->bufIndex++;
	if (fir->bufIndex >= fir->filterLenght) fir->bufIndex = 0;

	return fir->out;
}


void FIRFilter_CalcCoeff(FIRFilter *fir, uint8_t FILT_TYPE, uint8_t WIN_TYPE, uint16_t NUM_TOTAL_SAMPLES, uint16_t NUM_SHIFT_SAMPLES, float SAMPLE_TIME_S, float CUTOFF_FREQUENCY_HZ, float CUTOFF_FREQUENCY2_HZ){

	float window[NUM_TOTAL_SAMPLES];
	float frequencyVectorHz[NUM_TOTAL_SAMPLES];
	float df = 20000.0 / (float)(NUM_TOTAL_SAMPLES - 1);

    for (uint16_t n = 0; n < NUM_TOTAL_SAMPLES; n++){

		if (n != NUM_SHIFT_SAMPLES){
            if (FILT_TYPE == 0) fir->FIR_IMPULSE_RESPONSE[n] = sin(2.0 * M_PI * CUTOFF_FREQUENCY2_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES)) / (M_PI * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES));
            if (FILT_TYPE == 1) fir->FIR_IMPULSE_RESPONSE[n] = (sin(M_PI * (n - NUM_SHIFT_SAMPLES)) - sin(2.0 * M_PI * CUTOFF_FREQUENCY_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES))) / (M_PI * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES));
            if (FILT_TYPE == 2) fir->FIR_IMPULSE_RESPONSE[n] = (sin(2.0 * M_PI * CUTOFF_FREQUENCY2_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES)) - sin(2.0 * M_PI * CUTOFF_FREQUENCY_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES))) / (M_PI * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES));
            if (FILT_TYPE == 3) fir->FIR_IMPULSE_RESPONSE[n] = (sin(2.0 * M_PI * CUTOFF_FREQUENCY_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES)) - sin(2.0 * M_PI * CUTOFF_FREQUENCY2_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES)) + sin(M_PI * (n - NUM_SHIFT_SAMPLES))) / (M_PI * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES));
        }else{
            if (FILT_TYPE == 0) fir->FIR_IMPULSE_RESPONSE[n] = 2.0 * CUTOFF_FREQUENCY2_HZ;
            if (FILT_TYPE == 1) fir->FIR_IMPULSE_RESPONSE[n] = 1.0 / SAMPLE_TIME_S - 2.0 * CUTOFF_FREQUENCY_HZ;
            if (FILT_TYPE == 2) fir->FIR_IMPULSE_RESPONSE[n] = 2.0 * CUTOFF_FREQUENCY2_HZ - 2.0 * CUTOFF_FREQUENCY_HZ;
            if (FILT_TYPE == 3) fir->FIR_IMPULSE_RESPONSE[n] = 2.0 * CUTOFF_FREQUENCY_HZ - 2.0 * CUTOFF_FREQUENCY2_HZ + 1.0 / SAMPLE_TIME_S;
        }

		fir->FIR_IMPULSE_RESPONSE[n] *= SAMPLE_TIME_S;

		if(WIN_TYPE <= 0 || WIN_TYPE >= 11) window[n] = 1.0;
		if(WIN_TYPE == 1) window[n] = 1.0 - abs((n - 0.5 * NUM_TOTAL_SAMPLES) / (0.5 * NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 2) window[n] = 1.0 - pow((n - 0.5 * NUM_TOTAL_SAMPLES) / (0.5 * NUM_TOTAL_SAMPLES), 2.0);
		if(WIN_TYPE == 3) window[n] = sin(M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 4) window[n] = 0.5 * (1 - cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)));
		if(WIN_TYPE == 5) window[n] = (25.0 / 46.0) - (21.0 / 46.0) * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 6) window[n] = 0.42 - 0.5 * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.08 * cos(4.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 7) window[n] = 0.355768 - 0.487396 * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.144232 * cos(4.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) - 0.012604 * cos(6.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 8) window[n] = 0.3635819 - 0.4891775 * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.1365995 * cos(4.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) - 0.0106411 * cos(6.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 9) window[n] = 0.35875 - 0.48829 * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.14128 * cos(4.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) - 0.01168 * cos(6.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 10) window[n] = 0.21557895 - 0.41663158 * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.277263158 * cos(4.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) - 0.083578947 * cos(6.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.006947368 * cos(8.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));

		fir->FIR_IMPULSE_RESPONSE[n] *= window[n];
		frequencyVectorHz[n] = n * df;
    }


    for (uint16_t fIndex = 0; fIndex < NUM_TOTAL_SAMPLES; fIndex++){
    	float reWin = 0.0;
    	float imWin = 0.0;
        for (uint16_t n = 0; n < NUM_TOTAL_SAMPLES; n++){
            reWin = reWin + fir->FIR_IMPULSE_RESPONSE[n] * cos(2.0 * M_PI * frequencyVectorHz[fIndex] * n * SAMPLE_TIME_S);
            imWin = imWin - fir->FIR_IMPULSE_RESPONSE[n] * sin(2.0 * M_PI * frequencyVectorHz[fIndex] * n * SAMPLE_TIME_S);
        }
        fir->FIR_FREQUENCY_RESPONSE[fIndex] = 100.0 * sqrt(reWin * reWin + imWin * imWin);
    }

}
