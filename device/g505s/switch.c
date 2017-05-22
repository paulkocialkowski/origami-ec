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

#include <kb9012/gpio.h>
#include <kb9012/gpwu.h>
#include <g505s/switch.h>
#include <switch.h>

#pragma codeseg CSEGD

signed char switch_enable(unsigned char switch_output, unsigned char enable) __banked
{
	switch_output;
	enable;

	return -1;
}

unsigned char switch_status(unsigned char switch_input) __banked
{
	switch (switch_input) {
		case SWITCH_INPUT_AC:
			return gpio_input_data(GPIO_SWITCH_INPUT_AC);
		case SWITCH_INPUT_LID:
			return !gpio_input_data(GPIO_SWITCH_INPUT_LID);
		default:
			return 0;
	}
}

signed char switch_init(void) __banked
{
	signed char rc = 0;

	rc |= gpio_input_enable(GPIO_SWITCH_INPUT_AC, 1);
	rc |= gpio_input_enable(GPIO_SWITCH_INPUT_LID, 1);

	rc |= gpwu_trigger_toggle(GPIO_SWITCH_INPUT_AC, 1);
	rc |= gpwu_trigger_toggle(GPIO_SWITCH_INPUT_LID, 1);

	rc |= gpwu_event_enable(GPIO_SWITCH_INPUT_AC, 1);
	rc |= gpwu_event_enable(GPIO_SWITCH_INPUT_LID, 1);

	return rc;
}
