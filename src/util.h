#ifndef _UTIL_H
#define _UTIL_H
	#include "map.h"

// Un ilot définit un ensemble connexe
typedef struct Islet{
	SCell *cells;
	int nbCells;
}Islet;

typedef struct PlayerIslets{
	Islet *islet;
	int nbIslets;
}PlayerIslets;

unsigned int goodRandom(unsigned int max);
	unsigned int randomBounds(unsigned int min, unsigned int max);
	unsigned int distRandomBounds(unsigned int dist, unsigned int min, unsigned int max, Centre *cellsList, unsigned int taille, unsigned int x);
	Centre getCloser(Centre* cellsList,unsigned int size, unsigned int x, unsigned int y);
	unsigned int getIdOnClick(unsigned int nbNodes, Centre *cellsList);
	int isNeighbor(SCell *cell1, SCell *cell2);
	void printColourOfPlayer(int id);
	void generateMatrix(MapContext* mapContext, int*** matrix);
	int calcReinforcements(int **matrix, MapContext *mapContext, int nbPlayer, int idPlayer);
	void createIslets (PlayerIslets *player,int** matrix, MapContext *mapContext, int idPlayer, int nbPlayer);
	int maxConnex(PlayerIslets *player);
	int getIndex(PlayerIslets *player, int id, MapContext* mapContext);




#endif
