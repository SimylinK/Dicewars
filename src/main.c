#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "map.h"
#include "init.h"

int main (int argc, char *argv[]){
    //Pas assez d'arguments
    if (argc < 2) {
      printf("Erreur !\nIl faut passer 2 arguments : le nombre de parties et le nombre de joueurs\n");
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

      int player = 0;
      int end = 0;
      //Boucle de jeu
      while(!end){
        //Tour d'un joueur humain

        if(interfaces[player] == NULL){
          //Récupération du choix du joueur
          int cellFrom;
          printf("id de la cellule de départ : \n");
          cellFrom = getIdOnClick(mapContext->nbNodes, mapContext->cellsList);

          printf("%i\n", cellFrom);

          if (cellFrom == -2){
            end = 1;
          } else if (cellFrom == -1){
            //passage au joueur suivant
            player = (player+1) % nbPlayer;
          } else {

            int cellTo;
            printf("id de la cellule d'arrivée : \n");
            cellTo = getIdOnClick(mapContext->nbNodes, mapContext->cellsList);
            if (cellTo == -2){
              end = 1;
            }else{

              //TODO : Là pour jouer le coup sur l'interface ?
              printf("Coup joué : %d vers %d\n", cellFrom, cellTo);
              // Faire appel à l'arbitre
              // On redessine la map
              drawMap(mapContext->map, mapContext->cellsList, mapContext->nbNodes);

            }
          }
        }
        //Tour d'une IA
        else {
          printf("Tour de l'IA\n");
          //ici faire interfaces[player]->PlayTurn(...)

          //Quand l'ia termine son tour ou coup incorrect
          player = (player+1) % nbPlayer;
        }
      }
      return 0;
    }
}
