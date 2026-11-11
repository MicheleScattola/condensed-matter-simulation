CC= gcc
CFLAGS=  -Wall -lm -O3 #-pg -g

default: md.7.c
	$(CC) -o md.x md.7.c $(CFLAGS)

clean:
	rm -f *.x
	rm h19.dat h21.dat ave19.dat ave21.dat cv.dat
