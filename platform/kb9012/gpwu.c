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

#include <8051.h>
#include <kb9012/core.h>
#include <kb9012/gpwu.h>
#include <kb9012/gpio.h>
#include <core.h>

#pragma codeseg CSEGP

static unsigned char gpwu_save[32];

static unsigned char gpwu_register_offset(unsigned char gpio)
{
	unsigned char base;
	unsigned char offset;

	base = gpio & GPIO_BASE_MASK;

	if (base == GPIO_BASE_D0)
		offset = GPWU_OFFSET_D0;
	else if (base == GPIO_BASE_A0)
		offset = GPWU_OFFSET_A0 + ((gpio >> GPWU_OFFSET_SHIFT) & GPWU_OFFSET_MASK_A0);
	else
		offset = gpio >> GPWU_OFFSET_SHIFT;

	return offset;
}

static unsigned char gpwu_value_enable(unsigned char gpio, unsigned char value, unsigned char enable)
{
	unsigned char flag;

	flag = 1 << (gpio & GPWU_BIT_MASK);

	if (enable)
		value |= flag;
	else
		value &= ~flag;

	return value;
}

static signed char gpwu_register_enable(unsigned char gpio, unsigned short base, unsigned char enable)
{
	unsigned short address;
	unsigned char offset;
	unsigned char value;

	offset = gpwu_register_offset(gpio);
	if (offset > GPWU_OFFSET_SPAN)
		return -1;

	address = base + offset;

	value = register_read(address);
	value = gpwu_value_enable(gpio, value, enable);
	register_write(address, value);

	return 0;
}

static signed char gpwu_register_check(unsigned char gpio, unsigned short base)
{
	unsigned short address;
	unsigned char offset;
	unsigned char value;

	offset = gpwu_register_offset(gpio);
	if (offset > GPWU_OFFSET_SPAN)
		return -1;

	address = base + offset;

	value = register_read(address);
	value = value & gpwu_value_enable(gpio, 0, 1);

	return value;
}

static signed char gpwu_interrupt_enable(unsigned char gpio, unsigned char enable)
{
	unsigned char base;
	unsigned char index;
	unsigned char flag;

	base = gpio & GPIO_BASE_MASK;
	index = GPWU_INTERRUPT_INDEX_BASE;

	if (base == GPIO_BASE_D0)
		index += GPWU_INTERRUPT_INDEX_D0;
	else if (base == GPIO_BASE_A0)
		index += GPWU_INTERRUPT_INDEX_A0;
	else
		index += gpio >> GPWU_INTERRUPT_INDEX_SHIFT;

	flag = (1 << index);

	if (enable)
		P3IE |= flag;
	else
		P3IE &= ~flag;

	return 0;
}

signed char gpwu_event_enable(unsigned char gpio, unsigned char enable) __banked
{
	signed char rc = 0;

	rc |= gpwu_event_clear(gpio);
	rc |= gpwu_register_enable(gpio, GPWU_EN_BASE, enable);
	rc |= gpwu_interrupt_enable(gpio, enable);

	return rc;
}

signed char gpwu_event_pending(unsigned char gpio) __banked
{
	return gpwu_register_check(gpio, GPWU_PF_BASE);
}

signed char gpwu_event_clear(unsigned char gpio) __banked
{
	return gpwu_register_enable(gpio, GPWU_PF_BASE, 1);
}

signed char gpwu_polarity_selection(unsigned char gpio, unsigned char high) __banked
{
	return gpwu_register_enable(gpio, GPWU_PS_BASE, high);
}

signed char gpwu_trigger_selection(unsigned char gpio, unsigned char level) __banked
{
	return gpwu_register_enable(gpio, GPWU_EL_BASE, level);
}

signed char gpwu_trigger_toggle(unsigned char gpio, unsigned char enable) __banked
{
	return gpwu_register_enable(gpio, GPWU_CH_BASE, enable);
}

void gpwu_suspend(void) __banked
{
	unsigned short address;
	unsigned char i;

	address = GPWU_EN_BASE;

	for (i = 0; i < sizeof(gpwu_save) / sizeof(unsigned char); i++)
		gpwu_save[i] = register_read(address++);
}

void gpwu_resume(void) __banked
{
	unsigned short address;
	unsigned char i;

	address = GPWU_EN_BASE;

	for (i = 0; i < sizeof(gpwu_save) / sizeof(unsigned char); i++)
		register_write(address++, gpwu_save[i]);
}
