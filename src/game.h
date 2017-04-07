#ifndef _GAME_H
    #define _GAME_H

    #include "utils/interface.h"
    #include "display/map.h"
    #include "utils/init.h"

    void gameLoop(MapContext *mapContext, SPlayer *players, SInterface *interfaces, int nbPlayer, int isFirst, int isLast);
	void runTurn(STurn *turn, MapContext *mapContext);
    void updateDices(SCell *cellWinner, SCell *cellLoser, int attackWin);
    void destroyMap(MapContext *mapContextToDestroy);

#endif //_GAME_H
