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

#include <8051.h>
#include <kb9012/core.h>
#include <core.h>
#include <config.h>


void suspend(unsigned char type)
{
	unsigned char value;

	if (type == SUSPEND_TYPE_STOP) {
		value = register_read(EC_PMUCFG);
		value |= EC_PMUCFG_STOP;
		register_write(EC_PMUCFG, value);
	} else {
		value = register_read(EC_PMUCFG);
		value |= EC_PMUCFG_IDLE;
		register_write(EC_PMUCFG, value);
	}
}

unsigned char _sdcc_external_startup(void)
{
	unsigned char value;

	/* Access to external modules */
	PCON2 |= PCON2_EXTERNAL_MODULES_ENABLE;

	/* Clock configuration */
	value = register_read(EC_CLKCFG);
	value |= EC_CLKCFG_CLOCK(CONFIG_CLOCK);
	register_write(EC_CLKCFG, value);

	/* Interrupts enable */
	IE |= IE_ENABLE_ALL;

	return 0;
}
