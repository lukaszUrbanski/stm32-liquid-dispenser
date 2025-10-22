/*
 * sensors.c
 *
 *  Created on: Oct 20, 2025
 *      Author: Admin
 */
#include "sensors.h"
#include "tim.h"

volatile uint32_t pulseCounter;

void FLOW_SENSOR_Init(){
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	pulseCounter = 0;
}
uint32_t FLOW_SENSOR_GetValue(void){
	return pulseCounter;
}

uint32_t FLOW_SENSOR_GetValue_ml(void){
	return (uint32_t)((float)pulseCounter / PULSE_PER_ML);
}

void FLOW_SENSOR_ResetValue(){
	pulseCounter = 0;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // If the interrupt is triggered by channel 1
	{
		pulseCounter ++;
	}
}
