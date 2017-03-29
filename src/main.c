    #include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "util.h"


int main (int argc, char *argv[]){
    //Pas assez d'arguments
    if (argc < 2) {
      printf("Erreur !\nIl faut passer au moins 2 arguments : le nombre de parties et le nombre de joueurs\n");
    }
    //Pas assez de joueurs
    else if ((unsigned int)atoi(argv[2]) < 1 || (unsigned int)atoi(argv[2]) > 8) {
      printf("Erreur !\nLa partie peut comporter de 2 à 8 joueurs\n");
    }
    else {
      srand((unsigned int)time(NULL));
      //unsigned int nbGame = (unsigned int)atoi(argv[1]);
      unsigned int nbPlayer = (unsigned int)atoi(argv[2]);

      MapContext *mapContext = malloc(sizeof(MapContext));
    	initMap(mapContext, nbPlayer);


    	mainMap(mapContext);

      // Initialisation des joueurs via interfaces
      SInterface **interfaces = (SInterface**)malloc(nbPlayer*sizeof(SInterface*));
      if (initPlayers(nbPlayer, interfaces, argc, argv) == 1) {
        return 1;
      }

      //appel de gameLoop
        gameLoop(mapContext, interfaces, nbPlayer);


      return 0;
    }
}
