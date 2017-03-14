CC=gcc -std=c99

CFLAGS=-Wall -Werror
LCFLAGS=$(shell sdl2-config --cflags)
LLIBS=$(shell sdl2-config --libs)

test: main.o interface.o
	$(CC) -o test  main.o interface.o $(LCFLAGS) $(LLIBS)

main.o: main.c interface.c
	$(CC) $(CFLAGS) -c main.c -o main.o

interface.o: interface.c
	$(CC) $(CFLAGS) -c interface.c -o interface.o

clean:
	rm *.o test