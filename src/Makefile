CC=gcc -std=c99
EXECUTABLE=dicewars

CFLAGS=-Wall -g 
LLIBS=$(shell sdl2-config --cflags --libs)

$(EXECUTABLE): main.o interface.o util.o map.o init.o arbiter.o
	$(CC) -o $(EXECUTABLE)  main.o interface.o util.o map.o init.o arbiter.o $(LLIBS) -lm -ldl

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

interface.o: interface.c interface.h
	$(CC) $(CFLAGS) -c interface.c -o interface.o

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c -o util.o

map.o: map.c map.h init.h
	$(CC) $(CFLAGS) -c map.c -o map.o -lm

init.o: init.c init.h
	$(CC) $(CFLAGS) -c init.c -o init.o -lm

arbiter.o: arbiter.c arbiter.h
	$(CC) $(CFLAGS) -c arbiter.c -o arbiter.o -lm

clean:
	rm *.o $(EXECUTABLE)
