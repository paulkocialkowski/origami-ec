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

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdio.h>

/*
 * Macros
 */

#define serial_printf				printf

/*
 * API functions
 */

signed char serial_send(char c);
char serial_recv(void);
unsigned char serial_send_available(void);
unsigned char serial_recv_available(void);
void serial_suspend(void);
void serial_resume(void);
signed char serial_init(void);

/*
 * Functions
 */

void putchar(char c);
signed char serial_putc(char c);
signed char serial_puts(const char *string);
signed char serial_print(const char *string);
signed char serial_printnl(const char *string);

#endif
