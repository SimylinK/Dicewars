#ifndef DICEWARS_MAP_H
#define DICEWARS_MAP_H
	#include "interface.h"
	#define WIDTH 1024
	#define HEIGHT 768

typedef struct Point{

	unsigned int x;
	unsigned int y;
	int id;	// Id de la cellule
	int owner;	// Id du joueur qui la possède
	}Point;

	void initMap(unsigned int nbPlayer);
	void assignSCell(unsigned int nbPlayer, unsigned int nbNodes, SMap *map);
	void displayMap();
	void generateGraph(Point*** graph, int nbNodes, SMap *map);



#endif //DICEWARS_MAP_H
