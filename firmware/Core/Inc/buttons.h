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
	BTN_MINUS,
	BTN_PLUS,
	BTN_COUNT
}btn_id_t;

typedef enum
{
	EV_NONE = 0,
	EV_CLICK,
	EV_HOLD,
	EV_RELEASE
}btn_event_type_t;

typedef struct
{
	btn_event_type_t type;
	btn_id_t id;
}btn_event_t;

void Buttons_Init(void);
bool Buttons_GetEvent(btn_event_t* event);



#endif /* INC_BUTTONS_H_ */
