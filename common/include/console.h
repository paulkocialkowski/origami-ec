/*
 * Copyright (C) 2015-2017 Paul Kocialkowski <contact@paulk.fr>
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

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <segment.h>

/*
 * Values
 */

#define CONSOLE_PROMPT				"> "
#define CONSOLE_RESULT				"< "
#define CONSOLE_RESULT_ERROR			"ERROR\n"
#define CONSOLE_RESULT_OK			"OK\n"

#define KEY_END_OF_TEXT						0x03
#define KEY_ESCAPE						0x1b
#define KEY_BACKSPACE						0x7f
#define KEY_UP							'A'
#define KEY_DOWN						'B'
#define KEY_RIGHT						'C'
#define KEY_LEFT						'D'
#define KEY_END							'F'

enum {
	COMMAND_VERSION,
#ifdef CONFIG_CONSOLE_HELP
	COMMAND_HELP,
#endif
	COMMAND_LED,
	COMMAND_BUTTON,
	COMMAND_SWITCH,
	COMMAND_DEBUG,
	COMMAND_RESET,
	COMMAND_COUNT
};

/*
 * Functions
 */

signed char console_task(void) __segment_hint_console;
signed char console_init(void) __segment_hint_console;

#ifdef CONFIG_CONSOLE_HELP
signed char help_command(unsigned char argc, char **argv) __segment_hint_console;
#endif

#endif
