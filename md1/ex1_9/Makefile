CC= gcc
CFLAGS=  -Wall -lm -O3

all: default nocorr

default: md.2.c
	$(CC) -o md.x md.2.c $(CFLAGS)

nocorr: md.2.nocorr.c
	$(CC) -o md.nocorr.x md.2.nocorr.c $(CFLAGS)

clean:
	rm -f *.x
	rm -f *.dat
