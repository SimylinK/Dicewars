#ifndef _ARBITER_H
#define _ARBITER_H

    #include "utils/util.h"

    int whoWins(SCell cellFrom, SCell cellTo);
    int checkMove(STurn *turn, MapContext *mapContext);
    void getNbPlayersCells(int *nbPlayersCells, int nbOlayer, SCell *cells, int nbCells);
    int gameIsOver(int *nbPlayersCells, int nbPlayer);

#endif //_ARBITER_H
