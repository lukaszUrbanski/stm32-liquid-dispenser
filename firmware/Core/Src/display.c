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

extern AppData_t AppData;

void Display_Init(void)
{
	ILI9341_Init(&hspi1);
	GFX_SetFont(font_8x5);
	EF_SetFont(&arialBlack_20ptFontInfo);
	ILI9341_ClearDisplay(ILI9341_WHITE);

	EF_PutString((const uint8_t*)"Liquid Dispenser", 45, 50, ILI9341_BLACK, BG_TRANSPARENT, ILI9341_WHITE);

	AppData.isDisplayChanged = 1;
}

void Display_PrintIdleScreen(void)
{
	uint8_t Msg[32];

	if(!AppData.isDisplayChanged) return;

	ILI9341_ClearDisplay(ILI9341_WHITE);
	EF_PutString((const uint8_t*)"Liquid Dispenser", 45, 10, ILI9341_RED, BG_TRANSPARENT, ILI9341_WHITE);

	GFX_DrawFillRectangle(20, (240 - 10 - 30), 85, 30, ILI9341_DARKGREY );
	EF_PutString((const uint8_t*)"MENU", 25, (240 - 10 - 30), ILI9341_WHITE, BG_TRANSPARENT, ILI9341_DARKGREY);

	GFX_DrawFillRoundRectangle(85, 90, 150, 50, 10, ILI9341_DARKGREY);
	EF_PutString((const uint8_t*)"DISPENSE", 90, 100, ILI9341_WHITE, BG_TRANSPARENT, ILI9341_DARKGREY);


	sprintf((char*)Msg, "Volume: %d ml", AppData.volumeToDispense);
	EF_PutString((const uint8_t*)Msg, 50, 150,  ILI9341_BLACK, BG_TRANSPARENT, ILI9341_WHITE);

	AppData.isDisplayChanged = 0;

}
void Display_DispenseScreen(void)
{
	uint8_t Msg[32];

	if(!AppData.isDisplayChanged) return;

	ILI9341_ClearDisplay(ILI9341_WHITE);
	EF_PutString((const uint8_t*)"Dispensing...", 60, 100, ILI9341_BLUE, BG_TRANSPARENT, ILI9341_WHITE);

	sprintf((char*)Msg, "Volume: %d ml", AppData.totalDispensedVolume);
	EF_PutString((const uint8_t*)Msg, 50, 150,  ILI9341_BLACK, BG_TRANSPARENT, ILI9341_WHITE);

	AppData.isDisplayChanged = 0;
}







