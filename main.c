#include <stdlib.h>
#include "interface.h"
#include <time.h>
#include <SDL2/SDL.h>
#include "map.h"

int main (int argc, char *argv[]){
    srand((unsigned int)time(NULL));
    unsigned int nbGame = (unsigned int)atoi(argv[1]);
    unsigned int nbPlayer = (unsigned int)atoi(argv[2]);

	initMap(nbPlayer);

    return 0;

}
