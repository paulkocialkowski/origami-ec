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

#include <kb9012/gpio.h>
#include <kb9012/gpwu.h>
#include <g505s/button.h>
#include <button.h>

#pragma codeseg CSEGD

char button_pressed(unsigned char button) __banked
{
	switch (button) {
		case BUTTON_POWER:
			return !gpio_input_data(GPIO_BUTTON_POWER);
		case BUTTON_RECOVERY:
			return !gpio_input_data(GPIO_BUTTON_RECOVERY);
		default:
			return 0;
	}
}

signed char button_init(void) __banked
{
	signed char rc = 0;

	rc |= gpio_input_enable(GPIO_BUTTON_POWER, 1);
	rc |= gpio_input_enable(GPIO_BUTTON_RECOVERY, 1);

	rc |= gpwu_trigger_toggle(GPIO_BUTTON_POWER, 1);
	rc |= gpwu_trigger_toggle(GPIO_BUTTON_RECOVERY, 1);

	rc |= gpwu_event_enable(GPIO_BUTTON_POWER, 1);
	rc |= gpwu_event_enable(GPIO_BUTTON_RECOVERY, 1);

	return rc;
}
