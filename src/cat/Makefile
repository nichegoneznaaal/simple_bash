
CC = gcc

CFLAGS = -Wall -Werror -Wextra

all: s21_cat

s21_cat: cat.o
	$(CC) $(CFLAGS) cat.o -o s21_cat -g

cat.o: cat.c
	$(CC) $(CFLAGS) -c cat.c -o cat.o -g

clean:
	rm *.o
	rm s21_cat

rebuild: clean all