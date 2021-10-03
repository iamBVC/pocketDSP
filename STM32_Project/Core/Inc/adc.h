/*
 * adc.h
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stm32h7xx_hal.h"

#define M0_PORT GPIOD
#define M0_PIN  GPIO_PIN_8
#define M1_PORT GPIOD
#define M1_PIN  GPIO_PIN_9
#define RST_PORT GPIOD
#define RST_PIN  GPIO_PIN_10

extern I2S_HandleTypeDef hi2s2;
extern TIM_HandleTypeDef htim12;

void ADC_Init();

#endif /* INC_ADC_H_ */
