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

uint8_t Msg[32];

void Display_Init(void)
{
	ILI9341_Init(&hspi1);
	GFX_SetFont(font_8x5);
	EF_SetFont(&arialBlack_20ptFontInfo);
	ILI9341_ClearDisplay(ILI9341_WHITE);

	EF_PutString((const uint8_t*)"Liquid Dispenser", 45, 50, ILI9341_BLACK, BG_TRANSPARENT, ILI9341_WHITE);


}
