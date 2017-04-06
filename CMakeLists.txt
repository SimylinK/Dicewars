cmake_minimum_required(VERSION 3.6)
project(dicewars)


set(SOURCE_FILES src/main.c src/arbiter.c src/arbiter.h src/init.c src/init.h src/interface.h src/map.h src/map.c src/util.c src/util.h)
add_executable(dicewars ${SOURCE_FILES})

target_link_libraries(dicewars -lmingw32 -lSDL2main -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid)
