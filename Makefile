CC=gcc
CFLAGS=-c -O6 -Wall -Wextra

PROGS=inv_test inv_bits_test

all: $(PROGS)

inv_bits_test: inv_bits_test.o
	$(CC) $< -o $@

inv_test: \
 inv_test.o \
 inv_machine_word.o
	$(CC) $< inv_machine_word.o -o $@

inv_bits_test.o: inv_bits_test.c \
 inv_bits.h
	$(CC) $(CFLAGS) $< -o $@

inv_test.o: \
 inv_test.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(PROGS)
