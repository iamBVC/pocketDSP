/*
 * wah.h
 *
 *  Created on: Jul 23, 2021
 *      Author: Brian
 */

#ifndef INC_WAH_H_
#define INC_WAH_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FILTER_LENGHT 32

typedef struct {
	float buf[WAH_FILTER_LENGHT];
	float WAH_IMPULSE_RESPONSE[WAH_FILTER_LENGHT];
	uint16_t bufIndex;
	uint16_t filterLenght;
	uint16_t RangeMin;
	uint16_t RangeMax;
	uint16_t Bandwidth;
	float MaxAmplitude;
	float out;
} WahFilter;
void Wah_Init(WahFilter *wah);
float Wah_Update(WahFilter *wah, float inp);
void Wah_CalcCoeff(WahFilter *wah, float SAMPLE_TIME_S);

#endif /* INC_WAH_H_ */
