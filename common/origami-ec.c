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

#include <core.h>
#include <serial.h>
#include <console.h>
#include <button.h>
#include <led.h>
#include <switch.h>
#include <origami-ec.h>
#include <version.h>
#include <interrupts.h>
#include <config.h>

static void information_print(signed char rc)
{
	if (rc < 0)
		serial_print("Failed to initialize ");
	else
		serial_print("Initialized ");

	serial_printf("%s, version: %s\n", NAME, VERSION);
	serial_printnl(DESCRIPTION);
}

#ifdef CONFIG_CONSOLE
signed char version_command(unsigned char argc, char **argv)
{
	argc;
	argv;

	serial_print(NAME);
	serial_print("\nVersion: \t");
	serial_print(VERSION);
	serial_print("\nPlatform: \t");
	serial_print(CONFIG_PLATFORM_NAME);
	serial_print("\nDevice: \t");
	serial_print(CONFIG_DEVICE_NAME);
	serial_print("\n");

	return 0;
}

signed char reset_command(unsigned char argc, char **argv)
{
	argc;
	argv;

	reset();

	return 0;
}
#endif

void init(void)
{
	signed char rc = 0;

	/* Keep going in case of failure. */

	rc |= serial_init();
	rc |= button_init();
	rc |= led_init();
	rc |= switch_init();

	information_print(rc);

#ifdef CONFIG_CONSOLE
	console_init();
#endif
}

void tasks(void)
{
	signed char rc;

	while (1) {
		rc = button_task();
		if (rc == 0)
			continue;

		rc = switch_task();
		if (rc == 0)
			continue;

		rc = console_task();
		if (rc == 0)
			continue;

		/* TODO: Go to stop mode if host is not up. */
		suspend(SUSPEND_TYPE_IDLE);
	}
}

void main(void)
{
	/* Initialize subsystems. */
	init();

	/* Run tasks. */
	tasks();

	/* Should never hang. */
	hang();
}
