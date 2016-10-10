/*
 * Copyright (C) 2016 Paul Kocialkowski <contact@paulk.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <kb9012/gpio.h>
#include <config.h>

#define GPIO_EC_ON						0xd2
#define GPIO_SUSP						0xd5
#define GPIO_LED_CAPS						0x53
#define GPIO_LED_NUM						0x1a
#define GPIO_EC_RSMRST						0xa3
#define GPIO_PBTN_OUT						0xa9
#define GPIO_SYSON						0x56
#define GPIO_VR_ON						0x57
#define GPIO_EC_VGA_EN						0x0c
#define GPIO_BKOFF						0xa8
#define GPIO_VGATE						0x41
#define GPIO_PROCHOT						0xa6
#define GPIO_MAINPWON						0xa7
#define GPIO_USBON						0x4b
#define GPIO_KBRST						0x01
// EC_LID_OUT#

#ifdef CONFIG_CONSOLE
signed char debug_command(unsigned char argc, char **argv)
{
	signed char rc = 0;
	char button;

	if (argc < 2)
		return -1;

	if (strcmp(argv[1], "test") == 0) {
		rc |= gpio_output_data(GPIO_EC_ON, 1);
		rc |= gpio_output_enable(GPIO_EC_ON, 1);

//		rc |= gpio_output_data(GPIO_MAINPWON, 1);
//		rc |= gpio_output_enable(GPIO_MAINPWON, 1);

		rc |= gpio_output_data(GPIO_SUSP, 1);
		rc |= gpio_output_enable(GPIO_SUSP, 1);

		rc |= gpio_output_data(GPIO_LED_CAPS, 0);
		rc |= gpio_output_enable(GPIO_LED_CAPS, 1);

		rc |= gpio_output_data(GPIO_LED_NUM, 0);
		rc |= gpio_output_enable(GPIO_LED_NUM, 1);

		return 0;
	}

	rc |= gpio_input_enable(GPIO_VGATE, 1);

	if (strcmp(argv[1], "on") == 0) {
		rc |= gpio_output_data(GPIO_EC_ON, 1);
		rc |= gpio_output_enable(GPIO_EC_ON, 1);

		rc |= gpio_output_data(GPIO_LED_CAPS, 0);
		rc |= gpio_output_enable(GPIO_LED_CAPS, 1);

		rc |= gpio_output_data(GPIO_LED_NUM, 0);
		rc |= gpio_output_enable(GPIO_LED_NUM, 1);

		rc |= gpio_output_data(GPIO_EC_RSMRST, 1);
		rc |= gpio_output_enable(GPIO_EC_RSMRST, 1);

		rc |= gpio_output_data(GPIO_PROCHOT, 1);
		rc |= gpio_output_enable(GPIO_PROCHOT, 1);

		rc |= gpio_output_data(GPIO_PROCHOT, 1);
		rc |= gpio_output_enable(GPIO_PROCHOT, 1);

		rc |= gpio_output_data(GPIO_MAINPWON, 1);
		rc |= gpio_output_enable(GPIO_MAINPWON, 1);
	} else if (strcmp(argv[1], "btn") == 0) {
		rc |= gpio_output_data(GPIO_PBTN_OUT, 1);

		rc |= gpio_output_data(GPIO_SYSON, 1);
		rc |= gpio_output_enable(GPIO_SYSON, 1);

		rc |= gpio_output_data(GPIO_SUSP, 1);
		rc |= gpio_output_enable(GPIO_SUSP, 1);

		rc |= gpio_output_data(GPIO_KBRST, 1);
		rc |= gpio_output_enable(GPIO_KBRST, 1);

		rc |= gpio_output_data(GPIO_VR_ON, 1);
		rc |= gpio_output_enable(GPIO_VR_ON, 1);

		rc |= gpio_output_data(GPIO_BKOFF, 1);
		rc |= gpio_output_enable(GPIO_BKOFF, 1);

		rc |= gpio_output_data(GPIO_USBON, 0);
		rc |= gpio_output_enable(GPIO_USBON, 1);

		rc |= gpio_output_data(GPIO_EC_VGA_EN, 1);
		rc |= gpio_output_enable(GPIO_EC_VGA_EN, 1);
	} else {
		rc |= gpio_output_data(GPIO_SUSP, 0);
	}


	button = !!gpio_input_data(GPIO_VGATE);
	printf("VGAT GPIO is %d\n", button);


// VGA_GATE#
// USB_ON

// VR_ON?might be needed!

// +LEDVDD
// BKOFF#
// EC_RSMRST#
// PBTN_OUT#
// SYSON
// SUSP#

// KBRST#
// EC_VGA_EN
// 

	return rc;
}
#endif
