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

#ifndef _KB9012_WATCHDOG_H_
#define _KB9012_WATCHDOG_H_

#include <segment.h>

/*
 * Macros
 */

/* 32 ms per period: high 10 bits of 20 bits counter with 32.768 kHz clock. */
#define WDT_COUNTER(period) \
	((period + ((1 << WDT_PERIOD_SHIFT) - 1)) >> WDT_PERIOD_SHIFT)

/*
 * Registers
 */

#define WDTCFG							0xfe80
#define WDTPF							0xfe81
#define WDT							0xfe82

#define LEDCFG							0xfe83

/*
 * Values
 */

#define WDTCFG_SOURCE_INTERNAL					(1 << 7)
#define WDTCFG_INTERRUPT_ENABLE					(1 << 1)
#define WDTCFG_RESET_ENABLE					(1 << 0)

#define WDTPF_INTERRUPT_FLAG					(1 << 1)
#define WDTPF_RESET_FLAG					(1 << 0)

#define WDT_PERIOD_SHIFT					5
#define WDT_SHIFT						2

#define LEDCFG_WDT_SHIFT					6
#define LEDCFG_WDT_MASK						0x03

/*
 * Functions
 */

void watchdog_init(void) __segment_hint_platform;

#endif
