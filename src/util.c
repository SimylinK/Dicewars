#include <stdio.h>
#include <stdlib.h>
#include "util.h"


// Retourne un unsigned int entre 0 et max compris
unsigned int goodRandom(unsigned int max) {

	return rand()%(max+1);
}

// Retourne un unsigned int entre min et max, min<=max
unsigned int randomBounds(unsigned int min, unsigned int max){
	return min + goodRandom(max-min);
}

// Retourne un unsigned int entre min et max, avec une distance d'au moins dist à tous les elements du tableau
// Si x est à un, on teste sur x, sinon sur y
unsigned int distRandomBounds(unsigned int dist, unsigned int min, unsigned int max, Centre *cellsList, unsigned int size, unsigned int x){
	int farEnough; //Les cells sont assez loin
	unsigned int rand = 0;
	do {
		farEnough = 1;
		rand = randomBounds(min, max);
		// On teste sur x ou y
		if (x == 1) {
			for (int i=0; i<size && farEnough; i++) {
				if (abs(cellsList[i].x - rand) < dist) farEnough = 0;
			}
		} else{
			for (int i = 0; i<size && farEnough; i++) {
				if (abs(cellsList[i].y - rand) < dist) farEnough = 0;
			}
		}
	} while(!farEnough);

	return rand;
}

// Retourne la cellule la plus proche
Centre getCloser(Centre* cellsList,unsigned int size, unsigned int x, unsigned int y){

	unsigned int minDist = (unsigned int)abs(cellsList[0].x - x) + abs(cellsList[0].y - y);// Distance de manatthan
	int minIndex = 0;
	unsigned int dist;

	for (int i=1; i<size; i++) {
		dist = (unsigned int)abs(cellsList[i].x - x) + abs(cellsList[i].y - y); // Distance de manatthan
		if (dist <= minDist) {
			minIndex = i;
			minDist = dist;
		}
	}
	return cellsList[minIndex];
}

// Renvoie l'id de la cellule la plus proche sur le clic
unsigned int getIdOnClick(unsigned int nbNodes, Centre *cellsList){
	SDL_Event event;
	int id = -1;
	int noEvent = 1;

	while (noEvent){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_MOUSEBUTTONDOWN:
				if (SDL_BUTTON_LEFT){

					//si le clic se fait sur la zone de la map
					if (event.button.y>BORDERTOP
                        && event.button.y<HEIGHT
                        && event.button.x>BORDERLANDR
						&& event.button.x<WIDTH){

						id = getCloser(cellsList, nbNodes, (unsigned int)event.button.x, (unsigned int)event.button.y).cell->id;
						noEvent = 0;

					} else {
						//si le clic se fait sur le bouton tour suivant
						if((event.button.y>(BUTTONY - BUTTONH/2))
							&& (event.button.y<(BUTTONY + BUTTONH/2))
								&& (event.button.x>(BUTTONX - BUTTONW/2))
									&& (event.button.x<(BUTTONX + BUTTONW/2))){

							id = -1;
							noEvent = 0;
						}
						//sinon...
					}
				}
				break;
			case SDL_QUIT:
				id = -2;
				noEvent = 0;
				break;
			default:
				noEvent = 1;
				break;
		}
	}
	return (unsigned int)id;
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
void printColourOfPlayer(int id){
    switch (id){ // On définit les couleurs des joueurs
        case 0:
            printf("jaune\n");
            break;
        case 1:
            printf("blanc\n");
            break;
        case 2:
            printf("cyan\n");
            break;
        case 3:
            printf("rouge\n");
            break;
        case 4:
            printf("bleu\n");
            break;
        case 5:
            printf("rose\n");
            break;
        case 6:
            printf("orange\n");
            break;
        case 7:
            printf("vert\n");
            break;
		default:
			printf("Pas de couleur\n");
			break;
    }
}

// Fonction qui renvoie le nombre de renforts d'un joueur ayant l'id : idPlayer
// On fait le max du nombre de SCell de ses composantes connexes
int calcReinforcements(MapContext *mapContext, int nbPlayer, int idPlayer){

	// On fait les mallocs des structures nécessaires
	PlayerIslets *player = malloc(sizeof(PlayerIslets));


	int maxIslets; // Nombre max de cellules, et donc d'ilots, par joueur
	// Nombre de SCell minimum par joueur
	unsigned int nbCellsMin = mapContext->nbNodes / nbPlayer;
	// Nombre de SCell en plus (distribuées sur les premiers joueurs)
	unsigned int nbCellsLeft = mapContext->nbNodes % nbPlayer;

	if (nbCellsLeft){
		maxIslets = nbCellsMin + 1;
	} else {
		maxIslets = nbCellsMin;
	}

	player->islet = malloc(sizeof(Islet)*maxIslets);
	player->allMyCells = malloc(sizeof(SCell)*maxIslets);
	player->nbIslets = 0;
	player->nbOfCells = 0;

	for (int i=0; i<maxIslets; i++){
		player->islet[i].cells = malloc(sizeof(SCell)*maxIslets);
		player->islet[i].nbCells = 0;
	}

    getAllCells(player, mapContext, idPlayer);

    assembleIslets(player, idPlayer);

	int reinforcements = maxConnex(player);
	// faire les free

	return reinforcements;
}

void getAllCells(PlayerIslets *player, MapContext *mapContext, int idPlayer){
    int index = 0;
    for (int i=0; i<mapContext->nbNodes; i++){
        if (mapContext->map->cells[i].owner == idPlayer){
            player->allMyCells[index] = mapContext->map->cells[i];
			player->nbOfCells+=1;
			index++;
        }
    }
}

void assembleIslets(PlayerIslets *player, int idPlayer){
	for (int i=0; i<player->nbOfCells; i++){
		if (!cellInIslet(player, player->allMyCells[i].id)){ // Si la cellule n'a pas été visitée
			player->nbIslets++; // On a donc une composante connexe en plus
			DFS(player, player->allMyCells[i], idPlayer); // DFS sur elle
		}
	}
}

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

int cellInIslet(PlayerIslets *player, int id){
    int found = 0;
	for (int i=0; i<player->nbIslets && !found; i++){
		for (int j=0; j<player->islet[i].nbCells; j++){
			if (player->islet[i].cells[j].id == id) {
				found = 1;
			}
		}
    }
    return found;
}