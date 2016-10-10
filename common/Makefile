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
