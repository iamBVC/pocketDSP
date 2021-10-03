#ifndef  INC_ALLPASS_FILTER_H_
#define  INC_ALLPASS_FILTER_H_

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
} ALLPASSFilter;
void ALLPASSFilter_Init(ALLPASSFilter *apf, uint16_t D, float G);
float ALLPASSFilter_Update(ALLPASSFilter *apf, float inp);

#endif
