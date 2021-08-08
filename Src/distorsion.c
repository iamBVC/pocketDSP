/*
 * distorsion.c
 *
 *  Created on: 26 lug 2021
 *      Author: Brian
 */


#include "distorsion.h"


void Distorsion_Init(Distorsion *dist, float T, float R){
	dist->Threshold = T;
	dist->Ratio = R;
	dist->out = 0.0f;
}
float Distorsion_Update(Distorsion *dist, float inp){
	if(inp > dist->Threshold){
		dist->out = dist->Threshold + (inp - dist->Threshold) / dist->Ratio;
	}else{
		dist->out = inp;
	}
	return dist->out;
}
