/*
 * mainProgramLoop.c
 *
 *  Created on: Sep 15, 2025
 *      Author: Admin
 */


#include "mainProgramLoop.h"


AppData_t AppData =
{
	.isDisplayChanged = 1,
	.volumeToDispense = 100,
	.totalDispensedVolume = 0,
	.currentState = MPL_INIT,
//	.StartButtonPressed = 0,
//	.StopButtonPressed = 0,
//	.MenuButtonPressed = 0,
};

void InitActivity(void);
void IdleActivity(void);
void DispenseActivity(void);

uint32_t time_stamp = 0; // for simulate dispensing
btn_event_t lastEvent;
///////////////////////
// Main Program Loop //
///////////////////////

void MainProgramLoop(void)
{
	switch(AppData.currentState)
	{
	case MPL_INIT:
		// Initialization code here
		InitActivity();
		break;
	case MPL_IDLE:
		// Idle state code here
		IdleActivity();
		break;
	case MPL_READY:
		// Ready state code here
		break;
	case MPL_DISPENSE:
		// Dispensing code here
		DispenseActivity();
		break;
	case MPL_ERROR:
		// Error handling code here
		break;
	default:
		AppData.currentState = MPL_ERROR;
		break;
	}
}

void InitActivity(void)
{
	// Initialize peripherals, variables, etc.
	Display_Init();
	Buttons_Init();

	AppData.currentState = MPL_IDLE;
}

void IdleActivity(void)
{
	// Code for idle state
	//Buttons_Scan();

	Buttons_GetEvent(&lastEvent);

	EncoderDir_t Rotation = EncoderRotated();

	if(Rotation == ENC_CW)
	{
		AppData.volumeToDispense += 10;
		AppData.isDisplayChanged = 1;
	}
	else if (Rotation == ENC_CCW)
	{
		if (AppData.volumeToDispense >= 10)
		{
			AppData.volumeToDispense -= 10;
			AppData.isDisplayChanged = 1;
		}
	}

	Display_Update();

	if (lastEvent.id == BTN_START && lastEvent.type == EV_CLICK)
	{
		AppData.currentState = MPL_DISPENSE;
		AppData.isDisplayChanged = 1;
		time_stamp = HAL_GetTick(); // use for simulate dispensing
		//AppData.StartButtonPressed = 0; // Reset button state
	}
}

void DispenseActivity(void)
{

	// Code for dispensing state
	Display_Update();
	Buttons_Scan();
	if (Button_WasClicked(BTN_STOP))
	{
		AppData.currentState = MPL_IDLE;
		AppData.isDisplayChanged = 1;
		//AppData.StopButtonPressed = 0; // Reset button state
	}
	// Simulate dispensing process
	if (HAL_GetTick() - time_stamp >= 1000){
		time_stamp = HAL_GetTick();
		AppData.totalDispensedVolume += 10; // Increment dispensed volume
		AppData.isDisplayChanged = 1; // Mark display for update
		// Simulate time taken to dispense
	}
	if (AppData.totalDispensedVolume >= AppData.volumeToDispense) // Example condition to stop dispensing
	{
		AppData.currentState = MPL_IDLE;
		AppData.totalDispensedVolume = 0; // Reset for next time
		AppData.isDisplayChanged = 1;
	}
}


