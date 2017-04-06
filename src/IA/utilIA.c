#include <stdlib.h>
#include "utilIA.h"

// Retourne un unsigned int entre 0 et max compris
unsigned int goodRandom(unsigned int max) {

	return rand()%(max+1);
}

// Retourne un unsigned int entre min et max, min<=max
unsigned int randomBounds(unsigned int min, unsigned int max){
	return min + goodRandom(max-min);
}

//renvoie 1 si les cellules passées en paramètre sont voisines, 0 sinon
int isNeighbor(SCell *cell1, SCell *cell2){
	int neighbor = 0;
	for (int i=0 ;  i<cell1->nbNeighbors ; i++){
		if (cell1->neighbors[i] == cell2){
			//on a trouvé cell2 dans la liste des voisins de cell1
			neighbor = 1;
		}
	}
	return neighbor;
}


// Fonction qui calcule les renforts d'un joueur ayant l'id : idPlayer
// On fait le max du nombre de SCell de ses composantes connexes
int calcReinforcements(const SMap *map, int idPlayer, PlayerIslets* player){

	for (int i=0; i<map->nbCells; i++){
		player->islet[i].cells = malloc(sizeof(SCell)*map->nbCells);
		player->islet[i].nbCells = 0;
	}


	getAllCells(player, map, idPlayer);

	assembleIslets(player, idPlayer);

	int reinforcements = maxConnex(player);

	return reinforcements;
}


void getAllCells(PlayerIslets *player, const SMap *map, int idPlayer){
	int index = 0;
	for (int i=0; i<map->nbCells; i++){
		if (map->cells[i].owner == idPlayer){
			player->allMyCells[index] = map->cells[i];
			player->nbOfCells+=1;
			index++;
		}
	}
}

void assembleIslets(PlayerIslets *player, int idPlayer){
	for (int i=0; i<player->nbOfCells; i++){
		if (!cellInIslet(player, player->allMyCells[i].id)){ // Si la cellule n'a pas été visitée
			player->nbIslets++; // On a donc une composante connexe en plus
			DFS(player, player->allMyCells[i], idPlayer);
		}
	}
}

// Parcours en profondeur
void DFS(PlayerIslets *player, SCell cell, int idPlayer){
	// On ajoute la cellule a une composante connexe
	int index = player->nbIslets-1;
	player->islet[index].cells[player->islet[index].nbCells] =  cell;
	player->islet[index].nbCells ++;


	for (int i=0; i<cell.nbNeighbors; i++){
		if (cell.neighbors[i]->owner == idPlayer && !cellInIslet(player, cell.neighbors[i]->id)) { // Si la cellule a le bon owner et n'a pas été visitée
			DFS(player, *(cell.neighbors[i]), idPlayer);
		}
	}
}

int maxConnex(PlayerIslets *player){
	int max = 0;
	for (int i = 0; i < player->nbIslets; i++){
		if (player->islet[i].nbCells>max){
			max = player->islet[i].nbCells;
		}
	}
	return max;
}

// Renvoie si la cellule est dans les ilôts du joueur
int cellInIslet(PlayerIslets *player, int id){
	int found = 0;
	for (int i=0; i<player->nbIslets && !found; i++){
		for (int j=0; j<player->islet[i].nbCells &&!found; j++){
			if (player->islet[i].cells[j].id == id) {
				found = 1;
			}
		}
	}
	return found;
}
