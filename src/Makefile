CC=gcc -std=c99
EXECUTABLE=dicewars

CFLAGS=-Wall -g
LLIBS=$(shell sdl2-config --cflags --libs)

$(EXECUTABLE): main.o util.o map.o init.o arbiter.o output.o
	$(CC) -o $(EXECUTABLE)  main.o util.o map.o init.o arbiter.o output.o $(LLIBS) -lm -ldl

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

util.o: util.c util.h interface.h
	$(CC) $(CFLAGS) -c util.c -o util.o

map.o: map.c map.h init.h interface.h
	$(CC) $(CFLAGS) -c map.c -o map.o -lm

init.o: init.c init.h interface.h
	$(CC) $(CFLAGS) -c init.c -o init.o -lm

arbiter.o: arbiter.c arbiter.h interface.h
	$(CC) $(CFLAGS) -c arbiter.c -o arbiter.o -lm

output.o: output.c output.h
	$(CC) $(CFLAGS) -c output.c -o output.o -lm

clean:
	rm *.o $(EXECUTABLE)
