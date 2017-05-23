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

#define SEGMENT_HINT_CORE
#include <segment.h>
#include <8051.h>
#include <kb9012/core.h>
#include <kb9012/gpwu.h>
#include <kb9012/serial.h>
#include <g505s/button.h>
#include <core.h>
#include <config.h>
#include <serial.h>

void suspend(unsigned char type)
{
	unsigned char value;

	gpwu_suspend();
	serial_suspend();

	/* Clock configuration to idle. */
	value = register_read(EC_CLKCFG);
	value = value_mask_clear(value, EC_CLKCFG_CLOCK_MASK, EC_CLKCFG_CLOCK_SHIFT);
	value |= EC_CLKCFG_CLOCK(CONFIG_CLOCK_IDLE);
	register_write(EC_CLKCFG, value);

	if (type == SUSPEND_TYPE_STOP) {
		value = register_read(EC_PMUCFG);
		value |= EC_PMUCFG_STOP;
		register_write(EC_PMUCFG, value);
	} else {
		value = register_read(EC_PMUCFG);
		value |= EC_PMUCFG_IDLE;
		register_write(EC_PMUCFG, value);
	}

	/* Clock configuration to normal. */
	value = register_read(EC_CLKCFG);
	value = value_mask_clear(value, EC_CLKCFG_CLOCK_MASK, EC_CLKCFG_CLOCK_SHIFT);
	value |= EC_CLKCFG_CLOCK(CONFIG_CLOCK);
	register_write(EC_CLKCFG, value);

	serial_resume();
	gpwu_resume();
}

unsigned char _sdcc_external_startup(void)
{
	unsigned char value;

	/* Access to external modules. */
	PCON2 |= PCON2_EXTERNAL_MODULES_ENABLE;

	/* Clock configuration. */
	value = register_read(EC_CLKCFG);
	value = value_mask_clear(value, EC_CLKCFG_CLOCK_MASK, EC_CLKCFG_CLOCK_SHIFT);
	value |= EC_CLKCFG_CLOCK(CONFIG_CLOCK) | EC_CLKCFG_FLASH_CLOCK_FULL;
	register_write(EC_CLKCFG, value);

	/* Idle suspend wakeup. */
	value = register_read(EC_PMUCFG);
	value = EC_PMUCFG_STOP_WAKEUP_GPWU | EC_PMUCFG_IDLE_WAKEUP_INTERRUPT;
	register_write(EC_PMUCFG, value);

	/* Interrupts enable. */
	IE |= IE_ALL_ENABLE;

	return 0;
}
