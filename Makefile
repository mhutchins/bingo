SHELL = /bin/sh

CC      = gcc
INCLUDES= -I.
LIBS    = -lnsl -lsocket -lmd5
CFLAGS  = -shared -fpic -ggdb -threads -pedantic -Wall
LDFLAGS = -lpthread

TARGET=bingo
OBJS = d_main.o

.SUFFIXES:
.SUFFIXES: .d .o .h .c .cc .C .cpp
.c.o: ; $(CC) $(CFLAGS) -MMD -c $*.c

CC=gcc
CPP=g++

%.d: %.c
	touch $@
%.d: %.cc
	touch $@
%.d: %.C
	touch $@
%.d: %.cpp
	touch $@

DEPENDENCIES = $(OBJS:.o=.d)

# 
# Targets:
# 
all: $(TARGET) make.dep

bingo: $(OBJS) $(HDRS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

clean:
	-rm -f $(TARGET) $(OBJS) $(DEPENDENCIES) make.dep


make.dep: $(DEPENDENCIES)
	-cat $(DEPENDENCIES) > make.dep

include make.dep
