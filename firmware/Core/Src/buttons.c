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
#define HOLD_TIME_MS 600

/* -- Button hardware configuration -- */
typedef struct
{
	GPIO_TypeDef* port;
	uint16_t pin;
}btn_hw_t;

static const btn_hw_t btn_hw[BTN_COUNT] =
{
	[BTN_START] = {BTN_START_GPIO_Port, BTN_START_Pin}, // BTN_START
	[BTN_MINUS] = {BTN_MINUS_GPIO_Port, BTN_MINUS_Pin}, // BTN_STOP
	[BTN_PLUS] = {BTN_PLUS_GPIO_Port, BTN_PLUS_Pin}  // BTN_MENU

};

/* -- Button states -- */
typedef struct
{
	uint8_t stable; // Stable state after debounce
	uint8_t sample;	// Current sampled state
	uint32_t lastChangeTime;
	uint32_t debounceCounter;
	uint32_t pressStartTime;
	uint8_t holdReported;
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
	}
	// Initialize queue
	q_head = q_tail = 0;

	HAL_TIM_Base_Start_IT(&htim3); // Start timer for encoder if needed
	debugCounter = 0;
}

void Buttons_ScanISR(void)
{
	uint32_t now = HAL_GetTick();

	for(btn_id_t i = 0; i < BTN_COUNT; i++)
	{
		uint8_t currentSample = readButton(i);

		if(currentSample != btn_states[i].sample)
		{
			btn_states[i].sample = currentSample;
			btn_states[i].lastChangeTime = HAL_GetTick();
		}
		else if((now - btn_states[i].lastChangeTime) >= DEBOUNCE_TIME_MS)
		{
			if(btn_states[i].stable != btn_states[i].sample)
			{

				btn_states[i].stable = btn_states[i].sample;

				if(btn_states[i].stable == 1)
				{
					btn_states[i].pressStartTime = now;
					btn_states[i].holdReported = 0;

				}
				else
				{
					uint32_t pressDur = (btn_states[i].pressStartTime) ? (now - btn_states[i].pressStartTime) : 0;

					 if(!btn_states[i].holdReported && pressDur < HOLD_TIME_MS)
					 {
					   // Krótki klik (CLICK) – jak dotąd
						 q_push(EV_CLICK, i);
					 }
					 //Zawsze wyślij RELEASE
					 q_push(EV_RELEASE, i);

					 // Czyścimy kontekst
					 btn_states[i].pressStartTime = 0;
					 btn_states[i].holdReported   = 0;
				}
			}
		}
		// --- DETEKCJA HOLD (po ustabilizowanym naciśnięciu) ---
		if(btn_states[i].stable == 1 && !btn_states[i].holdReported)
		{
			if((now - btn_states[i].pressStartTime) >= HOLD_TIME_MS)
			{
				q_push(EV_HOLD, i);
				btn_states[i].holdReported = 1; // tylko raz na jedno naciśnięcie
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


