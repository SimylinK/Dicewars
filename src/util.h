#ifndef _UTIL_H
#define _UTIL_H
	#include "map.h"

	unsigned int goodRandom(unsigned int max);
	unsigned int randomBounds(unsigned int min, unsigned int max);
	unsigned int distRandomBounds(unsigned int dist, unsigned int min, unsigned int max, Centre *cellsList, unsigned int taille, unsigned int x);

#endif
