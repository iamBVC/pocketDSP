/*
 * phaser.c
 *
 *  Created on: 23 lug 2021
 *      Author: Brian
 */



#include "delay.h"



void Delay_Init(Delay *dly, uint16_t D, float M, float F){
	COMBFilter_Init(&dly->cbf_dly, D, F);
	dly->DryWet = M;
	dly->out = 0.0f;
}
float Delay_Update(Delay *dly, float inp){
	COMBFilter_Update(&dly->cbf_dly, inp);
	dly->out = (dly->cbf_dly.out * dly->DryWet) + (inp * (1.0 - dly->DryWet));
	return dly->out;
}
