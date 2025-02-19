PROJECT_NAME ?= PugEngine
VERSION ?= 0.1
CC ?= gcc
BUILD ?= release #debug
FLAGS += -Wall -Werror -O2
LINKS ?= bitboard.c
SOURCE ?= board.c

ifeq ($(BUILD), debug)
	flags += -g -Og
endif
ifeq ($(BUILD), release)
    flags += -O3
endif

$(PROJECT_NAME): $(SOURCE)
	$(CC) -o $(PROJECT_NAME) $(SOURCE) $(FLAGS) $(LINKS)
