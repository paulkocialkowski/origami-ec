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

#include <ctype.h>
#include <string.h>
#include <serial.h>
#include <console.h>
#include <led.h>
#include <button.h>
#include <switch.h>
#include <debug.h>
#include <origami-ec.h>
#include <config.h>

#ifdef CONFIG_CONSOLE_HISTORY
static char console_history[CONFIG_CONSOLE_HISTORY_COUNT][CONFIG_CONSOLE_BUFFER_LENGTH];
static unsigned char console_history_count;
static unsigned char console_history_index;
static unsigned char console_history_position_offset;
#endif

static char console_buffer[CONFIG_CONSOLE_BUFFER_LENGTH];
static unsigned char console_buffer_count;
static unsigned char console_buffer_index;
static char console_escape;

static char *console_command_string(unsigned char command)
{
	switch (command) {
		case COMMAND_VERSION:
			return "version";
#ifdef CONFIG_CONSOLE_HELP
		case COMMAND_HELP:
			return "help";
#endif
		case COMMAND_LED:
			return "led";
		case COMMAND_BUTTON:
			return "button";
		case COMMAND_SWITCH:
			return "switch";
		case COMMAND_DEBUG:
			return "debug";
		default:
			return NULL;
	}
}

static signed char console_command_call(unsigned char command, unsigned char argc, char **argv)
{
	switch (command) {
		case COMMAND_VERSION:
			return version_command(argc, argv);
#ifdef CONFIG_CONSOLE_HELP
		case COMMAND_HELP:
			return help_command(argc, argv);
#endif
		case COMMAND_LED:
			return led_command(argc, argv);
		case COMMAND_BUTTON:
			return button_command(argc, argv);
		case COMMAND_SWITCH:
			return switch_command(argc, argv);
		case COMMAND_DEBUG:
			return debug_command(argc, argv);
		default:
			return -1;
	}
}

static signed char console_command_handle(unsigned char argc, char **argv)
{
	unsigned char command;
	char *command_string;

	for (command = 0; command < COMMAND_COUNT; command++) {
		command_string = console_command_string(command);

		if (strcmp(command_string, argv[0]) == 0)
			return console_command_call(command, argc, argv);
	}

	return -1;
}

static void console_command_result(signed char rc)
{
	serial_print(CONSOLE_RESULT);

	if (rc < 0)
		serial_print(CONSOLE_RESULT_ERROR);
	else
		serial_print(CONSOLE_RESULT_OK);
}

static void console_buffer_split(unsigned char *argc, char **argv)
{
	unsigned char length;
	unsigned char mark = 0;
	unsigned char i;

	*argc = 0;

	length = strlen(console_buffer) + 1;

	for (i = 0; i < length; i++) {
		if (isspace(console_buffer[i]) || console_buffer[i] == '\0') {
			if (mark == i) {
				mark++;
			} else {
				console_buffer[i] = '\0';
				argv[*argc] = &console_buffer[mark];
				(*argc)++;

				if (*argc >= CONFIG_CONSOLE_COMMAND_ARGS_LIMIT)
					break;

				mark = i + 1;
			}
		}
	}
}

static void console_buffer_print(unsigned char count)
{
	unsigned char i;

	for (i = 0; i < count; i++)
		serial_putc(console_buffer[console_buffer_index + i]);
}

static void console_repeat_print(const char c, unsigned char count)
{
	while (count) {
		serial_putc(c);
		count--;
	}
}

static void console_reset(void)
{
#ifdef CONFIG_CONSOLE_HISTORY
	console_history_position_offset = 0;
#endif

	console_buffer_count = 0;
	console_buffer_index = 0;

	console_escape = 0;

	serial_print("\n");
	serial_print(CONSOLE_PROMPT);
}

static void console_move(char key, unsigned char count)
{
	serial_printf("\x1b[%d%c", count, key);
}

