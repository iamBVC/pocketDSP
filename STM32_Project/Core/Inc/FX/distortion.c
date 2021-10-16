/*
 * distorsion.c
 *
 *  Created on: 26 lug 2021
 *      Author: Brian
 */


#include "distortion.h"


void Distortion_Init(Distortion *dist, float inputgain, float threshold, float outputgain){
	dist->InputGain = inputgain;
	dist->OutputGain = outputgain;
	dist->Threshold = threshold;
	dist->out = 0.0f;
}
float Distortion_Update(Distortion *dist, float inp){
	inp *= dist->InputGain;
	if(inp > dist->Threshold){
		dist->out = dist->Threshold;
	}else{
		dist->out = inp;
	}

	if(inp < 0.0 - dist->Threshold){
		dist->out = 0.0 - dist->Threshold;
	}else{
		dist->out = inp;
	}

	return (dist->out * dist->OutputGain);
}
