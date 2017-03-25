#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include <math.h>
#include "map.h"

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

	unsigned int minDist = abs(cellsList[0].x - x) + abs(cellsList[0].y - y);// Distance de manatthan
	unsigned int minIndex = 0;
	unsigned int dist;

	for (int i=1; i<size; i++) {
		dist = abs(cellsList[i].x - x) + abs(cellsList[i].y - y); // Distance de manatthan
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
		SDL_PollEvent(&event);
		switch(event.type){
			case SDL_MOUSEBUTTONDOWN:
				if (SDL_BUTTON_LEFT){
					id = getCloser(cellsList, nbNodes, event.button.x, event.button.y).id;
					noEvent = 0;
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
