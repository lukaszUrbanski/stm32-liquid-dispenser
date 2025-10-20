/*
 * sensors.c
 *
 *  Created on: Oct 20, 2025
 *      Author: Admin
 */
#include "sensors.h"
#include "tim.h"

volatile uint16_t pulseCounter;

void FLOW_SENSOR_Init(){
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	pulseCounter = 0;
}

void FLOW_SENSOR_GetValue(uint16_t* flowValue){
	*flowValue = pulseCounter;
}

void FLOW_SENSOR_ResetValue(){
	pulseCounter = 0;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // If the interrupt is triggered by channel 1
	{
		pulseCounter ++;
	}
}
