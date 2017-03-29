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
int getIdOnClick(int nbNodes, Centre *cellsList){
	SDL_Event event;
	int id;
	int noEvent = 1;

	while (noEvent){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_MOUSEBUTTONDOWN:
				if (SDL_BUTTON_LEFT){

					//si le clic se fait sur la zone de la map
					if (event.button.y<HEIGHT
						&& event.button.x<WIDTH){

						id = getCloser(cellsList, nbNodes, event.button.x, event.button.y).cell->id;
						noEvent = 0;

					} else {
						//si le clic se fait sur le bouton tour suivant
						if(event.button.y>BUTTONY
						   && event.button.y<(BUTTONY + BUTTONH)
						   && event.button.x>BUTTONX
						   && event.button.x<(BUTTONX + BUTTONW)){

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
		}
	}
	return id;
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
    }
}
