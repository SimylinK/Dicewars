#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../interface.h"
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

	strcpy(info->name, "Le chien fou");

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

	int cellFrom;
	int cellTo;
	int playAgain = 0;
	int numberOfPlayableTurns = 0;
	// On évalue seulement les tours "jouables" -> voir les if
	for (int i=0; i < map->nbCells; i++){
		// On vérifie si on est l'owner et on initialise cellFrom
		if ((cellFrom = map->cells[i].owner)){
			for (int j=0; j < map->cells[i].nbNeighbors; j++){
				// On vérifie si on attaque une cellule dont on est pas l'owner, et on initialise cellTo
				if(cellFrom != (cellTo = map->cells[i].neighbors[j]->owner)
				    && tabProbas[cellFrom][cellTo] >= 0.74 // Si on a + de 74% de chances de gagner le coup
					&& map->cells[cellFrom].nbDices > 1){

					playableTurns[numberOfPlayableTurns].cellFrom = (unsigned int)cellFrom;
					playableTurns[numberOfPlayableTurns].cellTo = (unsigned int)cellTo;
					numberOfPlayableTurns++;
				}
			}
		}
	}


	// On cherche le meilleur tour
	int bestFrom;
	int bestTo;
	int bestProb = 0;
	for (int i=0; i<numberOfPlayableTurns; i++){
		cellFrom = playableTurns[i].cellFrom;
		cellTo = playableTurns[i].cellTo;
		if(tabProbas[cellFrom][cellTo]>bestProb){
			bestFrom = cellFrom;
			bestTo = cellTo;
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