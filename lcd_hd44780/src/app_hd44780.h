/*
 * Copyright (c) 2023 
 *
 * Hd44780 driver
 */

#pragma once

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define APP_HD44780_SUCCESS      0
#define APP_HD44780_NOT_READY    1

class AppHd44780 {
public:
	static AppHd44780 &Instance()
	{
		static AppHd44780 sAppHd44780;
		return sAppHd44780;
	};

	static int InitLCD();
    static void LCDSetCursor(uint8_t col, uint8_t row);
    static void LCDString(char *msg);
    static void LCDDigit(uint32_t value);
	static void LCDClear();

private:

};