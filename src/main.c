#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "util.h"
#include "map.h"

// Initialisation des joueurs
typedef void (*pfInitGame)(unsigned int, unsigned int, SPlayerInfo*);
typedef void (*pfPlayTurn)(const SMap*, STurn*);
typedef void (*pfEndGame)(unsigned int);

typedef struct  {
  pfInitGame InitGame;
  pfPlayTurn PlayTurn;
  pfEndGame EndGame;
} SInterface;

//initialise interfaces
//renvoie 1 si ça c'est bien passé, 0 sinon
int initPlayers(int nbPlayer, SInterface **interfaces, int argc, char *argv[]){

  // Le nombre d'humains
  int nbHumans = nbPlayer - argc + 3;

  for (int i=0; i<nbPlayer; i++){
    //Les joueurs normaux
    if (i < nbHumans){
      interfaces[i] = NULL;
    } else {
      void *ia;
      pfInitGame InitGame;
      pfPlayTurn PlayTurn;
      pfEndGame EndGame;

      if ((ia=dlopen(argv[i+2],RTLD_LAZY))==NULL) {
        // Erreur de chargement de la librairie
        printf("La librairie %s n'a pas pu être chargée\n", argv[i+2]);
        return(0);
      }

      if ((InitGame=(pfInitGame)dlsym(ia,"InitGame"))==NULL) {
        // Erreur lors du chragement de la fonction
        printf("Une erreur s'est produite lors de la lecture de InitGame de %s\n", argv[i+2]);
        return(0);
      }
      if ((PlayTurn=(pfPlayTurn)dlsym(ia,"PlayTurn"))==NULL) {
        // Erreur lors du chragement de la fonction
        printf("Une erreur s'est produite lors de la lecture de PlayTurn de %s\n", argv[i+2]);
        return(0);
      }
      if ((EndGame=(pfEndGame)dlsym(ia,"EndGame"))==NULL) {
        // Erreur lors du chragement de la fonction
        printf("Une erreur s'est produite lors de la lecture de EndGame de %s\n", argv[i+2]);
        return(0);
      }

      SInterface ia1 = {.InitGame = InitGame, .PlayTurn = PlayTurn, .EndGame = EndGame};

      interfaces[i] = &ia1;

      //Initialisation de l'IA au passage
      SPlayerInfo info;
      interfaces[i]->InitGame(0, nbPlayer, &info);
    }
  }

  return 1;
}

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

      SDL_Window *window = NULL;
    	SDL_Renderer* renderer = NULL;
    	mainMap(mapContext, window, renderer);

      // Initialisation des joueurs via interfaces
      SInterface **interfaces = (SInterface**)malloc(nbPlayer);
      if (initPlayers(nbPlayer, interfaces, argc, argv) == 0) {
        return 0;
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

            //TODO : Là pour jouer le coup sur l'interface ?
            printf("Coup joué : %d vers %d\n", cellFrom, cellTo);
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
