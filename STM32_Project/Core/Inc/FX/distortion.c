/*
 * distorsion.c
 *
 *  Created on: 26 lug 2021
 *      Author: Brian
 */


#include "distortion.h"


void Distortion_Init(Distortion *dist, float inputgain, float crossover, float clipping, float outputgain){
	dist->InputGain = inputgain;
	dist->OutputGain = outputgain;
	dist->Clipping = clipping;
	dist->Crossover = crossover;
	dist->out = 0.0f;
}
float Distortion_Update(Distortion *dist, float inp){
	dist->out = inp * dist->InputGain;
	if(dist->out <= dist->Crossover && dist->out >= 0.0 - dist->Crossover) dist->out = 0.0f;
	if(dist->out > dist->Crossover) dist->out -= dist->Crossover;
	if(dist->out < 0.0 - dist->Crossover) dist->out += dist->Crossover;

	if(dist->out > dist->Clipping) dist->out = dist->Clipping;
	if(dist->out < 0.0 - dist->Clipping) dist->out = 0.0 - dist->Clipping;
	dist->out *= dist->OutputGain;

	return dist->out;
}
