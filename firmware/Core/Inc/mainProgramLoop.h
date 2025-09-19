/*
 * mainProgramLoop.h
 *
 *  Created on: Sep 17, 2025
 *      Author: Admin
 */

#ifndef INC_MAINPROGRAMLOOP_H_
#define INC_MAINPROGRAMLOOP_H_

#include "main.h"
#include "display.h"
#include "buttons.h"


typedef enum
{
	MPL_INIT,
	MPL_IDLE,
	MPL_READY,
	MPL_DISPENSE,
	MPL_ERROR
}MPL_State_t;

typedef struct
{
	uint32_t 		volumeToDispense; // in milliliters
	uint32_t 		totalDispensedVolume; // in milliliters
	MPL_State_t 	currentState;

	uint8_t 		isDisplayChanged;

	uint8_t 		StartButtonPressed;
	uint8_t 		StopButtonPressed;
	uint8_t 		MenuButtonPressed;


}AppData_t;

void MainProgramLoop(void);


#endif /* INC_MAINPROGRAMLOOP_H_ */
