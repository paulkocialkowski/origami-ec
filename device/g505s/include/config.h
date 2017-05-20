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

#ifndef _CONFIG_H_
#define _CONFIG_H_

/*
 * Names
 */

#define CONFIG_DEVICE_NAME			"G505s"
#define CONFIG_PLATFORM_NAME			"KB9012"

/*
 * Clock
 */

#define CONFIG_CLOCK						32000000
#define CONFIG_CLOCK_IDLE					8000000

/*
 * Serial
 */

#define CONFIG_SERIAL_BAUDRATE					115200

/*
 * Console
 */

#ifdef CONFIG_CONSOLE
#define CONFIG_CONSOLE_BUFFER_LENGTH				64
#define CONFIG_CONSOLE_COMMAND_ARGS_LIMIT			8

#ifdef CONFIG_CONSOLE_HISTORY
#define CONFIG_CONSOLE_HISTORY_COUNT				4
#endif
#endif

#endif
