/*
 * display.c
 *
 *  Created on: Sep 17, 2025
 *      Author: Admin
 */
#include "main.h"
#include "display.h"
#include "spi.h"
#include "TFT_ILI9341.h"
#include "GFX_Color.h"
#include "fonts/fonts.h"
#include "GFX_EnhancedFonts.h"
#include "EnhancedFonts/arialBlack_20ptFontInfo.h"
#include "string.h"
#include <stdio.h>
#include "mainProgramLoop.h"

uint8_t Msg[32];

typedef enum
{
	TFT_INIT,
	TFT_IDLE,
	TFT_DISPENSE,
	TFT_MENU,
	TFT_ERROR,
}TFT_State_t;

TFT_State_t TFT_State = TFT_INIT;

uint8_t isDisplayChanged = 1;

void Display_Init(void)
{
	ILI9341_Init(&hspi1);
	GFX_SetFont(font_8x5);
	EF_SetFont(&arialBlack_20ptFontInfo);
	ILI9341_ClearDisplay(ILI9341_WHITE);

	EF_PutString((const uint8_t*)"Liquid Dispenser", 45, 50, ILI9341_BLACK, BG_TRANSPARENT, ILI9341_WHITE);

	TFT_State = TFT_IDLE;
}

void displayIdleScreen(void)
{
	if(!isDisplayChanged) return;

	ILI9341_ClearDisplay(ILI9341_WHITE);
	EF_PutString((const uint8_t*)"Liquid Dispenser", 45, 10, ILI9341_RED, BG_TRANSPARENT, ILI9341_WHITE);

	GFX_DrawFillRectangle(20, (240 - 10 - 30), 85, 30, ILI9341_DARKGREY );
	EF_PutString((const uint8_t*)"MENU", 25, (240 - 10 - 30), ILI9341_WHITE, BG_TRANSPARENT, ILI9341_DARKGREY);

	GFX_DrawFillRoundRectangle(85, 90, 150, 50, 10, ILI9341_DARKGREY);
	EF_PutString((const uint8_t*)"DISPENSE", 90, 100, ILI9341_WHITE, BG_TRANSPARENT, ILI9341_DARKGREY);

	sprintf((char*)Msg, "Volume: %d ml", 100);
	EF_PutString((const uint8_t*)Msg, 50, 150,  ILI9341_BLACK, BG_TRANSPARENT, ILI9341_WHITE);

	isDisplayChanged = 0;
}

void Display_Task(void)
{
	switch(TFT_State)
	{
	case TFT_IDLE:
		// Idle state code here
		displayIdleScreen();
		break;
	case TFT_DISPENSE:
		// Dispensing state code here

		break;
	case TFT_MENU:
		// Menu state code here
		break;
	case TFT_ERROR:
		// Error handling code here
		break;
	default:
		TFT_State = TFT_ERROR;
		break;
	}
}





