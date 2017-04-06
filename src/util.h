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
    SCell *allMyCells;
    int nbOfCells;
}PlayerIslets;

	unsigned int goodRandom(unsigned int max);
	unsigned int randomBounds(unsigned int min, unsigned int max);
	unsigned int distRandomBounds(unsigned int dist, unsigned int min, unsigned int max, Centre *cellsList, unsigned int taille, unsigned int x);
	Centre getCloser(Centre* cellsList,unsigned int size, unsigned int x, unsigned int y);
	unsigned int getIdOnClick(unsigned int nbNodes, Centre *cellsList);
	int isNeighbor(SCell *cell1, SCell *cell2);
	void printColourOfPlayer(int id);

	void giveReinforcements(MapContext *mapContext, int nbPlayer, int idPlayer);
	int calcReinforcements(PlayerIslets *player, MapContext *mapContext, int idPlayer);
	int maxConnex(PlayerIslets *player);
    int cellInIslet(PlayerIslets *player, int id);
    void getAllCells(PlayerIslets *player, MapContext *mapContext, int idPlayer);
    void assembleIslets(PlayerIslets *player, int idPlayer);
    void DFS(PlayerIslets *player, SCell cell, int idPlayer);
    int cellsFull(PlayerIslets *player, MapContext *mapContext);

	void freeDl(SInterface *interfaces, int nbIA);


#endif
