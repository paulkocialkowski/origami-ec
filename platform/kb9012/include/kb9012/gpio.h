/*
 * Copyright (C) 2015-2016 Paul Kocialkowski <contact@paulk.fr>
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

#ifndef _KB9012_GPIO_H_
#define _KB9012_GPIO_H_

/*
 * Registers
 */

#define GPIO_FS_BASE						0xfc00
#define GPIO_OE_BASE						0xfc10
#define GPIO_D_BASE						0xfc20
#define GPIO_IN_BASE						0xfc30
#define GPIO_PU_BASE						0xfc40
#define GPIO_OD_BASE						0xfc50
#define GPIO_IE_BASE						0xfc60

#define GPIO_MISC						0xfc70
#define GPIO_MISC2						0xfc71
#define GPIO_LED						0xfc74
#define GPIO_FDA						0xfc75

/*
 * Values
 */

#define GPIO_BASE_MASK						0xc0
#define GPIO_BASE_A0						0x80
#define GPIO_BASE_D0						0xc0

#define GPIO_OFFSET_SHIFT					3
#define GPIO_OFFSET_SPAN					0x0f
#define GPIO_OFFSET_MASK_A0					(1 << 0)
#define GPIO_OFFSET_A0						0x0c
#define GPIO_OFFSET_D0						0x0f

#define GPIO_BIT_MASK						0x07

#define GPIO_E51TXD						0x16
#define GPIO_E51RXD						0x17

/*
 * Functions
 */

signed char gpio_function_selection(unsigned char gpio, unsigned char alternative);
signed char gpio_output_enable(unsigned char gpio, unsigned char enable);
signed char gpio_input_enable(unsigned char gpio, unsigned char enable);
signed char gpio_output_data(unsigned char gpio, unsigned char data);
signed char gpio_input_data(unsigned char gpio);
signed char gpio_pull_up(unsigned char gpio, unsigned char enable);
signed char gpio_open_drain(unsigned char gpio, unsigned char enable);

#endif
