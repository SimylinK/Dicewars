#include <stdlib.h>
#include <time.h>
#include "map.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

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

  // 1 joueur normal, le reste en IA
  for (int i=0; i<nbPlayer; i++){
    //le joueur normal
    if (i == 0 || argc < 4){ // (argc > 4) indique qu'il n'y a pas de bibliothèques en argument
      interfaces[i] = NULL;
    } else {

      //Choix de la bibliothèque dynamique pour l'IA
      int indexArgv = 3;
      if (argc > 4) { //Cas avec 2 bibliothèques en argument
        //Choix d'une bibiothèque de façon aléatoire
        int zeroOrOne = ((double) rand() / (RAND_MAX)) + 1;
        indexArgv += zeroOrOne;
      }

      void *ia;
      pfInitGame InitGame;
      pfPlayTurn PlayTurn;
      pfEndGame EndGame;

      if ((ia=dlopen(argv[indexArgv],RTLD_LAZY))==NULL) {
        // Erreur de chargement de la librairie
        printf("La librairie %s n'a pas pu être chargée\n", argv[indexArgv]);
        return(0);
      }

      if ((InitGame=(pfInitGame)dlsym(ia,"InitGame"))==NULL) {
        // Erreur lors du chragement de la fonction
        printf("Une erreur s'est produite lors de la lecture de InitGame de %s\n", argv[indexArgv]);
        return(0);
      }
      if ((PlayTurn=(pfPlayTurn)dlsym(ia,"PlayTurn"))==NULL) {
        // Erreur lors du chragement de la fonction
        printf("Une erreur s'est produite lors de la lecture de PlayTurn de %s\n", argv[indexArgv]);
        return(0);
      }
      if ((EndGame=(pfEndGame)dlsym(ia,"EndGame"))==NULL) {
        // Erreur lors du chragement de la fonction
        printf("Une erreur s'est produite lors de la lecture de EndGame de %s\n", argv[indexArgv]);
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

      //Initialisation du nom des joueurs
      char names[nbPlayer][30];
      for (int i=0; i<nbPlayer; i++){
        printf("Le joueur %d doit choisir son nom\n", i+1);
        scanf("%s", names[i]);
      }

      // Initialisation des joueurs via interfaces
      SInterface **interfaces = (SInterface**)malloc(nbPlayer);
      if (initPlayers(nbPlayer, interfaces, argc, argv) == 0) {
        return 0;
      }

      int player = 0;
      //Boucle de jeu
      while(1){
        //Tour d'un joueur humain
        if(interfaces[player] == NULL){
          //Récupération du choix du joueur
          printf("C'est le tour de : %s (-1 pour passer au joueur suivant)\n", names[player]);
          char cellFromString[10];
          printf("id de la cellule de départ : ");
          scanf("%s", cellFromString);

          printf("%s\n", cellFromString);

          if (strcmp(cellFromString, "-1") == 0){
            //passage au joueur suivant
            player = (player+1) % nbPlayer;
          } else {

            char cellToString[10];
            printf("id de la cellule d'arrivé : ");
            scanf("%s", cellToString);

            int cellFrom = (int)*cellFromString - '0';
            int cellTo = (int)*cellToString - '0';

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
