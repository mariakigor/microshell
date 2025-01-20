CC = gcc
CFLAGS = -Wall
LDFLAGS = -lreadline

microshell: microshell.c
	$(CC) $(CFLAGS) microshell.c -o microshell $(LDFLAGS)