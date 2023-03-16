
CC = gcc

CFLAGS = -Wall -Werror -Wextra

all: s21_grep

s21_grep: grep.o
	$(CC) $(CFLAGS) grep.o -o s21_grep -g

grep.o: grep.c
	$(CC) $(CFLAGS) -c grep.c -o grep.o -g

clean:
	rm *.o
	rm s21_grep

rebuild: clean all