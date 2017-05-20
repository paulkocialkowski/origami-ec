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

#include <string.h>
#include <serial.h>
#include <button.h>
#include <led.h>
#include <config.h>

unsigned char button_event = 1;

signed char button_task(void)
{
	if (!button_event)
		return -1;

	button_event = 0;

	if (button_pressed(BUTTON_POWER))
		led_enable(LED_POWER, 1);
	else
		led_enable(LED_POWER, 0);

	if (button_pressed(BUTTON_RECOVERY))
		led_enable(LED_BATTERY_LOW, 1);
	else
		led_enable(LED_BATTERY_LOW, 0);

	return 0;
}

#ifdef CONFIG_CONSOLE
signed char button_command(unsigned char argc, char **argv)
{
	unsigned char button = 0;
	unsigned char pressed;
	unsigned char i;

#ifdef CONFIG_CONSOLE_HELP
	if (strcmp(argv[1], "help") == 0) {
		serial_print("Usage: button [power|recovery]\n");
		return 0;
	}
#endif

	if (argc < 2)
		return -1;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "power") == 0)
			button = BUTTON_POWER;
		else if (strcmp(argv[i], "recovery") == 0)
			button = BUTTON_RECOVERY;
		else
			return -1;
	}

	pressed = button_pressed(button);
	if (pressed)
		serial_print("Button is pressed\n");
	else
		serial_print("Button is not pressed\n");

	return 0;
}
#endif
