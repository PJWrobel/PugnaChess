PROJECT_NAME ?= PugEngine
VERSION ?= 0.1
CC ?= gcc
BUILD ?= release #debug
FLAGS += -Wall -Werror -O2
LINKS ?= bitboard.c
SOURCE ?= board.c

ifeq ($(BUILD), debug)
	flags += -g
endif

$(PROJECT_NAME): $(SOURCE)
	$(CC) $(SOURCE) -o $(PROJECT_NAME) $(FLAGS) $(LINKS)
