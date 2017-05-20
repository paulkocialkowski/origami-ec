/*
 * Copyright (C) 2016-2017 Paul Kocialkowski <contact@paulk.fr>
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

#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <kb9012/interrupts.h>

/*
 * Functions
 */

void gpwu_interrupt_2(void) __interrupt(26);
void gpwu_interrupt_5(void) __interrupt(30);

#endif
