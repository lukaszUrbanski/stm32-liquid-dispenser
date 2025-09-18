/*
 * buttons.c
 *
 *  Created on: Sep 18, 2025
 *      Author: Admin
 */

#include "main.h"
#include "buttons.h"

#define DEBOUNCE_TIME_MS 50

typedef struct
{
	GPIO_TypeDef* port;
	uint16_t pin;
}btn_hw_t;

static const btn_hw_t btn_hw[BTN_COUNT] =
{
	{BTN_START_GPIO_Port, BTN_START_Pin}, // BTN_START
	{BTN_STOP_GPIO_Port, BTN_STOP_Pin}, // BTN_STOP
	{BTN_MENU_GPIO_Port, BTN_MENU_Pin}  // BTN_MENU
};

typedef struct
{
	uint8_t stable;
	uint8_t sample;
	uint32_t lastChangeTime;
	uint8_t click_Flag;
}btn_t;

static btn_t btns[BTN_COUNT];

static inline uint8_t readButton(Btn_id_t btn_id)
{
	return (HAL_GPIO_ReadPin(btn_hw[btn_id].port, btn_hw[btn_id].pin) == GPIO_PIN_RESET) ? 1 : 0;
}

void Buttons_Init(void)
{
	for(uint8_t i = 0; i < BTN_COUNT; i++)
	{
		btns[i].stable = readButton((Btn_id_t)i);
		btns[i].sample = btns[i].stable;
		btns[i].lastChangeTime = HAL_GetTick();
		btns[i].click_Flag = 0;
	}
}

void Buttons_Scan(void)
{
	uint32_t currentTime = HAL_GetTick();
	for(uint8_t i = 0; i < BTN_COUNT; i++)
	{
		uint8_t currentSample = readButton((Btn_id_t)i);
		if(currentSample != btns[i].sample)
		{
			btns[i].sample = currentSample;
			btns[i].lastChangeTime = currentTime;
		}
		else if((currentTime - btns[i].lastChangeTime) >= DEBOUNCE_TIME_MS)
		{
			if(btns[i].stable != btns[i].sample)
			{
				btns[i].stable = btns[i].sample;
				if(btns[i].stable == 1)
				{
					btns[i].click_Flag = 1;
				}
			}
		}
	}
}
