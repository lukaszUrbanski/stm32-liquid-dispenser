/*
 * display.h
 *
 *  Created on: Sep 17, 2025
 *      Author: Admin
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "main.h"

void Display_Init(void);
void Display_UpdateVolume(uint16_t volume);
void Display_PrintIdleScreen(uint16_t volume);
void Display_PrintDispenseScreen(void);


#endif /* INC_DISPLAY_H_ */
