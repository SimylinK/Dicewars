CC=gcc -std=c99
EXECUTABLE=dicewars

CFLAGS=-Wall
LCFLAGS=$(shell sdl2-config --cflags)
LLIBS=$(shell sdl2-config --libs)

$(EXECUTABLE): main.o interface.o util.o map.o
	$(CC) -o $(EXECUTABLE)  main.o interface.o util.o map.o $(LCFLAGS) $(LLIBS) -lm

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

interface.o: interface.c interface.h
	$(CC) $(CFLAGS) -c interface.c -o interface.o

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c -o util.o

map.o: map.c map.h
	$(CC) $(CFLAGS) -c map.c -o map.o -lm

clean:
	rm *.o $(EXECUTABLE)