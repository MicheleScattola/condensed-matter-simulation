CC= gcc
CFLAGS=  -Wall -lm -O3 #-pg -g

default: mc1 md8

mc1: mc.1.c
	$(CC) -o mc.x mc.1.c $(CFLAGS)

md8: md.8.c
	$(CC) -o md.x md.8.c $(CFLAGS)

clean:
	rm -f *.x
