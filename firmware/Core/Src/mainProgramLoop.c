/*
 * mainProgramLoop.c
 *
 *  Created on: Sep 15, 2025
 *      Author: Admin
 */


#include "mainProgramLoop.h"

AppData_t AppData =
{
	.totalDispensedVolume = 40,
	.currentState = MPL_INIT,
	.StartButtonPressed = 0,
	.StopButtonPressed = 0,
	.MenuButtonPressed = 0,
};

void InitActivity(void);
void IdleActivity(void);



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
	AppData.currentState = MPL_IDLE;
}

void IdleActivity(void)
{
	// Code for idle state
	Display_IdleScreen();
	if (AppData.StartButtonPressed)
	{
		AppData.currentState = MPL_DISPENSE;
		AppData.isDisplayChanged = 1;
		AppData.StartButtonPressed = 0; // Reset button state
	}
}

