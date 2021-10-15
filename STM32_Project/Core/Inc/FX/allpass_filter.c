/*
 * allpass_filter.c
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */


#include "allpass_filter.h"

void ALLPASSFilter_Init(ALLPASSFilter *apf, uint16_t D, float G){
	apf->bufIndex = 0;
	apf->Ndelay = D;
	apf->gain = G;
	apf->out = 0.0f;
	apf->delayIndex = 0;
	apf->bufSize = D+1;
	if (sizeof(apf->bufx) > 4) free(apf->bufx);
	if (sizeof(apf->bufy) > 4) free(apf->bufy);
	apf->bufx = (float*)calloc(apf->bufSize, sizeof(float));
	apf->bufy = (float*)calloc(apf->bufSize, sizeof(float));

}

float ALLPASSFilter_Update(ALLPASSFilter *apf, float inp){
	apf->bufx[apf->bufIndex] = inp;

	if(apf->bufIndex >= apf->Ndelay){
		apf->delayIndex = apf->bufIndex - apf->Ndelay;
	}else{
		apf->delayIndex = apf->bufIndex + apf->bufSize - apf->Ndelay;
	}

	apf->out = apf->bufx[apf->delayIndex] - (apf->gain * apf->bufx[apf->bufIndex]) + (apf->gain * apf->bufy[apf->delayIndex]);
	apf->bufy[apf->bufIndex] = apf->out;

	apf->bufIndex++;
	if (apf->bufIndex == apf->bufSize){
		apf->bufIndex = 0;
	}

	return apf->out;
}
