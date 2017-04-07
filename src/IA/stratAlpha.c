#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/interface.h"
#include "utilIA.h"
#include "proba.h"

// Structure pour garder les informations du contexte de la partie
typedef struct
{
	int id;	// identifiant de l'IA
	int nbPlayer;	// Noms des personnes participant au projet.
} SContext;

SContext contexte; // variable globale du contexte

void InitGame(unsigned int id, unsigned int nbPlayer, SPlayerInfo *info){
	contexte.id = id;
	contexte.nbPlayer = nbPlayer;

	strcpy(info->name, "Strat alpha");

	strcpy(info->members[0], "Monvoisin Mathilde");
	strcpy(info->members[1], "Le Priol Yoann");
	strcpy(info->members[2], "Maraval Nathan");
	strcpy(info->members[3], "Pagano Lucas");
}

// On crée un tableau avec les cellules et leur valeur associée
int PlayTurn(unsigned int id, const SMap *map, STurn *turn) {

	// On initialise les structures nécessaires
	PlayerIslets *player = malloc(sizeof(PlayerIslets));

	player->islet = malloc(sizeof(Islet)*map->nbCells);
	player->allMyCells = malloc(sizeof(SCell)*map->nbCells);
	player->nbIslets = 0;
	player->nbOfCells = 0;

	for (int i=0; i<map->nbCells; i++){
		player->islet[i].cells = malloc(sizeof(SCell)*map->nbCells);
		player->islet[i].nbCells = 0;
	}


	// On calcule ses renforts et on remplit player du même temps
	calcReinforcements(map, id, player);

	STurn *playableTurns = malloc(sizeof(STurn)*(player->nbOfCells)*(map->nbCells-player->nbOfCells));


	int playAgain = 0;
	int numberOfPlayableTurns = 0;
	// On évalue seulement les tours "jouables" -> voir les if
	for (int i = 0; i < map->nbCells; i++) {
		// On vérifie si on est l'owner et si la cellule a plus d'un dé
		if (id == (unsigned int) map->cells[i].owner && map->cells[i].nbDices > 1) {
			for (int j = 0; j < map->cells[i].nbNeighbors; j++) {
				// On vérifie si on attaque une cellule dont on est pas l'owner
				if (map->cells[i].neighbors[j]->owner != id
					// On élimine au passage les coups pour lesquels on a moins de dés que l'adversaire
						&& map->cells[i].nbDices >=  map->cells[i].neighbors[j]->nbDices){
					playableTurns[numberOfPlayableTurns].cellFrom = (unsigned int) map->cells[i].id;;
					playableTurns[numberOfPlayableTurns].cellTo = (unsigned int) map->cells[i].neighbors[j]->id;;
					numberOfPlayableTurns++;
				}
			}
		}
	}

	int idFrom;
	int idTo;
	// On élimine les coups à dés égaux si la stack est sous 8
	if (map->stack[id]<8){
		int numberOfPlayableTurns2 = 0;
		for (int i=0; i<numberOfPlayableTurns; i++) {
			idFrom = playableTurns[i].cellFrom;
			idTo = playableTurns[i].cellTo;
			if (map->cells[idFrom].nbDices > map->cells[idTo].nbDices){
				playableTurns[numberOfPlayableTurns].cellFrom = (unsigned int) map->cells[idFrom].id;;
				playableTurns[numberOfPlayableTurns].cellTo = (unsigned int) map->cells[idTo].id;
				numberOfPlayableTurns2++;
			}

		}
		numberOfPlayableTurns = numberOfPlayableTurns2;
	}





	// On cherche la meilleure probabilité
	int bestFrom;
	int bestTo;

	double bestProb = 0;
	for (int i=0; i<numberOfPlayableTurns; i++){
		idFrom = playableTurns[i].cellFrom;
		idTo = playableTurns[i].cellTo;
		if(tabProbas[map->cells[idFrom].nbDices-1][map->cells[idFrom].nbDices-1]>bestProb){
			bestFrom = idFrom;
			bestTo = idTo;
			playAgain = 1;
		}
	}

	turn->cellTo = (unsigned int)bestTo; // Si non initialisé, on renvoie 0, donc pas de problème
	turn->cellFrom = (unsigned int)bestFrom;

	free(playableTurns);

	for (int i=0; i<map->nbCells; i++){
		free(player->islet[i].cells);
	}

	free(player->allMyCells);
	free(player->islet);
	free(player);

	return playAgain ;
}




void EndGame(unsigned int id, unsigned int idWinner) {
}
