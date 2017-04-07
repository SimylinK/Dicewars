#ifndef _OUTPUT_H
#define _OUTPUT_H

	#include "../utils/interface.h"

	//chemin vers le fichier de sortie
	#define OUTPUTFILE "resultats"

	void outputOpen();
	void outputClose();
	void outputPlayer(int id, SPlayerInfo player);
	void outputInit(SMap *map);
	void outputTurn(int cellFrom, int cellTo, int attackWin);
	void outputEndGame(int idWin);

#endif //_OUTPUT_H
