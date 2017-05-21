# Copyright (C) 2015-2017 Paul Kocialkowski <contact@paulk.fr>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

sources-common = button.c led.c origami-ec.c serial.c switch.c debug.c

ifeq ($(CONFIG_CONSOLE), yes)
sources-common += console.c
config-defines += CONFIG_CONSOLE

ifeq ($(CONFIG_CONSOLE_HELP), yes)
config-defines += CONFIG_CONSOLE_HELP
endif

ifeq ($(CONFIG_CONSOLE_HISTORY), yes)
config-defines += CONFIG_CONSOLE_HISTORY
endif
endif
