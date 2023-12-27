CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -lpthread -lgmp
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: $(OBJS)
	$(CC) $(OBJS) ec.o -o a.out $(CFLAGS)

clean:
	rm -f $(OBJS) a.out
