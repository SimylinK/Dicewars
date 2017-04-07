cmake_minimum_required(VERSION 3.6)
project(dicewars)


set(SOURCE_FILES src/main.c src/game.c src/game.h src/arbiter.c src/arbiter.h src/utils/init.c src/utils/init.h src/utils/interface.h src/display/map.h src/display/map.c src/utils/util.c src/utils/util.h src/logs/output.c src/logs/output.h)
add_executable(dicewars ${SOURCE_FILES})

target_link_libraries(dicewars -lmingw32 -lSDL2main -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid)
