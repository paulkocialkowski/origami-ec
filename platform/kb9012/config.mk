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

# Memory

CONFIG_MEMORY_MODEL = large

CONFIG_XRAM_ADDRESS = 0xec00
CONFIG_XRAM_SIZE = 0x1000

CONFIG_CODE_PRIMARY_BASE = 0x0000
CONFIG_CODE_PRIMARY_SIZE = 0xc000
CONFIG_CODE_SEGMENTS = CSEG1 CSEG2
CONFIG_CODE_SEGMENT_CSEG1_BASE = 0xc000
CONFIG_CODE_SEGMENT_CSEG1_MAP = 0xc000
CONFIG_CODE_SEGMENT_CSEG2_BASE = 0x1c000
CONFIG_CODE_SEGMENT_CSEG2_MAP = 0x10000
CONFIG_CODE_SEGMENT_SIZE = 0x4000
CONFIG_CODE_SEGMENT_CONSTANT = CSEG
CONFIG_CODE_SIZE = 0x20000

CONFIG_IMAGE_SIZE = $(CONFIG_CODE_SIZE)
