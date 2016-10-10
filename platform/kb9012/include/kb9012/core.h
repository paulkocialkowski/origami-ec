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

#ifndef _KB9012_CORE_H_
#define _KB9012_CORE_H_

/*
 * Macros
 */

#define EC_CLKCFG_CLOCK(clock) \
	(((clock / EC_CLKCFG_CLOCK_BASE) - 1) << 2)

/*
 * Registers
 */

__sfr __at(0x86) PCON2;

#define EC_PMUCFG						0xff0c
#define EC_CLKCFG						0xff0d

/*
 * Values
 */

#define PCON2_EXTERNAL_MODULES_ENABLE				(1 << 4)

#define IE_ENABLE_ALL						(1 << 7)
#define IE_ENABLE_SERIAL					(1 << 4)

#define EC_PMUCFG_STOP						(1 << 7)
#define EC_PMUCFG_IDLE						(1 << 6)

#define EC_CLKCFG_CLOCK_BASE					8000000

#endif
