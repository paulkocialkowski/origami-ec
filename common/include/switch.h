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

#ifndef _SWITCH_H_
#define _SWITCH_H_

/*
 * Values
 */

enum {
	SWITCH_INPUT_AC,
	SWITCH_INPUT_LID,
	SWITCH_INPUT_COUNT
};

/*
 * Variables
 */

extern unsigned char switch_event;

/*
 * API functions
 */

signed char switch_enable(unsigned char switch_output, unsigned char enable);
unsigned char switch_status(unsigned char switch_input);
signed char switch_init(void);

/*
 * Functions
 */

signed char switch_task(void);

#ifdef CONFIG_CONSOLE
signed char switch_command(unsigned char argc, char **argv);
#endif

#endif
