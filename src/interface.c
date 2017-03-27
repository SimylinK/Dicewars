#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

// Structure pour garder les informations du contexte de la partie
typedef struct
{
	int id;	// identifiant de l'IA
	int nbPlayer;	// Noms des personnes participant au projet.
} SContext;

SContext contexte; // variable global du contexte

void InitGame(unsigned int id, unsigned int nbPlayer, SPlayerInfo *info){
    //printf("IA %i initialized\n", id);

    contexte.id = id;
    contexte.nbPlayer = nbPlayer;

    strcpy(info->name, "Le chien fou");

    strcpy(info->members[0], "Monvoisin Mathilde");
    strcpy(info->members[1], "Le Priol Yoann");
    strcpy(info->members[2], "Maraval Nathan");
    strcpy(info->members[3], "Pagano Lucas");
}

//Attaque si il a plus de cases que l'adversaire
int PlayTurn(const SMap *map, STurn *turn) {

  for (int i=0; i < map->nbCells; i++){
    if (map->cells[i].id == contexte.id){
      for (int j=0; j < map->cells[i].nbNeighbors; j++){
        if (map->cells[i].neighbors[j]->owner != contexte.id //la case attaqué n'appartient pas a l'IA
          && map->cells[i].nbDices > map->cells[i].neighbors[j]->nbDices){  //la case de l'IA a plus de dé que l'autre case
          //Attaque
          turn->cellFrom = map->cells[i].id;
          turn->cellTo = map->cells[i].neighbors[j]->id;
          return 1;
        }
      }
    }
  }
  return 0;
}

void EndGame(unsigned int idWinner){
	printf("END\n");
}
