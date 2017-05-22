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

#ifndef _LED_H_
#define _LED_H_

/*
 * Values
 */

enum {
	LED_POWER,
	LED_BATTERY_CHARGING,
	LED_BATTERY_LOW,
	LED_CAPS_LOCK,
	LED_NUM_LOCK,
	LED_COUNT
};

/*
 * API functions
 */

signed char led_enable(unsigned char led, unsigned char enable) __banked;
signed char led_init(void) __banked;

/*
 * Functions
 */

#ifdef CONFIG_CONSOLE
signed char led_command(unsigned char argc, char **argv);
#endif

#endif
