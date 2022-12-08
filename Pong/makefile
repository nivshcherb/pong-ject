.PHONY : all 

# base directories
ROOT	:= $(shell	dirname $(realpath	$(firstword $(MAKEFILE_LIST))))
SRC		:= $(ROOT)/src
INCLUDE	:= $(ROOT)/include
INTFACE	:= $(ROOT)/interface
TEST	:= $(ROOT)/test
BIN		:= $(ROOT)/bin

# compilation flags
CC		:= g++
CFLAGS	:= -std=c++11 -g

all:	$(BIN)/surface_test.out \
		$(BIN)/kbd_test.out

$(BIN)/surface_test.out:	$(TEST)/surface_test.cpp \
							$(SRC)/surface.cpp \
							$(SRC)/term_surface.cpp
	$(CC) $(CFLAGS) -o $@ $^ -I$(INTFACE) -I$(INCLUDE)

$(BIN)/kbd_test.out:	$(TEST)/kbd_test.cpp \
						$(SRC)/linux_kbd_device.cpp
	$(CC) $(CFLAGS) -o $@ $^ -I$(INTFACE) -I$(INCLUDE)

clean:
	rm *.out