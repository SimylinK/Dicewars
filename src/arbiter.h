#ifndef _ARBITER_H
#define _ARBITER_H

    #include "init.h"
    #include "interface.h"
    #include "map.h"
    #include "util.h"

void gameLoop(MapContext *mapContext, SInterface **interfaces, int nbPlayer);

int runTurn(STurn *turn, MapContext *mapContext);
int whoWins(SCell cellFrom, SCell cellTo);
void updateDices(SCell *cellWinner, SCell *cellLoser);
int checkMove(STurn *turn, MapContext *mapContext);
int isNeighbor(SCell *cell1, SCell *cell2, MapContext *mapContext);

#endif //_ARBITER_H
