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

# Platform

CONFIG_PLATFORM = kb9012

# Console

ifeq ($(CONFIG_DEBUG), yes)
CONFIG_CONSOLE = yes
CONFIG_CONSOLE_HELP = yes
CONFIG_CONSOLE_HISTORY = yes
endif
