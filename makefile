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

all:	$(BIN)/termwin.out

$(BIN)/termwin.out:	$(TEST)/termwin_test.cpp \
					$(SRC)/termwin.cpp
	$(CC) $(CFLAGS) -o $@ $^ -I$(INTFACE) -I$(INCLUDE)

clean:
	rm *.out