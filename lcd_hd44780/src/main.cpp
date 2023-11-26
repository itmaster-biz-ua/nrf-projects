/*
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Display text strings on HD44780 based 20x4 LCD controller
 * using GPIO for parallel interface on nRF7002 DK.
 *
 * Datasheet: http://lcd-linux.sourceforge.net/pdfdocs/hd44780.pdf
 *
 * LCD Wiring
 * ----------
 *
 * The wiring for the LCD is as follows:
 * 1 : GND
 * 2 : 5V
 * 3 : Contrast (0-5V)*
 * 4 : RS (Register Select)
 * 5 : R/W (Read Write)       - GROUND THIS PIN
 * 6 : Enable or Strobe
 * 7 : Data Bit 0             - NOT USED
 * 8 : Data Bit 1             - NOT USED
 * 9 : Data Bit 2             - NOT USED
 * 10: Data Bit 3             - NOT USED
 * 11: Data Bit 4
 * 12: Data Bit 5
 * 13: Data Bit 6
 * 14: Data Bit 7
 * 15: LCD Backlight +5V**
 * 16: LCD Backlight GND
 *
 *
 * nRF7002 DK
 * -----------
 *
 * The gpio_atmel_sam3 driver is being used.
 *
 * This sample app display text strings per line & page wise.
 *
 * Every 3 second you should see this repeatedly
 * on display:
 * 
 *     *********************
 *     nRF7002 DK 0
 *
 *      ------------------
 *     HD44780-LCD!
 */
#include "app_hd44780.h"

int main(void)
{
	uint32_t value = 12304;
    AppHd44780::InitLCD();
		AppHd44780::LCDSetCursor(0, 0);
		AppHd44780::LCDString((char*)"Current=      A");

		AppHd44780::LCDSetCursor(0, 1);
		AppHd44780::LCDString((char*)"Power=        W");
	while (1) {
		printk("Page 1: message\n");

		//AppHd44780::LCDClear() ;

		AppHd44780::LCDSetCursor(8, 0);
		AppHd44780::LCDDigit(value);


		AppHd44780::LCDSetCursor(6, 1);
		AppHd44780::LCDDigit(value);

/*
        AppHd44780::LCDString("********************");
		AppHd44780::LCDSetCursor(0, 1);
        AppHd44780::LCDString("nRF7002 DK ");
        AppHd44780::LCDDigit(i);
		
		k_msleep(MSEC_PER_SEC * 3U);

	
		AppHd44780::LCDClear() ;

		printk("Page 2: message\n");

        AppHd44780::LCDString("-------------------");
		AppHd44780::LCDSetCursor(0, 1);
        AppHd44780::LCDString("HD44780-LCD!");
		k_msleep(MSEC_PER_SEC * 3U);


		AppHd44780::LCDClear() ;*/
      k_msleep(MSEC_PER_SEC * 3U);
		--value;
	}
	return 0;
}
