.PHONY : all	clean

# base directories
ROOT	:= $(shell	dirname $(realpath	$(firstword $(MAKEFILE_LIST))))
BIN		:= $(ROOT)/bin
INCLUDE	:= $(ROOT)/include
INTFACE	:= $(ROOT)/interface
OBJ		:= $(ROOT)/obj
SRC		:= $(ROOT)/src
TEST	:= $(ROOT)/test

# compilation flags
CC		:= g++
CFLAGS	:= -std=c++11 -g

all:	$(BIN)/surface_test.out \
		$(BIN)/kbd_test.out

clean:
	@rm $(BIN)/*
	@rm $(OBJ)/*

# Object files

$(OBJ)/%.o:	$(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $^ -I$(INTFACE) -I$(INCLUDE) -o $@

# Binary files

$(BIN)/surface_test.out:	$(TEST)/surface_test.cpp \
							$(OBJ)/surface.o \
							$(OBJ)/term_surface.o
	$(CC) $(CFLAGS) $^ -I$(INTFACE) -I$(INCLUDE) -o $@

$(BIN)/kbd_test.out:	$(TEST)/kbd_test.cpp \
						$(OBJ)/linux_kbd_device.o
	$(CC) $(CFLAGS) $^ -I$(INTFACE) -I$(INCLUDE) -o $@