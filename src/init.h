#ifndef DICEWARS_INIT_H
#define DICEWARS_INIT_H

	#include "interface.h"

	// Initialisation de la map
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


	// Initialisation des joueurs
	typedef void (*pfInitGame)(unsigned int, unsigned int, SPlayerInfo*);
	typedef void (*pfPlayTurn)(const SMap*, STurn*);
	typedef void (*pfEndGame)(unsigned int);

	typedef struct  {
		pfInitGame InitGame;
		pfPlayTurn PlayTurn;
		pfEndGame EndGame;
	} SInterface;

	int initPlayers(int nbPlayer, SInterface **interfaces, int argc, char *argv[]);



#endif //DICEWARS_INIT_H
