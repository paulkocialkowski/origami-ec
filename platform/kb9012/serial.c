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
#include <kb9012/serial.h>
#include <kb9012/core.h>
#include <kb9012/gpio.h>
#include <serial.h>
#include <config.h>

static char serial_send_buffer[8];
static unsigned char serial_send_count;
static unsigned char serial_send_start;
static unsigned char serial_send_busy;

static char serial_recv_buffer[8];
static unsigned char serial_recv_count;
static unsigned char serial_recv_start;

static inline void serial_interrupt_enable(void)
{
	IE |= IE_SERIAL_ENABLE;
}

static inline void serial_interrupt_disable(void)
{
	IE &= ~IE_SERIAL_ENABLE;
}

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
		RI = 0;

		if (serial_recv_count == sizeof(serial_recv_buffer))
			return;

		index = (serial_recv_start + serial_recv_count);
		if (index >= sizeof(serial_recv_buffer))
			index -= sizeof(serial_recv_buffer);

		serial_recv_buffer[index] = SBUF;
		serial_recv_count++;
	}
}

signed char serial_send(char c)
{
	unsigned char index;

	while (!(sizeof(serial_send_buffer) - serial_send_count)) ;

	/* Let the buffer clear out on new line. */
	if (c == '\n')
		while (serial_send_busy) ;

	serial_interrupt_disable();

	if (serial_send_busy) {
		/* Buffer overflow. */
		if (serial_send_count == sizeof(serial_send_buffer))
			goto complete;

		index = (serial_send_start + serial_send_count);
		if (index >= sizeof(serial_send_buffer))
			index -= sizeof(serial_send_buffer);

		serial_send_buffer[index] = c;
		serial_send_count++;
	} else {
		serial_send_busy = 1;
		SBUF = c;
	}

complete:
	serial_interrupt_enable();

	return 0;
}

char serial_recv(void)
{
	char c;

	while (!serial_recv_count) ;

	serial_interrupt_disable();

	c = serial_recv_buffer[serial_recv_start];
	serial_recv_count--;

	serial_recv_start++;
	if (serial_recv_start == sizeof(serial_recv_buffer))
		serial_recv_start = 0;

	serial_interrupt_enable();

	return c;
}

unsigned char serial_send_available(void)
{
	return (sizeof(serial_send_buffer) - serial_send_count);
}

unsigned char serial_recv_available(void)
{
	return serial_recv_count;
}

void serial_suspend(void)
{
	while (serial_send_busy) ;

	SCON2 = (SCON_COUNTER(CONFIG_SERIAL_BAUDRATE, CONFIG_CLOCK_IDLE) >> 8) & 0xff;
	SCON3 = SCON_COUNTER(CONFIG_SERIAL_BAUDRATE, CONFIG_CLOCK_IDLE) & 0xff;
}

void serial_resume(void)
{
	SCON2 = (SCON_COUNTER(CONFIG_SERIAL_BAUDRATE, CONFIG_CLOCK) >> 8) & 0xff;
	SCON3 = SCON_COUNTER(CONFIG_SERIAL_BAUDRATE, CONFIG_CLOCK) & 0xff;
}

signed char serial_init(void)
{
	serial_send_count = 0;
	serial_send_start = 0;
	serial_send_busy = 0;

	serial_recv_count = 0;
	serial_recv_start = 0;

	SCON2 = (SCON_COUNTER(CONFIG_SERIAL_BAUDRATE, CONFIG_CLOCK) >> 8) & 0xff;
	SCON3 = SCON_COUNTER(CONFIG_SERIAL_BAUDRATE, CONFIG_CLOCK) & 0xff;

	gpio_function_selection(GPIO_E51TXD, 1);
	gpio_output_enable(GPIO_E51TXD, 1);
	gpio_input_enable(GPIO_E51RXD, 1);

	serial_interrupt_enable();

	return 0;
}
