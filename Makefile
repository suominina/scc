CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99

scc: src/main.c
	$(CC) $(CFLAGS) -o $@ $^
