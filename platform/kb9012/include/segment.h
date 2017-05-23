/*
 * Copyright (C) 2017 Paul Kocialkowski <contact@paulk.fr>
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

#ifndef _SEGMENT_H_
#define _SEGMENT_H_

/*
 * Macros
 */

/* CSEG common segment */

#if defined(SEGMENT_HINT_CONSOLE) || defined(SEGMENT_HINT_SERIAL)
#define __segment_hint_console
#define __segment_hint_serial
#pragma codeseg CSEG1
#else
#define __segment_hint_console __banked
#define __segment_hint_serial __banked
#endif

#define __segment_hint_core

/* CSEG2 switched segment */

#if defined(SEGMENT_HINT_PLATFORM) || defined(SEGMENT_HINT_BUTTON) || defined(SEGMENT_HINT_LED) || defined(SEGMENT_HINT_SWITCH)
#define __segment_hint_platform
#define __segment_hint_button
#define __segment_hint_led
#define __segment_hint_switch
#pragma codeseg CSEG2
#else
#define __segment_hint_platform __banked
#define __segment_hint_button __banked
#define __segment_hint_led __banked
#define __segment_hint_switch __banked
#endif

#endif
