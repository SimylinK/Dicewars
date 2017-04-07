CC=gcc -std=c99
EXECUTABLE=dicewars

CFLAGS=-Wall -g
LLIBS=$(shell sdl2-config --cflags --libs)

UTIL_H=src/utils/util.h
UTIL_C=src/utils/util.c
INIT_H=src/utils/init.h
INIT_C=src/utils/init.c
INTERFACE_H=src/utils/interface.h

MAP_H=src/display/map.h
MAP_C=src/display/map.c

OUTPUT_H=src/logs/output.h
OUTPUT_C=src/logs/output.c

ARBITER_H=src/arbiter.h
ARBITER_C=src/arbiter.c
MAIN_C=src/main.c

BIN=bin/


$(EXECUTABLE): $(BIN)main.o $(BIN)util.o $(BIN)map.o $(BIN)init.o $(BIN)arbiter.o $(BIN)output.o
	$(CC) -o $(EXECUTABLE)  $(BIN)main.o $(BIN)util.o $(BIN)map.o $(BIN)init.o $(BIN)arbiter.o $(BIN)output.o $(LLIBS) -lm -ldl

$(BIN)main.o: $(MAIN_C) $(UTIL_H) $(ARBITER_H)
	$(CC) $(CFLAGS) -c $(MAIN_C) -o $(BIN)main.o

$(BIN)util.o: $(UTIL_C) $(UTIL_H) $(INTERFACE_H) $(UTIL_H) $(MAP_H)
	$(CC) $(CFLAGS) -c $(UTIL_C) -o $(BIN)util.o

$(BIN)map.o: $(MAP_C) $(MAP_H) $(INIT_H) $(INTERFACE_H) $(UTIL_H)
	$(CC) $(CFLAGS) -c $(MAP_C) -o $(BIN)map.o -lm

$(BIN)init.o: $(INIT_C) $(INIT_H) $(INTERFACE_H) $(UTIL_H)
	$(CC) $(CFLAGS) -c $(INIT_C) -o $(BIN)init.o -lm

$(BIN)arbiter.o: $(ARBITER_C) $(ARBITER_H) $(INTERFACE_H) $(OUTPUT_H) $(INIT_H) $(UTIL_H) $(MAP_H)
	$(CC) $(CFLAGS) -c $(ARBITER_C) -o $(BIN)arbiter.o -lm

$(BIN)output.o: $(OUTPUT_C) $(OUTPUT_H) $(INTERFACE_H)
	$(CC) $(CFLAGS) -c $(OUTPUT_C) -o $(BIN)output.o -lm

clean:
	rm $(BIN)*.o $(EXECUTABLE)
