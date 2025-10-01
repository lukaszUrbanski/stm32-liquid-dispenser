/*
 * buttons.c
 *
 *  Created on: Sep 18, 2025
 *      Author: Admin
 */

#include "main.h"
#include "buttons.h"
#include "tim.h"

#define DEBOUNCE_TIME_MS 20

/* -- Button hardware configuration -- */
typedef struct
{
	GPIO_TypeDef* port;
	uint16_t pin;
}btn_hw_t;

static const btn_hw_t btn_hw[BTN_COUNT -1] =
{
	[BTN_START] = {BTN_START_GPIO_Port, BTN_START_Pin}, // BTN_START
	[BTN_STOP] = {BTN_STOP_GPIO_Port, BTN_STOP_Pin}, // BTN_STOP
	[BTN_MENU] = {BTN_MENU_GPIO_Port, BTN_MENU_Pin}  // BTN_MENU

};

/* -- Button states -- */
typedef struct
{
	uint8_t stable; // Stable state after debounce
	uint8_t sample;	// Current sampled state
	uint8_t lastEncStable; // Previous stable state
	uint32_t lastChangeTime;
	uint32_t debounceCounter;
}btn_t;

static btn_t btn_states[BTN_COUNT];

/* -- Ring buffer for button events -- */
#define QUEUE_SIZE 16
static btn_event_t eventQueue[QUEUE_SIZE];
static uint8_t q_head = 0, q_tail = 0;

static inline void q_push(btn_event_type_t event, btn_id_t id)
{
	uint8_t n = (q_head + 1) % QUEUE_SIZE;
	if(n != q_tail) // Check for overflow
	{
		eventQueue[q_head].type = event;
		eventQueue[q_head].id = id;
		q_head = n;
	}
}


/* -- Raw read state of button  -- */
static inline uint8_t readButton(btn_id_t btn_id)
{
	if (btn_id == ENCODER) return ( __HAL_TIM_GET_COUNTER(&htim2)) / 4;
	return (HAL_GPIO_ReadPin(btn_hw[btn_id].port, btn_hw[btn_id].pin) == GPIO_PIN_RESET) ? 1 : 0;
}


/* -- Public functions -- */
bool Buttons_GetEvent(btn_event_t* event)
{
	if(q_head == q_tail) return false; // Queue empty
	*event = eventQueue[q_tail];
	q_tail = (q_tail + 1) % QUEUE_SIZE;
	return true;
}

uint16_t debugCounter;
void Buttons_Init(void)
{
	for(uint8_t i = 0; i < BTN_COUNT; i++)
	{
		btn_states[i].stable = readButton((btn_id_t)i);
		btn_states[i].sample = btn_states[i].stable;
		btn_states[i].lastChangeTime = HAL_GetTick();
		btn_states[i].lastEncStable = 0;
	}
	// Initialize queue
	q_head = q_tail = 0;

	HAL_TIM_Base_Start_IT(&htim3); // Start timer for encoder if needed
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // Start encoder interface
	debugCounter = 0;
}

void Buttons_ScanISR(void)
{
	for(btn_id_t i = 0; i < BTN_COUNT; i++)
	{
		uint8_t currentSample = readButton(i);
		if(currentSample != btn_states[i].sample)
		{
			btn_states[i].sample = currentSample;
			btn_states[i].lastChangeTime = HAL_GetTick();
		}
		else if((HAL_GetTick() - btn_states[i].lastChangeTime) >= DEBOUNCE_TIME_MS)
		{
			if(btn_states[i].stable != btn_states[i].sample)
			{
				btn_states[i].stable = btn_states[i].sample;
				if(i == ENCODER)
				{
					// Encoder handling
					if((btn_states[i].stable > btn_states[i].lastEncStable) || (btn_states[i].lastEncStable - btn_states[i].stable > 32768))
					{
						q_push(EV_ENC_RIGHT, i);
					} else
					{
						q_push(EV_ENC_LEFT, i);
					}
					btn_states[i].lastEncStable = btn_states[i].stable;
				}
				else
				{
					if(btn_states[i].stable == 1)
					{
						q_push(EV_CLICK, i);
					}
					else
					{
						q_push(EV_NONE, i);
					}
				}
			}
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3) // Encoder timer
	{
		if(debugCounter >= 100) // 1 second at 0.5ms interval
		{
			debugCounter = 0;
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin); // For debug
		}
		debugCounter++;
		Buttons_ScanISR();
	}
}


