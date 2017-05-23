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

#define SEGMENT_HINT_LED
#include <segment.h>
#include <string.h>
#include <serial.h>
#include <led.h>
#include <config.h>

#ifdef CONFIG_CONSOLE
signed char led_command(unsigned char argc, char **argv)
{
	unsigned char led = 0;
	unsigned char i;

#ifdef CONFIG_CONSOLE_HELP
	if (strcmp(argv[1], "help") == 0) {
		serial_print("Usage: led [enable|disable] [power|battery_charging|battery_low|caps_lock|num_lock]\n");
		return 0;
	}
#endif

	if (argc < 3)
		return -1;

	for (i = 2; i < argc; i++) {
		if (strcmp(argv[i], "power") == 0)
			led = LED_POWER;
		else if (strcmp(argv[i], "battery_charging") == 0)
			led = LED_BATTERY_CHARGING;
		else if (strcmp(argv[i], "battery_low") == 0)
			led = LED_BATTERY_LOW;
		else if (strcmp(argv[i], "caps_lock") == 0)
			led = LED_CAPS_LOCK;
		else if (strcmp(argv[i], "num_lock") == 0)
			led = LED_NUM_LOCK;
		else
			return -1;
	}

	if (strcmp(argv[1], "enable") == 0)
		return led_enable(led, 1);
	else if (strcmp(argv[1], "disable") == 0)
		return led_enable(led, 0);

	return -1;
}
#endif
