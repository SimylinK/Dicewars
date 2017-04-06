#ifndef DICEWARS_UTILIA_H
#define DICEWARS_UTILIA_H
#include "../interface.h"

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
int isNeighbor(SCell *cell1, SCell *cell2);

// Pour les renforts
int calcReinforcements(SMap *map, int idPlayer);
int maxConnex(PlayerIslets *player);
int cellInIslet(PlayerIslets *player, int id);
void getAllCells(PlayerIslets *player, SMap *map, int idPlayer);
void assembleIslets(PlayerIslets *player, int idPlayer);
void DFS(PlayerIslets *player, SCell cell, int idPlayer);

#endif //DICEWARS_UTILIA_H
