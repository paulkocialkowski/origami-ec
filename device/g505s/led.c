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
#include <g505s/led.h>
#include <led.h>

#pragma codeseg CSEGD

signed char led_enable(unsigned char led, unsigned char enable) __banked
{
	/* LED GPIO outputs are active low. */
	unsigned char data = !enable;

	switch (led) {
		case LED_POWER:
			return gpio_output_data(GPIO_LED_POWER, data);
		case LED_BATTERY_CHARGING:
			return gpio_output_data(GPIO_LED_BATTERY_CHARGING, data);
		case LED_BATTERY_LOW:
			return gpio_output_data(GPIO_LED_BATTERY_LOW, data);
		case LED_CAPS_LOCK:
			return gpio_output_data(GPIO_LED_CAPS_LOCK, data);
		case LED_NUM_LOCK:
			return gpio_output_data(GPIO_LED_NUM_LOCK, data);
		default:
			return -1;
	}
}

signed char led_init(void) __banked
{
	signed char rc = 0;

	/* LED GPIO outputs are active low. */
	rc |= gpio_output_data(GPIO_LED_POWER, 1);
	rc |= gpio_output_data(GPIO_LED_BATTERY_CHARGING, 1);
	rc |= gpio_output_data(GPIO_LED_BATTERY_LOW, 1);
	rc |= gpio_output_data(GPIO_LED_CAPS_LOCK, 1);
	rc |= gpio_output_data(GPIO_LED_NUM_LOCK, 1);

	rc |= gpio_output_enable(GPIO_LED_POWER, 1);
	rc |= gpio_output_enable(GPIO_LED_BATTERY_CHARGING, 1);
	rc |= gpio_output_enable(GPIO_LED_BATTERY_LOW, 1);
	rc |= gpio_output_enable(GPIO_LED_CAPS_LOCK, 1);
	rc |= gpio_output_enable(GPIO_LED_NUM_LOCK, 1);

	return rc;
}
