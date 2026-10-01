CC= gcc
CFLAGS=  -Wall -lm -O3

all: verlet euler

verlet: md.2.c
	$(CC) -o verlet.x md.2.c $(CFLAGS)

euler:	md.1.c
	$(CC) -o euler.x md.1.c $(CFLAGS)


clean:
	rm -f *.x
	rm -f *.dat
