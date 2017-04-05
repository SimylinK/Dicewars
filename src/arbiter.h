#ifndef _ARBITER_H
#define _ARBITER_H

    #include "init.h"
    #include "interface.h"
    #include "map.h"
    #include "util.h"


	void gameLoop(MapContext *mapContext, SPlayer *players, SInterface *interfaces, int nbPlayer);
	void runTurn(STurn *turn, MapContext *mapContext);
	int whoWins(SCell cellFrom, SCell cellTo);
	void updateDices(SCell *cellWinner, SCell *cellLoser, int attackWin);
	int checkMove(STurn *turn, MapContext *mapContext);
	void destroyMap(SMap *mapToDestroy);

#endif //_ARBITER_H
