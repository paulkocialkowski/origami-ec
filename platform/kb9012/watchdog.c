/*
 * Copyright (C) 2018 Paul Kocialkowski <contact@paulk.fr>
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

#define SEGMENT_HINT_PLATFORM
#include <segment.h>
#include <kb9012/core.h>
#include <kb9012/watchdog.h>
#include <core.h>
#include <config.h>

void watchdog_init(void)
{
	unsigned char value;

	/* High 8 bits. */
	value = WDT_COUNTER(CONFIG_WATCHDOG_PERIOD) >> WDT_SHIFT;
	register_write(WDT, value);

	/* Low 2 bits. */
	value = register_read(LEDCFG);
	value = value_mask_clear(value, LEDCFG_WDT_MASK, LEDCFG_WDT_SHIFT);
	value |= WDT_COUNTER(CONFIG_WATCHDOG_PERIOD) << LEDCFG_WDT_SHIFT;
	register_write(LEDCFG, value);

	value = WDTCFG_RESET_ENABLE;
	value |= WDTCFG_SOURCE_INTERNAL;
	value |= WDTCFG_INTERRUPT_ENABLE;

	register_write(WDTCFG, value);

	value = register_read(ECHA);
	value &= ~ECHA_WRITE_PROTECT;
	register_write(ECHA, value);

	value = register_read(PXCFG);
	value |= PXCFG_WDT_RESET_GPIO;
	register_write(PXCFG, value);

	value = register_read(ECHA);
	value |= ECHA_WRITE_PROTECT;
	register_write(ECHA, value);

	P0IE |= P0IE_WATCHDOG;
}
