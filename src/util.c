#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include <math.h>

// Retourne un unsigned int entre 0 et max
// 0 <= max <= RAND_MAX
unsigned int goodRandom(unsigned int max) {

	unsigned int num_bins = max + 1;
	unsigned int num_rand = (unsigned int) RAND_MAX + 1;
	unsigned int bin_size = num_rand / num_bins;
	unsigned int defect   = num_rand % num_bins;

	unsigned int result;
	do {
		result = (unsigned int)rand();
	}

	while (num_rand - defect <= (unsigned long)result);

	// Division tronquée intentionnelle
	return result/bin_size;
}

// Retourne un unsigned int entre min et max, min<=max
unsigned int randomBounds(unsigned int min, unsigned int max){
	return min + goodRandom(max-min);
}

// Retourne un unsigned int entre min et max, avec une distance d'au moins dist à tous les elements du tableau
// Si x est à un, on teste sur x, sinon sur y
unsigned int distRandomBounds(unsigned int dist, unsigned int min, unsigned int max, Centre *cellsList, unsigned int taille, unsigned int x){
	int farEnough; //Les cells sont assez loin
	unsigned int rand = 0;
	do{
		farEnough = 1;
		rand = randomBounds(min, max);
		for (int i=0; i<taille; i++){
			if (x == 1){
				if (abs(cellsList[i].x-rand)<dist){
					farEnough = 0;
					printf("Je réattribue\n");
				}
			} else
				if (abs(cellsList[i].y-rand)<dist){
					farEnough = 0;
					printf("Je réattribue\n");
				}
		}
	} while(!farEnough);

	return rand;
}