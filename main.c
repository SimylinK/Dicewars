#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "interface.h"

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
    //unsigned int nbGame = (unsigned int)atoi(argv[1]);
    unsigned int nbPlayer = (unsigned int)atoi(argv[2]);

    //Initialisation du nom des joueurs
    char names[nbPlayer][30];
    for (int i=0; i<nbPlayer; i++){
      printf("Le joueur %d doit choisir son nom\n", i+1);
      scanf("%s", names[i]);
    }

    // Initialisation des joueurs

    typedef void (*pfInitGame)(unsigned int, unsigned int, SPlayerInfo*);
    typedef void (*pfPlayTurn)(const SMap*, STurn*);
    typedef void (*pfEndGame)(unsigned int);

    typedef struct  {
      pfInitGame InitGame;
      pfPlayTurn PlayTurn;
      pfEndGame EndGame;
    } SInterface;

    SInterface *interfaces[nbPlayer];

    // 1 joueur normal, le reste en IA
    for (int i=0; i<nbPlayer; i++){
      //le joueur normal
      if (i == 0){
        interfaces[i] = NULL;
      } else {
        void *ia;

        pfInitGame InitGame;
        pfPlayTurn PlayTurn;
        pfEndGame EndGame;

        if ((ia=dlopen("./malib.so",RTLD_LAZY))==NULL) {
          // Erreur de chargement de la librairie
          printf("La librairie n'a pas pu être chargée\n");
          return(0);
        }

        if ((InitGame=(pfInitGame)dlsym(ia,"InitGame"))==NULL) {
          // Erreur lors du chragement de la fonction
          printf("Une erreur s'est produite lors de la lecture de InitGame\n");
          return(0);
        }
        if ((PlayTurn=(pfPlayTurn)dlsym(ia,"PlayTurn"))==NULL) {
          // Erreur lors du chragement de la fonction
          printf("Une erreur s'est produite lors de la lecture de PlayTurn\n");
          return(0);
        }
        if ((EndGame=(pfEndGame)dlsym(ia,"EndGame"))==NULL) {
          // Erreur lors du chragement de la fonction
          printf("Une erreur s'est produite lors de la lecture de EndGame\n");
          return(0);
        }

        SInterface ia1 = {.InitGame = InitGame, .PlayTurn = PlayTurn, .EndGame = EndGame};

        interfaces[i] = &ia1;

        //Initialisation de l'IA au passage
        SPlayerInfo info = {.name = "stratGroupe5", .members = {"Monvoisin Mathilde\0", "Le Priol Yoann\0", "Maraval Nathan\0", "Pagano Lucas\0"}};
        interfaces[i]->InitGame(0, nbPlayer, &info);
        printf("%s\n", info.name);
      }

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

        //Quand l'ia termine son tour ou cup incorrect
        player = (player+1) % nbPlayer;
      }

    }

    return 0;
  }
}
