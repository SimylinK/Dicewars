#ifndef DICEWARS_INIT_H
#define DICEWARS_INIT_H

	#include "interface.h"

	#define WIDTH 1024
	#define HEIGHT 768
	#define nbDicesPerCell 3
	#define minDistBetweenCells 0 // distance en pixels

// Une SCell avec des coordonnées
	typedef struct Centre{

		SCell *cell;
		int x;
		int y;
	}Centre;

	typedef struct MapContext{
			SMap *map;
			Centre* cellsList;
			int nbNodes;
		} MapContext;

	MapContext* initMap(MapContext *mapContext, unsigned int nbPlayer);
	void assignSCell(unsigned int nbPlayer, unsigned int nbNodes, SMap *map);
	void giveDices(unsigned int nbPlayer, unsigned int nbNodes, SMap *map);
	void neighbors(Centre *cellsList, SMap *map, unsigned int nbNodes);


#endif //DICEWARS_INIT_H
