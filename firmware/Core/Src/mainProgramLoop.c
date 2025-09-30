/*
 * mainProgramLoop.c
 *
 *  Created on: Sep 15, 2025
 *      Author: Admin
 */


#include "mainProgramLoop.h"


AppData_t AppData;
uint32_t dispense_time_stamp = 0; // for simulate dispensing
btn_event_t lastEvent;
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
		// Pump control code here

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
	AppData.displayState = DISP_NEW;

	Display_Init();
	Buttons_Init();

	AppData.currentMPLState = MPL_IDLE;
	AppData.currentDeviceState = DEV_IDLE;
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
		Display_PrintIdleScreen();
		break;

	case DEV_READY:
		break;

	case DEV_DISPENSE:
		Display_PrintDispenseScreen();
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

	Buttons_GetEvent(&lastEvent);
	switch (AppData.currentDeviceState)
	{
	case DEV_IDLE:
		EncoderDir_t Rotation = EncoderRotated();

		if (Rotation == ENC_CW) {
			AppData.volumeToDispense += 10;
			AppData.displayState = DISP_UPDATE;
		} else if (Rotation == ENC_CCW) {
			if (AppData.volumeToDispense >= 10) {
				AppData.volumeToDispense -= 10;
				AppData.displayState = DISP_UPDATE;
			}
		}

		if (lastEvent.id == BTN_START && lastEvent.type == EV_CLICK) {

			AppData.currentDeviceState = DEV_DISPENSE;

			AppData.displayState = DISP_NEW;
			dispense_time_stamp = HAL_GetTick(); // use for simulate dispensing
			//AppData.StartButtonPressed = 0; // Reset button state
		}
		break;
	case DEV_READY:
		// Ready state code here
		break;
	case DEV_DISPENSE:
		if (lastEvent.id == BTN_STOP && lastEvent.type == EV_CLICK) {
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

	AppData.currentMPLState = MPL_SENSORS_READ;
}

void Mpl_SensorsReadActivity(void)
{
	// Code for reading sensors, if any


	//------ Simulate sensor reading here ------//

	if(AppData.currentDeviceState == DEV_DISPENSE)
	{
		// Simulate reading a flow sensor or similar
		if (HAL_GetTick() - dispense_time_stamp >= 500){
				dispense_time_stamp = HAL_GetTick();
				AppData.totalDispensedVolume += 10; // Increment dispensed volume
				AppData.displayState = DISP_UPDATE; // Mark display for update
				// Simulate time taken to dispense
			}
			if (AppData.totalDispensedVolume >= AppData.volumeToDispense) // Example condition to stop dispensing
			{
				AppData.currentDeviceState = DEV_IDLE;
				AppData.totalDispensedVolume = 0; // Reset for next time
				AppData.displayState = DISP_NEW;
			}
	}
	AppData.currentMPLState = MPL_IDLE;
}

