#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    int player = 0;
    //Boucle de jeu
    while(1){
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

    return 0;
  }
}
