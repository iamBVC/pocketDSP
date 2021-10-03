/*
 * comb_filter.h
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */

#ifndef INC_COMB_FILTER_H_
#define INC_COMB_FILTER_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct {
	float *bufx;
	float *bufy;
	uint16_t bufSize;
	uint16_t bufIndex;
	uint16_t Ndelay;
	uint16_t delayIndex;
	float gain;
	float out;
} COMBFilter;
void COMBFilter_Init(COMBFilter *cbf, uint16_t D, float G);
float COMBFilter_Update(COMBFilter *cbf, float inp);

#endif /* INC_COMB_FILTER_H_ */
