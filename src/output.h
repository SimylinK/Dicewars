#include "interface.h"

void outputOpen();
void outputClose();
void outputPlayer(int id, SPlayerInfo player);
void outputInit(SMap *map);
void outputTurn(int cellFrom, int cellTo, int attackWin);
void outputEndGame(int idWin);
