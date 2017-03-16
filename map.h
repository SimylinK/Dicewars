#ifndef DICEWARS_MAP_H
#define DICEWARS_MAP_H
	#include "interface.h"
	#define WIDTH 1024
	#define HEIGHT 768

// Structure qui représente une cellule
typedef struct Centre{

	unsigned int x;
	unsigned int y;
	int id;	// Id de la cellule
	int owner;	// Id du joueur qui la possède
	}Centre;

typedef struct Pixel{
	int id;	// Id de la cellule
	int owner;	// Id du joueur qui la possède
} Pixel;

	void initMap(unsigned int nbPlayer);
	void assignSCell(unsigned int nbPlayer, unsigned int nbNodes, SMap *map);
	void displayMap(Pixel** graph);
	void generateGraph(Pixel*** graph, int nbNodes, SMap *map);
	void generateBorders(Pixel*** graph);



#endif //DICEWARS_MAP_H
