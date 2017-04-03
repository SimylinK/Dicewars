#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "interface.h"


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

// Génère une matrice d'adjacence
// Pour la lire, aller pour i de 0 à nbNodes, pour j de i+1 à nbNodes : elle est symétrique à diagonale nulle
void generateMatrix(MapContext* mapContext, int*** matrix){

	for (int i=0; i<mapContext->nbNodes; i++){
		for (int j=0; j<mapContext->nbNodes; j++){
			// Si les cellules sont voisines
			if(isNeighbor(&mapContext->map->cells[i], &mapContext->map->cells[j])){
				(*matrix)[i][j] = 1;
			} else{
				(*matrix)[i][j] = 0;
			}
		}
	}
}