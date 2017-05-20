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

#include <kb9012/gpwu.h>
#include <g505s/button.h>
#include <g505s/switch.h>
#include <button.h>
#include <switch.h>

void gpwu_interrupt_2(void) __interrupt(26)
{
	if (gpwu_event_pending(GPIO_BUTTON_RECOVERY)) {
		gpwu_event_clear(GPIO_BUTTON_RECOVERY);
		button_event = 1;
	}
}

void gpwu_interrupt_5(void) __interrupt(30)
{
	if (gpwu_event_pending(GPIO_BUTTON_POWER)) {
		gpwu_event_clear(GPIO_BUTTON_POWER);
		button_event = 1;
	} else if (gpwu_event_pending(GPIO_SWITCH_INPUT_AC)) {
		gpwu_event_clear(GPIO_SWITCH_INPUT_AC);
		switch_event = 1;
	} else if (gpwu_event_pending(GPIO_SWITCH_INPUT_LID)) {
		gpwu_event_clear(GPIO_SWITCH_INPUT_LID);
		switch_event = 1;
	}
}
