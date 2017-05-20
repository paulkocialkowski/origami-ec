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

#include <string.h>
#include <serial.h>
#include <switch.h>
#include <led.h>
#include <config.h>

unsigned char switch_event = 1;

signed char switch_task(void)
{
	if (!switch_event)
		return -1;

	switch_event = 0;

	if (switch_status(SWITCH_INPUT_LID))
		led_enable(LED_BATTERY_CHARGING, 1);
	else
		led_enable(LED_BATTERY_CHARGING, 0);

	return 0;
}

#ifdef CONFIG_CONSOLE
signed char switch_command(unsigned char argc, char **argv)
{
	unsigned char switch_input = 0;
	unsigned char switch_output = 0;
	unsigned char status;
	unsigned char i;

#ifdef CONFIG_CONSOLE_HELP
	if (strcmp(argv[1], "help") == 0) {
		serial_print("Usage: switch [enable|disable|status] [ac|lid]\n");
		return 0;
	}
#endif

	if (argc < 3)
		return -1;

	for (i = 2; i < argc; i++) {
		if (strcmp(argv[i], "ac") == 0)
			switch_input = SWITCH_INPUT_AC;
		else if (strcmp(argv[i], "lid") == 0)
			switch_input = SWITCH_INPUT_LID;
		else
			return -1;
	}

	if (strcmp(argv[1], "enable") == 0)
		return switch_enable(switch_output, 1);
	else if (strcmp(argv[1], "disable") == 0)
		return switch_enable(switch_output, 0);

	if (strcmp(argv[1], "status") == 0) {
		status = switch_status(switch_input);
		if (status)
			serial_print("Switch is active\n");
		else
			serial_print("Switch is inactive\n");
	}

	return 0;
}
#endif
