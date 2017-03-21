#include <stdlib.h>
#include <time.h>
#include "map.h"

int main (int argc, char *argv[]){
    srand((unsigned int)time(NULL));
    unsigned int nbGame = (unsigned int)atoi(argv[1]);
    unsigned int nbPlayer = (unsigned int)atoi(argv[2]);

	MapContext *mapContext = malloc(sizeof(MapContext));
	initMap(mapContext, nbPlayer);
	mainMap(mapContext);

    return 0;

}
