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
#include <kb9012/core.h>
#include <kb9012/gpio.h>
#include <core.h>

#pragma codeseg CSEGP

static unsigned char gpio_register_offset(unsigned char gpio)
{
	unsigned char base;
	unsigned char offset;

	base = gpio & GPIO_BASE_MASK;

	if (base == GPIO_BASE_D0)
		offset = GPIO_OFFSET_D0;
	else if (base == GPIO_BASE_A0)
		offset = GPIO_OFFSET_A0 + ((gpio >> GPIO_OFFSET_SHIFT) & GPIO_OFFSET_MASK_A0);
	else
		offset = gpio >> GPIO_OFFSET_SHIFT;

	return offset;
}

static unsigned char gpio_value_enable(unsigned char gpio, unsigned char value, unsigned char enable)
{
	unsigned char flag;

	flag = 1 << (gpio & GPIO_BIT_MASK);

	if (enable)
		value |= flag;
	else
		value &= ~flag;

	return value;
}

static signed char gpio_register_enable(unsigned char gpio, unsigned short base, unsigned char enable)
{
	unsigned short address;
	unsigned char offset;
	unsigned char value;

	offset = gpio_register_offset(gpio);
	if (offset > GPIO_OFFSET_SPAN)
		return -1;

	address = base + offset;

	value = register_read(address);
	value = gpio_value_enable(gpio, value, enable);
	register_write(address, value);

	return 0;
}

static signed char gpio_register_check(unsigned char gpio, unsigned short base)
{
	unsigned short address;
	unsigned char offset;
	unsigned char value;

	offset = gpio_register_offset(gpio);
	if (offset > GPIO_OFFSET_SPAN)
		return -1;

	address = base + offset;

	value = register_read(address);
	value = value & gpio_value_enable(gpio, 0, 1);

	return value;
}

signed char gpio_function_selection(unsigned char gpio, unsigned char alternative) __banked
{
	return gpio_register_enable(gpio, GPIO_FS_BASE, alternative);
}

signed char gpio_output_enable(unsigned char gpio, unsigned char enable) __banked
{
	return gpio_register_enable(gpio, GPIO_OE_BASE, enable);
}

signed char gpio_input_enable(unsigned char gpio, unsigned char enable) __banked
{
	return gpio_register_enable(gpio, GPIO_IE_BASE, enable);
}

signed char gpio_output_data(unsigned char gpio, unsigned char data) __banked
{
	return gpio_register_enable(gpio, GPIO_D_BASE, data);
}

signed char gpio_input_data(unsigned char gpio) __banked
{
	return gpio_register_check(gpio, GPIO_IN_BASE);
}

signed char gpio_pull_up(unsigned char gpio, unsigned char enable) __banked
{
	return gpio_register_enable(gpio, GPIO_PU_BASE, enable);
}

signed char gpio_open_drain(unsigned char gpio, unsigned char enable) __banked
{
	return gpio_register_enable(gpio, GPIO_OD_BASE, enable);
}
