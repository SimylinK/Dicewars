#ifndef DICEWARS_UTILIA_H
#define DICEWARS_UTILIA_H
#include "../utils/interface.h"

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
int calcReinforcements(const SMap *map, int idPlayer, PlayerIslets* player);
int maxConnex(PlayerIslets *player);
int cellInIslet(PlayerIslets *player, int id);
void getAllCells(PlayerIslets *player, const SMap *map, int idPlayer);
void assembleIslets(PlayerIslets *player, int idPlayer);
void DFS(PlayerIslets *player, SCell cell, int idPlayer);

// L'ia en elle-même
int pickBestTurns(PlayerIslets *player, STurn *playableTurns, STurn *bestTurns, int nbOfPlayableTurns, const SMap *map, int id, int reinforcements);
void clonePlayer(PlayerIslets *playerToClone, PlayerIslets *playerCloned, int nbCells);
void freePlayer(PlayerIslets *player, int nbCells);
void resetPlayer(PlayerIslets *player);

#endif //DICEWARS_UTILIA_H