static void console_move_left(void)
{
	if (console_buffer_index == 0)
		return;

	console_move(KEY_LEFT, 1);
	console_buffer_index--;
}

static void console_move_right(void)
{
	if (console_buffer_index == console_buffer_count)
		return;

	console_move(KEY_RIGHT, 1);
	console_buffer_index++;
}

static void console_move_home(void)
{
	if (console_buffer_index == 0)
		return;

	console_move(KEY_LEFT, console_buffer_index);
	console_buffer_index = 0;
}

static void console_move_end(void)
{
	unsigned char leftover;

	leftover = console_buffer_count - console_buffer_index;
	if (leftover == 0)
		return;

	console_move(KEY_RIGHT, leftover);
	console_buffer_index += leftover;
}

static void console_del(void)
{
	unsigned char leftover;

	leftover = console_buffer_count - console_buffer_index;
	if (leftover == 0)
		return;

	memmove(console_buffer + console_buffer_index, console_buffer + console_buffer_index + 1, leftover);
	console_buffer[console_buffer_index + leftover - 1] = ' ';

	console_buffer_print(leftover);
	console_repeat_print('\b', leftover);

	console_buffer_count--;
}

static void console_backspace(void)
{
	unsigned char leftover;

	if (console_buffer_index == 0)
		return;

	leftover = console_buffer_count - console_buffer_index;
	if (leftover != 0)
		memmove(console_buffer + console_buffer_index - 1, console_buffer + console_buffer_index, leftover);

	console_buffer_count--;
	console_buffer_index--;

	console_buffer[console_buffer_index + leftover] = ' ';

	console_move(KEY_LEFT, 1);
	console_buffer_print(leftover + 1);
	console_repeat_print('\b', leftover + 1);
}

#ifdef CONFIG_CONSOLE_HISTORY
static void console_history_save(void)
{
	strcpy(console_history[console_history_index], console_buffer);

	console_history_index++;
	if (console_history_index == CONFIG_CONSOLE_HISTORY_COUNT)
		console_history_index = 0;

	if (console_history_count != CONFIG_CONSOLE_HISTORY_COUNT)
		console_history_count++;
}

static void console_history_prev(void)
{
	unsigned char position;
	unsigned char leftover;
	unsigned char count;

	console_move_home();

	if (console_history_position_offset >= console_history_count)
		return;

	console_history_position_offset++;

	count = console_buffer_count;

	if (console_history_position_offset > console_history_index)
		position = console_history_count + console_history_index - console_history_position_offset;
	else
		position = console_history_index - console_history_position_offset;

	strcpy(console_buffer, console_history[position]);

	console_buffer_count = strlen(console_buffer);
	console_buffer_index = 0;

	console_buffer_print(console_buffer_count);
	console_buffer_index = console_buffer_count;

	if (count > console_buffer_count) {
		leftover = count - console_buffer_count;

		console_repeat_print(' ', leftover);
		console_repeat_print('\b', leftover);
	}
}

static void console_history_next(void)
{
	unsigned char position;
	unsigned char leftover;
	unsigned char count;

	console_move_home();

	if (console_history_position_offset == 0) {
		if (console_buffer_count > 0) {
			console_repeat_print(' ', console_buffer_count);
			console_move(KEY_LEFT, console_buffer_count);
		}

		console_buffer_count = 0;
		console_buffer_index = 0;

		return;
	}

	console_history_position_offset--;

	count = console_buffer_count;

	if (console_history_position_offset > console_history_index)
		position = console_history_count + console_history_index - console_history_position_offset;
	else
		position = console_history_index - console_history_position_offset;

	strcpy(console_buffer, console_history[position]);

	console_buffer_count = strlen(console_buffer);
	console_buffer_index = 0;

	console_buffer_print(console_buffer_count);
	console_buffer_index = console_buffer_count;

	if (count > console_buffer_count) {
		leftover = count - console_buffer_count;

		console_repeat_print(' ', leftover);
		console_repeat_print('\b', leftover);
	}
}
#endif

