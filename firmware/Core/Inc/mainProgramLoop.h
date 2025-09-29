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
	MPL_TFT_UPDATE,
	MPL_BUTTONS_SCAN,
	MPL_SENSORS_READ,
	MPL_PUMP_CONTROL
}MPL_State_t;


typedef enum
{
	DEV_IDLE,
	DEV_READY,
	DEV_DISPENSE,
	DEV_ERROR
}Device_State_t;

typedef enum
{
	DISP_NONE,
	DISP_NEW,
	DISP_UPDATE
}Disp_State_t;

typedef struct
{
	uint32_t 		volumeToDispense; // in milliliters
	uint32_t 		totalDispensedVolume; // in milliliters
	MPL_State_t 	currentMPLState;
	Device_State_t 	currentDeviceState;

	Disp_State_t 	displayState;
	uint8_t 		isStageChanged;

	uint8_t 		StartButtonPressed;
	uint8_t 		StopButtonPressed;
	uint8_t 		MenuButtonPressed;


}AppData_t;

void MainProgramLoop(void);

void Mpl_InitActivity(void);
void Mpl_IdleActivity(void);
void Mpl_TFTUpdateActivity(void);
void Mpl_ButtonsScanActivity(void);
void Mpl_SensorsReadActivity(void);



#endif /* INC_MAINPROGRAMLOOP_H_ */
