#ifndef _ARBITER_H
#define _ARBITER_H

    #include "utils/interface.h"
    #include "display/map.h"
    #include "utils/init.h"
    #include "utils/util.h"


	void gameLoop(MapContext *mapContext, SPlayer *players, SInterface *interfaces, int nbPlayer, int isFirst, int isLast);
	void runTurn(STurn *turn, MapContext *mapContext);
	int whoWins(SCell cellFrom, SCell cellTo);
	void updateDices(SCell *cellWinner, SCell *cellLoser, int attackWin);
	int checkMove(STurn *turn, MapContext *mapContext);
	void destroyMap(MapContext *mapContextToDestroy);
    void getNbPlayersCells(int *nbPlayersCells, int nbOlayer, SCell *cells, int nbCells);
    int gameIsOver(int *nbPlayersCells, int nbPlayer);

#endif //_ARBITER_H
