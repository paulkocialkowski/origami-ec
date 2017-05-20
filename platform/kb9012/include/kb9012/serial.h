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

#ifndef _KB9012_SERIAL_H_
#define _KB9012_SERIAL_H_

/*
 * Macros
 */

/*
 * Calculating the SCON counter value directly from the baud rate is not easy.
 * However, with a reference, it was found that:
 * SCON_COUNTER' = (baud / baud') * SCON_COUNTER * (CLOCK / CLOCK')
 *
 * The reference used is: 32768 for 1155 bauds at 32 MHz,
 * the equation was also simplified (32768 / 32 = 1024), to avoid overflow.
 */
#define SCON_COUNTER(baud, clock) \
	(((clock / 1000000) * 1024 * 1155) / baud)

/*
 * Registers
 */

__sfr __at(0x9a) SCON2;
__sfr __at(0x9b) SCON3;

/*
 * Values
 */

#define SCON_9BIT_SERIAL_PORT_TIMER1				(3 << 6)
#define SCON_9BIT_SERIAL_PORT_OSC				(2 << 6)
#define SCON_8BIT_SERIAL_PORT_TIMER1				(1 << 6)
#define SCON_8BIT_SHIFT_REGISTER_OSC				(0 << 6)
#define SCON_RECEIVE_ENABLE					(1 << 4)

#endif
