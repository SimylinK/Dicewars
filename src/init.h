#ifndef DICEWARS_INIT_H
#define DICEWARS_INIT_H

	#include "interface.h"

	// Initialisation de la map
	#define WIDTH 800 //largeur de la map
	#define HEIGHT 450 //hauteur de la map
	#define WWIDTH 800 //largeur de la fenetre
	#define WHEIGHT 600 //hauteur de la fenetre
	#define nbDicesPerCell 3
	#define minDistBetweenCells 0 // distance en pixels

	#define BUTTONX 100
	#define BUTTONY 500
	#define BUTTONW 100
	#define BUTTONH 40


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
	typedef int (*pfPlayTurn)(const SMap*, STurn*);
	typedef void (*pfEndGame)(unsigned int);

	typedef struct  {
		pfInitGame InitGame;
		pfPlayTurn PlayTurn;
		pfEndGame EndGame;
	} SInterface;

	int initPlayers(int nbPlayer, SInterface **interfaces, int argc, char *argv[]);



#endif //DICEWARS_INIT_H