static char console_escape_handle(char c)
{
	if (console_escape == 0) {
		if (c != KEY_ESCAPE)
			goto error;

		console_escape = c;
		goto complete;
	} else if (console_escape == KEY_ESCAPE) {
		if (c != '[' && c != '0')
			goto escape_error;

		console_escape = c;
		goto complete;
	} else if (console_escape == '[') {
		switch (c) {
			case '1':
				console_escape = c;
				goto complete;
			case '3':
				console_escape = c;
				goto complete;
#ifdef CONFIG_CONSOLE_HISTORY
			case KEY_UP:
				console_history_prev();
				goto escape_complete;
			case KEY_DOWN:
				console_history_next();
				goto escape_complete;
#endif
			case KEY_RIGHT:
				console_move_right();
				goto escape_complete;
			case KEY_LEFT:
				console_move_left();
				goto escape_complete;
			case KEY_END:
				console_move_end();
				goto escape_complete;
			default:
				goto escape_error;
		}
	} else if (console_escape == '1') {
		if (c != '~')
			goto escape_error;

		console_move_home();
		goto escape_complete;
	} else if (console_escape == '3') {
		if (c != '~')
			goto escape_error;

		console_del();
		goto escape_complete;
	}

escape_error:
	console_escape = 0;

error:
	return c;

escape_complete:
	console_escape = 0;

complete:
	return 0;
}

static char console_control_handle(char c)
{
	char *argv[CONFIG_CONSOLE_COMMAND_ARGS_LIMIT];
	unsigned char argc;
	signed char rc;

	if (c == '\r') {
		console_buffer[console_buffer_count] = '\0';

#ifdef CONFIG_CONSOLE_HISTORY
		if (console_buffer_count > 0)
			console_history_save();
#endif

		serial_print("\n");

		console_buffer_split(&argc, argv);

		if (argc > 0) {
			rc = console_command_handle(argc, argv);
			console_command_result(rc);
		}

		console_reset();

		return 0;
	} else if (c == KEY_BACKSPACE || c == '\b') {
		console_backspace();
		return 0;
	} else if (c == KEY_END_OF_TEXT) {
		console_reset();
	}

	return c;
}

static char console_text_handle(char c)
{
	unsigned char leftover;

	if (!isprint(c))
		return c;

	/* Discard character on buffer overflow, with room for '\0'. */
	if (console_buffer_count >= (CONFIG_CONSOLE_BUFFER_LENGTH - 1))
		goto complete;

	leftover = console_buffer_count - console_buffer_index;

	if (leftover != 0)
		memmove(console_buffer + console_buffer_index + 1, console_buffer + console_buffer_index, leftover);

	console_buffer[console_buffer_index] = c;
	console_buffer_print(leftover + 1);

	if (leftover != 0)
		console_repeat_print('\b', leftover);

	console_buffer_count++;
	console_buffer_index++;

complete:
	return 0;
}

signed char console_task(void)
{
	char c;

	if (!serial_recv_available())
		return -1;

	c = serial_recv();

	c = console_escape_handle(c);
	if (!c)
		goto complete;

	c = console_control_handle(c);
	if (!c)
		goto complete;

	console_text_handle(c);

complete:
	return 0;
}

signed char console_init(void)
{
#ifdef CONFIG_CONSOLE_HISTORY
	console_history_count = 0;
	console_history_index = 0;
#endif

	console_reset();

	return 0;
}

#ifdef CONFIG_CONSOLE_HELP
signed char help_command(unsigned char argc, char **argv)
{
	unsigned char command;
	char *command_string;

	argc;
	argv;

	serial_print("List of available commands:\n");

	for (command = 0; command < COMMAND_COUNT; command++) {
		command_string = console_command_string(command);
		serial_printnl(command_string);
	}

	serial_print("\nFor command-specific help, try: [command] help\n");

	return 0;
}
#endif
