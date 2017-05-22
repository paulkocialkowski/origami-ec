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

#ifndef _BUTTON_H_
#define _BUTTON_H_

/*
 * Values
 */

enum {
	BUTTON_POWER,
	BUTTON_RECOVERY,
	BUTTON_COUNT
};

/*
 * Variables
 */

extern unsigned char button_event;

/*
 * API functions
 */

char button_pressed(unsigned char button) __banked;
signed char button_init(void) __banked;

/*
 * Functions
 */

signed char button_task(void);

#ifdef CONFIG_CONSOLE
signed char button_command(unsigned char argc, char **argv);
#endif

#endif
