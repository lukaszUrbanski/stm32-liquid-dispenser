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


void Display_Init(void)
{
	ILI9341_Init(&hspi1);
	GFX_SetFont(font_8x5);
	EF_SetFont(&arialBlack_20ptFontInfo);
	ILI9341_ClearDisplay(ILI9341_WHITE);

	EF_PutString((const uint8_t*)"Liquid Dispenser", 45, 50, ILI9341_BLACK, BG_TRANSPARENT, ILI9341_WHITE);
}



void Display_PrintIdleScreen(uint16_t volume)
{
	uint8_t Msg[32];

	ILI9341_ClearDisplay(ILI9341_WHITE);
	EF_PutString((const uint8_t*) "Liquid Dispenser", 45, 10, ILI9341_RED, BG_TRANSPARENT, ILI9341_WHITE);

	ILI9341_ClearArea(85, 90, 150, 50, ILI9341_DARKGREY);
	EF_PutString((const uint8_t*) "DISPENSE", 90, 100, ILI9341_WHITE, BG_TRANSPARENT, ILI9341_DARKGREY);

	sprintf((char*) Msg, "Volume: %d ml", volume);
	EF_PutString((const uint8_t*) Msg, 50, 150, ILI9341_BLACK, BG_TRANSPARENT, ILI9341_WHITE);
}



void Display_PrintDispenseScreen(void)
{
	ILI9341_ClearDisplay(ILI9341_WHITE);
	EF_PutString((const uint8_t*) "Dispensing...", 60, 100, ILI9341_BLUE, BG_TRANSPARENT, ILI9341_WHITE);

	EF_PutString((const uint8_t*) "Volume: 0 ml", 50, 150, ILI9341_BLACK, BG_TRANSPARENT, ILI9341_WHITE);
}

void Display_UpdateVolume(uint16_t volume)
{
	uint8_t Msg[16];
	// Update only the volume value
	ILI9341_ClearArea(165, 150, 120, 30, ILI9341_WHITE);
	sprintf((char*)Msg, "%d ml", volume);
	EF_PutString((const uint8_t*)Msg, 170, 150,  ILI9341_BLACK, BG_TRANSPARENT, ILI9341_WHITE);
}






