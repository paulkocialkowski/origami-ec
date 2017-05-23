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

#define SEGMENT_HINT_SERIAL
#include <segment.h>
#include <serial.h>

void putchar(char c)
{
	if (c == '\n')
		serial_send('\r');

	serial_send(c);
}

signed char serial_putc(char c)
{
	return serial_send(c);
}

signed char serial_puts(const char *string)
{
	signed char rc = 0;

	while (*string != '\0') {
		rc |= serial_send(*string);
		string++;
	}

	return rc;
}

signed char serial_print(const char *string)
{
	signed char rc = 0;

	while (*string != '\0') {
		if (*string == '\n')
			rc |= serial_send('\r');

		rc |= serial_send(*string);
		string++;
	}

	return rc;
}

signed char serial_printnl(const char *string)
{
	signed char rc = 0;

	rc |= serial_print(string);
	rc |= serial_send('\r');
	rc |= serial_send('\n');

	return rc;
}
