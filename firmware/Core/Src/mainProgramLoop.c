/*
 * mainProgramLoop.c
 *
 *  Created on: Sep 15, 2025
 *      Author: Admin
 */

typedef enum
{
	MPL_INIT,
	MPL_IDLE,
	MPL_DISPENSE,
	MPL_ERROR
}MPL_State;

MPL_State MPL_CurrentState = MPL_INIT;

void MainProgramLoop(void)
{
	switch(MPL_CurrentState)
	{
	case MPL_INIT:
		// Initialization code here
		MPL_CurrentState = MPL_IDLE;
		break;
	case MPL_IDLE:
		// Idle state code here
		break;
	case MPL_DISPENSE:
		// Dispensing code here
		break;
	case MPL_ERROR:
		// Error handling code here
		break;
	default:
		MPL_CurrentState = MPL_ERROR;
		break;
	}
}
