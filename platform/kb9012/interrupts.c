/*
 * Copyright (C) 2015-2018 Paul Kocialkowski <contact@paulk.fr>
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
#include <kb9012/watchdog.h>
#include <kb9012/serial.h>
#include <core.h>

void serial_interrupt(void) __interrupt(4)
{
	unsigned char index;

	if (TI) {
		TI = 0;

		if (serial_send_count > 0) {
			SBUF = serial_send_buffer[serial_send_start];
			serial_send_count--;

			serial_send_start++;
			if (serial_send_start == sizeof(serial_send_buffer))
				serial_send_start = 0;
		} else {
			serial_send_busy = 0;
		}
	}

	if (RI) {
		if (serial_recv_count == sizeof(serial_recv_buffer)) {
			RI = 0;
			return;
		}

		index = (serial_recv_start + serial_recv_count);
		if (index >= sizeof(serial_recv_buffer))
			index -= sizeof(serial_recv_buffer);

		serial_recv_buffer[index] = SBUF;
		serial_recv_count++;

		RI = 0;
	}
}

void watchdog_interrupt(void) __interrupt(8)
{
	unsigned char value;

	value = register_read(WDTPF);
	if (value & WDTPF_INTERRUPT_FLAG || value & WDTPF_RESET_FLAG)
		register_write(WDTPF, WDTPF_INTERRUPT_FLAG | WDTPF_RESET_FLAG);
}
