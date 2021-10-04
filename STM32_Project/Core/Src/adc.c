/*
 * adc.c
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */
#include "adc.h"

void ADC_Init(){
	HAL_TIM_PWM_Init(&htim12);
	HAL_I2S_Init(&hi2s2);
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
	HAL_Delay(100);
	HAL_GPIO_WritePin (GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
}
