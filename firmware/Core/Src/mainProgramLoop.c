/*
 * mainProgramLoop.c
 *
 *  Created on: Sep 15, 2025
 *      Author: Admin
 */


#include "mainProgramLoop.h"
#include "tim.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"


AppData_t AppData;
uint32_t dispense_time_stamp = 0; // for simulate dispensing
btn_event_t lastEvent;
uint32_t repeat_time_stamp;
///////////////////////
// Main Program Loop //
///////////////////////

void MainProgramLoop(void)
{
	switch(AppData.currentMPLState)
	{
	case MPL_INIT:
		// Initialization code here
		Mpl_InitActivity();
		break;
	case MPL_IDLE:
		// Idle state code here
		Mpl_IdleActivity();
		break;
	case MPL_TFT_UPDATE:
		// Ready state code here
		Mpl_TFTUpdateActivity();
		break;
	case MPL_BUTTONS_SCAN:
		// Dispensing code here
		Mpl_ButtonsScanActivity();
		break;
	case MPL_SENSORS_READ:
		// Error handling code here
		Mpl_SensorsReadActivity();
		break;
	case MPL_PUMP_CONTROL:
		Mpl_PumpControlActivity();

		break;
	default:
		AppData.currentDeviceState = DEV_ERROR;
		break;
	}
}

void Mpl_InitActivity(void)
{
	// Initialize peripherals, variables, etc.
	AppData.volumeToDispense = 100; // Default volume to dispense
	AppData.totalDispensedVolume = 0;
	AppData.pumpState = 0; // Pump off
	AppData.displayState = DISP_NEW;

	Display_Init();
	Buttons_Init();
	FLOW_SENSOR_Init();


	AppData.currentMPLState = MPL_IDLE;
	AppData.currentDeviceState = DEV_IDLE;

	repeat_time_stamp = HAL_GetTick();
}

void Mpl_IdleActivity(void)
{
	// Code for idle state

	AppData.currentMPLState = MPL_TFT_UPDATE;
}
/* -- TFT Update Activity -- */
/* Update TFT display based on current state */

void Mpl_TFTUpdateActivity(void)
{
	if(AppData.displayState == DISP_NONE)
	{
		AppData.currentMPLState = MPL_BUTTONS_SCAN;
		return; // No need to update
	}
	// Update display based on device state

	switch(AppData.currentDeviceState)
	{
	case DEV_IDLE:

		if (AppData.displayState == DISP_NEW)
		{
			Display_PrintIdleScreen(AppData.volumeToDispense);
		}

		else if (AppData.displayState == DISP_UPDATE)
		{
			Display_UpdateVolume(AppData.volumeToDispense);
		}

		AppData.displayState = DISP_NONE; // Reset display state after update
		break;

	case DEV_READY:
		break;

	case DEV_DISPENSE:
		if (AppData.displayState == DISP_NEW)
		{
			Display_PrintDispenseScreen();
		}
		else if (AppData.displayState == DISP_UPDATE)
		{
			Display_UpdateVolume(AppData.totalDispensedVolume);
		}
		AppData.displayState = DISP_NONE; // Reset display state after update
		break;
	case DEV_ERROR:
		// Display error screen
		break;

	default:
		break;
	}
	AppData.currentMPLState = MPL_BUTTONS_SCAN;
}

/* -- Buttons Scan Activity -- */
void Mpl_ButtonsScanActivity(void)
{
	// Buttons_Scan();


	if(Buttons_GetEvent(&lastEvent))
	{
	// Handle button events based on current device state
		switch (AppData.currentDeviceState)
		{
		case DEV_IDLE:
			if (lastEvent.id == BTN_PLUS && (lastEvent.type == EV_CLICK || lastEvent.type == EV_HOLD))
			{
				AppData.volumeToDispense += 10;
				AppData.displayState = DISP_UPDATE;


			} else if (lastEvent.id == BTN_MINUS && (lastEvent.type == EV_CLICK || lastEvent.type == EV_HOLD)) {
				if (AppData.volumeToDispense >= 10) {
					AppData.volumeToDispense -= 10;
					AppData.displayState = DISP_UPDATE;
				}
			}
			if (lastEvent.id == BTN_START && lastEvent.type == EV_CLICK) {

				AppData.currentDeviceState = DEV_DISPENSE;

				AppData.displayState = DISP_NEW;
				dispense_time_stamp = HAL_GetTick(); // use for simulate dispensing
			}
			break;
		case DEV_READY:
			// Ready state code here
			break;
		case DEV_DISPENSE:
			if (lastEvent.id == BTN_START && lastEvent.type == EV_CLICK) {
				AppData.currentDeviceState = DEV_IDLE;
				AppData.displayState = DISP_NEW;
				AppData.totalDispensedVolume = 0; // Reset for next time
			}
			break;

		case DEV_ERROR:
				// Error handling code here
			break;
		default:
			AppData.currentDeviceState = DEV_ERROR;
			break;
		}
	}
	AppData.currentMPLState = MPL_SENSORS_READ;
}

void Mpl_SensorsReadActivity(void)
{
	// Code for reading sensors, if any
	uint8_t Msg[32];
	//uint16_t flowCounter;



	//------ Simulate sensor reading here ------//

	if(AppData.currentDeviceState == DEV_DISPENSE)
	{

		//flowCounter = __HAL_TIM_GET_COUNTER(&htim4);

		// Simulate reading a flow sensor or similar

			if (AppData.totalDispensedVolume >= AppData.volumeToDispense) // Example condition to stop dispensing
			{
				AppData.currentDeviceState = DEV_IDLE;
				AppData.totalDispensedVolume = FLOW_SENSOR_GetValue_ml();
				sprintf(Msg, "FLOW counter = %d \n\r", AppData.totalDispensedVolume);
				AppData.totalDispensedVolume = 0; // Reset for next time
				FLOW_SENSOR_ResetValue();
				AppData.displayState = DISP_NEW;
			}
			else{
				if(HAL_GetTick() - dispense_time_stamp >= 500){
					dispense_time_stamp = HAL_GetTick();
					AppData.totalDispensedVolume = FLOW_SENSOR_GetValue_ml();
					sprintf(Msg, "FLOW counter = %d \n\r", AppData.totalDispensedVolume);
					HAL_UART_Transmit(&huart2, Msg, strlen(Msg), 1000);
					//AppData.totalDispensedVolume += 10; // Increment dispensed volume
					AppData.displayState = DISP_UPDATE; // Mark display for update
					// Simulate time taken to dispense

				}
			}
	}
	AppData.currentMPLState = MPL_PUMP_CONTROL;
}

void Mpl_PumpControlActivity(void)
{
	// Control pump based on current state
	if (AppData.currentDeviceState == DEV_DISPENSE)
	{
		if (AppData.pumpState == 0)
		{
			// Turn on the pump
			HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin, GPIO_PIN_SET);
			AppData.pumpState = 1;
		}
	}
	else
	{
		if (AppData.pumpState == 1)
		{
			// Turn off the pump
			HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin, GPIO_PIN_RESET);
			AppData.pumpState = 0;
		}
	}
	AppData.currentMPLState = MPL_IDLE;
}

