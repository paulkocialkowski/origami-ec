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

#ifndef _KB9012_GPWU_H_
#define _KB9012_GPWU_H_

/*
 * Registers
 */

__sfr __at(0xb0) P3IE;

#define GPWU_EN_BASE						0xff30
#define GPWU_PF_BASE						0xff40
#define GPWU_PS_BASE						0xff50
#define GPWU_EL_BASE						0xff60
#define GPWU_CH_BASE						0xff70

/*
 * Values
 */

#define GPWU_OFFSET_SHIFT					3
#define GPWU_OFFSET_SPAN					0x0f
#define GPWU_OFFSET_MASK_A0					(1 << 0)
#define GPWU_OFFSET_A0						0x0d
#define GPWU_OFFSET_D0						0x0c

#define GPWU_BIT_MASK						0x07

#define GPWU_INTERRUPT_INDEX_BASE				1
#define GPWU_INTERRUPT_INDEX_SHIFT				4
#define GPWU_INTERRUPT_INDEX_A0					4
#define GPWU_INTERRUPT_INDEX_D0					5

/*
 * Functions
 */

signed char gpwu_event_enable(unsigned char gpio, unsigned char enable);
signed char gpwu_event_pending(unsigned char gpio);
signed char gpwu_event_clear(unsigned char gpio);
signed char gpwu_polarity_selection(unsigned char gpio, unsigned char high);
signed char gpwu_trigger_selection(unsigned char gpio, unsigned char level);
signed char gpwu_trigger_toggle(unsigned char gpio, unsigned char enable);

#endif
