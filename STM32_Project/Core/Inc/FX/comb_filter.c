/*
 * comb_filter.c
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */


#include "comb_filter.h"

void COMBFilter_Init(COMBFilter *cbf, uint16_t D, float G){
	cbf->bufIndex = 0;
	cbf->Ndelay = D;
	cbf->gain = G;
	cbf->out = 0.0f;
	cbf->delayIndex = 0;
	cbf->bufSize = D+1;
	cbf->bufx = calloc(cbf->bufSize, sizeof(float));
	cbf->bufy = calloc(cbf->bufSize, sizeof(float));

}

float COMBFilter_Update(COMBFilter *cbf, float inp){
	cbf->bufx[cbf->bufIndex] = inp;

	if(cbf->bufIndex >= cbf->Ndelay){
		cbf->delayIndex = cbf->bufIndex - cbf->Ndelay;
	}else{
		cbf->delayIndex = cbf->bufIndex + cbf->bufSize - cbf->Ndelay;
	}

	cbf->out = cbf->bufx[cbf->bufIndex] + (cbf->gain * cbf->bufy[cbf->delayIndex]);
	cbf->bufy[cbf->bufIndex] = cbf->out;

	cbf->bufIndex++;
	if (cbf->bufIndex == cbf->bufSize){
		cbf->bufIndex = 0;
	}

	return cbf->out;
}
