CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -Iinclude

TARGET = scc

SRCS = src/scc.c src/lex.c
OBJS = $(SRCS:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	rm -f $(OBJS)

clean:
	rm -f $(OBJS)
