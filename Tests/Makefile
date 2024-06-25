# Detect OS based on the PATH environment variable's use of semicolon as typical in Windows
#this is soley for testing weather or not its even possible and i see no usage of this file for
#at least a few years when my c code gets significantly more complex
CC = gcc
CFLAGS = -Wall -Werror -fPIC

ifeq ($(findstring ;,$(PATH)),;)
OS := Windows
else
# Default to Unix-like OS detection
OS := $(shell uname 2>/dev/null || echo Unknown)
OS := $(patsubst CYGWIN%,Cygwin,$(OS))
OS := $(patsubst MSYS%,MSYS,$(OS))
OS := $(patsubst MINGW%,MSYS,$(OS))
endif

ifeq ($(OS),Darwin)
all:
	@echo Mac-OS is not supported by this program >&2
else ifeq ($(OS), Windows)
all: main.py SHAREDADDER
	@echo You are running this on windows and this is supported
else ifeq ($(OS), Linux)
all: main.py SHAREDADDER
	@echo You are running this on Linux and this doesent have support yet >&2
else
all: main.py SHAREDADDER
	@echo This is a bug, please file a bug report at: https://github.com/Ha2k4r/RPI-Protogen >&2
	@echo please write your operating system in the report, and this down "$(OS)" >&2
endif

SHAREDADDER: add_one.c
	$(CC) $(CFLAGS) -shared -o add_one.so add_one.c

.PHONY: all
