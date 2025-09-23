/*
 * buttons.h
 *
 *  Created on: Sep 18, 2025
 *      Author: Admin
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include "stdint.h"
#include "stdbool.h"

typedef enum
{
	BTN_START,
	BTN_STOP,
	BTN_MENU,
	BTN_ENCODER, // Not a button, just for encoder handling
	BTN_COUNT
}btn_id_t;

typedef enum
{
	EV_NONE = 0,
	EV_CLICK,
	EV_LEFT,
	EV_RIGHT
}btn_event_type_t;

typedef struct
{
	btn_event_type_t type;
	btn_id_t id;
}btn_event_t;

typedef enum
{
	ENC_NONE = 0,
	ENC_CW = 1,
	ENC_CCW = -1
}EncoderDir_t;

void Buttons_Init(void);
void Buttons_Scan(void);
bool Button_WasClicked(btn_id_t id);
EncoderDir_t EncoderRotated(void);


#endif /* INC_BUTTONS_H_ */
