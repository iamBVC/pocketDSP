/*
 * delay.h
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "comb_filter.h"

typedef struct {
	float DryWet;
	float out;
	COMBFilter cbf_dly;
} Delay;
void Delay_Init(Delay *dly, uint16_t D, float M, float F);
float Delay_Update(Delay *dly, float inp);

#endif /* INC_DELAY_H_ */
