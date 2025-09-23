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
	uint32_t lastChangeTime;
	uint32_t debounceCounter;
	uint8_t click_flag;
}btn_t;

static btn_t btn_states[BTN_COUNT];

/* -- Encoder states -- */
uint8_t enc_A;
uint8_t enc_B;
uint8_t enc_state;
uint8_t enc_prewState;

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

bool Buttons_GetEvent(btn_event_t* event)
{
	if(q_head == q_tail) return false; // Queue empty
	*event = eventQueue[q_tail];
	q_tail = (q_tail + 1) % QUEUE_SIZE;
	return true;
}

/* -- Raw read state of button  -- */
static inline uint8_t readButton(btn_id_t btn_id)
{
	return (HAL_GPIO_ReadPin(btn_hw[btn_id].port, btn_hw[btn_id].pin) == GPIO_PIN_RESET) ? 1 : 0;
}


/* -- Public functions -- */
uint16_t debugCounter;
void Buttons_Init(void)
{
	for(uint8_t i = 0; i < BTN_COUNT; i++)
	{
		btn_states[i].stable = readButton((btn_id_t)i);
		btn_states[i].sample = btn_states[i].stable;
		btn_states[i].lastChangeTime = HAL_GetTick();
		btn_states[i].click_flag = 0;
	}
	// Initialize encoder state
	enc_A = HAL_GPIO_ReadPin(ENC_A_GPIO_Port, ENC_A_Pin);
	enc_B = HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin);
	enc_state = (enc_A << 1) | enc_B;
	enc_prewState = enc_state;

	// Initialize queue
	q_head = q_tail = 0;

	HAL_TIM_Base_Start_IT(&htim3); // Start timer for encoder if needed
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
				if(btn_states[i].stable == 1)
				{
					btn_states[i].click_flag = 1;
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




void Buttons_Scan(void)
{
	uint32_t currentTime = HAL_GetTick();
	for(uint8_t i = 0; i < BTN_COUNT; i++)
	{
		uint8_t currentSample = readButton((btn_id_t)i);
		if(currentSample != btn_states[i].sample)
		{
			btn_states[i].sample = currentSample;
			btn_states[i].lastChangeTime = currentTime;
		}
		else if((currentTime - btn_states[i].lastChangeTime) >= DEBOUNCE_TIME_MS)
		{
			if(btn_states[i].stable != btn_states[i].sample)
			{
				btn_states[i].stable = btn_states[i].sample;
				if(btn_states[i].stable == 1)
				{
					btn_states[i].click_flag = 1;
				}
			}
		}
	}
}

bool Button_WasClicked(btn_id_t id) {
  if (btn_states[id].click_flag) {
    btn_states[id].click_flag = 0;
    return true;
  }
  return false;
}

EncoderDir_t EncoderRotated(void)
{
	//uint32_t currentTime = HAL_GetTick();
	enc_A = HAL_GPIO_ReadPin(ENC_A_GPIO_Port, ENC_A_Pin);
	enc_B = HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin);
	enc_state = (enc_A << 1) | enc_B;


	if(enc_state != enc_prewState)
	{
		// Simple state machine for quadrature encoder
		if((enc_prewState == 0b00 && enc_state == 0b01) ||
		   (enc_prewState == 0b01 && enc_state == 0b11) ||
		   (enc_prewState == 0b11 && enc_state == 0b10) ||
		   (enc_prewState == 0b10 && enc_state == 0b00))
		{
			// Clockwise rotation
			// Handle clockwise rotation event here
			return ENC_CW;
		}
		else if((enc_prewState == 0b00 && enc_state == 0b10) ||
				(enc_prewState == 0b10 && enc_state == 0b11) ||
				(enc_prewState == 0b11 && enc_state == 0b01) ||
				(enc_prewState == 0b01 && enc_state == 0b00))
		{
			// Counter-clockwise rotation
			// Handle counter-clockwise rotation event here
			return ENC_CCW;
		}
		enc_prewState = enc_state;
	}
	return ENC_NONE;
}


