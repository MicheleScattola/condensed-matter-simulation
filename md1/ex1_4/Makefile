CC= gcc
CFLAGS=  -Wall -lm -O3

all: velocity verlet

velocity: md.4.c
	$(CC) -o velocity.x md.4.c $(CFLAGS)

verlet: md.2.c
	$(CC) -o verlet.x md.2.c $(CFLAGS)

clean:
	rm -f *.x
	rm -f *.dat