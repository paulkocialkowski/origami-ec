# Copyright (C) 2015-2016 Paul Kocialkowski <contact@paulk.fr>
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

# Tools

SDCC ?= sdcc
OBJCOPY ?= objcopy
SED ?= sed

# Project

NAME = origami-ec
VERSION = pre-square

# Config

CONFIG_DEVICE ?= g505s
CONFIG_DEBUG ?= yes

DEVICE_CONFIG = $(DEVICE)/config.mk
PLATFORM_CONFIG = $(PLATFORM)/config.mk

# Directories

COMMON = common
DEVICE = device/$(CONFIG_DEVICE)
PLATFORM = platform/$(CONFIG_PLATFORM)

BUILD = build
OUTPUT = output

# Sources

SOURCES_DIRS = $(COMMON) $(DEVICE) $(PLATFORM)
SOURCES_RULES = $(addsuffix /Makefile,$(SOURCES_DIRS))

SOURCES_COMMON = $(addprefix $(COMMON)/,$(sources-common))
SOURCES_DEVICE = $(addprefix $(DEVICE)/,$(sources-device))
SOURCES_PLATFORM = $(addprefix $(PLATFORM)/,$(sources-platform))

SOURCES = $(SOURCES_COMMON) $(SOURCES_DEVICE) $(SOURCES_PLATFORM)
OBJECTS = $(SOURCES:.c=.rel)
DEPS = $(SOURCES:.c=.d)

# Compiler

INCLUDES = $(dir $(BUILD_VERSION_HEADER)) $(addsuffix /include,$(SOURCES_DIRS))
DEFINES = $(config-defines)

CFLAGS = --std-c99 -mmcs51 --model-$(CONFIG_MEMORY_MODEL)
CFLAGS += $(foreach include,$(INCLUDES),-I$(include))
CFLAGS += $(foreach define,$(DEFINES),-D$(define))

LDFLAGS = --out-fmt-ihx -mmcs51 --model-$(CONFIG_MEMORY_MODEL)

# Produced files

BUILD_VERSION_HEADER = $(BUILD)/version.h

BUILD_OBJECTS = $(addprefix $(BUILD)/,$(OBJECTS))
BUILD_DEPS = $(addprefix $(BUILD)/,$(DEPS))
BUILD_BINARY = $(BUILD)/$(NAME)
BUILD_DIRS = $(sort $(dir $(BUILD_BINARY) $(BUILD_OBJECTS)))

OUTPUT_BINARY = $(OUTPUT)/$(NAME)-$(CONFIG_DEVICE).bin
OUTPUT_IMAGE = $(OUTPUT)/$(NAME)-$(CONFIG_DEVICE).img
OUTPUT_DIRS = $(sort $(dir $(OUTPUT_BINARY) $(OUTPUT_IMAGE)))

# Includes

-include $(DEVICE_CONFIG)
-include $(PLATFORM_CONFIG)
-include $(SOURCES_RULES)

# Config

ifneq ($(CONFIG_XRAM_ADDRESS),)
LDFLAGS += --xram-loc $(CONFIG_XRAM_ADDRESS) --xram-size $(CONFIG_XRAM_SIZE)
endif

ifneq ($(CONFIG_CODE_SIZE),)
LDFLAGS += --code-size $(CONFIG_CODE_SIZE)
endif

# Rules

all : $(OUTPUT_IMAGE)

.PHONY: device
device:
	 @echo " DEVICE $(CONFIG_DEVICE)"

$(BUILD_DIRS):
	@mkdir -p $@

$(BUILD_VERSION_HEADER): | $(BUILD_DIRS)
	@echo " VERSION"
	@touch $@
	@echo "#ifndef _VERSION_H_" >> $@
	@echo "#define _VERSION_H_" >> $@
	@echo "#define VERSION \"$(VERSION)\"" >> $@
	@echo "#endif" >> $@

$(BUILD_OBJECTS): $(BUILD)/%.rel: %.c $(BUILD_VERSION_HEADER) Makefile $(SOURCES_RULES) $(CONFIG) | $(BUILD_DIRS)
	@echo " CC     $<"
	@$(SDCC) $(CFLAGS) -MMD -c $< -o $(BUILD)/$*.d
	@$(SED)  "s,$(notdir $*).rel,$(BUILD)/$*.rel,g" -i $(BUILD)/$*.d
	@$(SDCC) $(CFLAGS) -c $< -o $(dir $@)

$(BUILD_BINARY): device $(BUILD_OBJECTS)
	@echo " LINK   $@"
	@$(SDCC) $(LDFLAGS) -o $(BUILD_BINARY) $(BUILD_OBJECTS)

$(OUTPUT_DIRS):
	@mkdir -p $@

$(OUTPUT_BINARY): $(BUILD_BINARY) | $(OUTPUT_DIRS)
	@echo " BINARY $@"
	@$(OBJCOPY) -I ihex -O binary $< $@

$(OUTPUT_IMAGE): $(OUTPUT_BINARY) | $(OUTPUT_DIRS)
	@echo " IMAGE  $@"
	@$(OBJCOPY) -I binary -O binary --pad-to=$(CONFIG_IMAGE_SIZE) --gap-fill=0 $< $@

.PHONY: clean
clean:
	@echo " CLEAN"
	@rm -f $(foreach object,$(basename $(BUILD_OBJECTS)),$(object)*) $(basename $(BUILD_BINARY))*

.PHONY: distclean
distclean: clean
	@echo " DISTCLEAN"
	@rm -rf $(BUILD) $(OUTPUT)

-include $(BUILD_DEPS)
