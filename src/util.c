#include <stdlib.h>
#include <stdio.h>
#include "util.h"

// Retourne un insigned int entre 0 et max
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

unsigned int randomBounds(unsigned int min, unsigned int max){
	return min + goodRandom(max-min);
}