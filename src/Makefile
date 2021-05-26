PROJECT_NAME ?= PugEngine
VERSION ?= 0.1
CC ?= gcc
BUILD ?= release #debug
FLAGS += -Wall -Werror -O2
LINKS ?= bitboard.c board.c
SOURCE ?= eval.c

ifeq ($(BUILD), debug)
	flags += -g
endif

$(PROJECT_NAME): $(SOURCE)
	$(CC) -o $(PROJECT_NAME) $(SOURCE) $(FLAGS) $(LINKS)
