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

#ifndef _CORE_H_
#define _CORE_H_

/*
 * Macros
 */

#define value_mask_clear(value, mask, offset) \
	(value & ~(mask << offset))

#define register_read(address) \
	*((volatile unsigned char __xdata *) address)

#define register_write(address, value) \
	*((volatile unsigned char __xdata *) address) = value

#define hang() \
	while (1)

#define nop() \
	__asm__ ("nop")

/*
 * Values
 */

enum {
	SUSPEND_TYPE_IDLE,
	SUSPEND_TYPE_STOP
};

/*
 * API functions
 */

void suspend(unsigned char type);
void reset(void);

#endif
