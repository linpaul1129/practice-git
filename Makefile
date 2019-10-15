CC = gcc
CFLAGS = -Wall -O2
SRCDIR = /opt/ForDemo/src
INCLUDE = -I. -I$(SRCDIR)/include
CYLIBS = $(SRCDIR)/.libs
TARGET = test
LDFLAGS = 
OBJS = test.o

ifdef DEBUG
DBGFLAG=-DDEBUG
endif

%.o :	%.c
	$(CC) $(INCLUDE) $(CFLAGS) $(DBGFLAG) -c $< -o $@

all:	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -L$(CYLIBS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
