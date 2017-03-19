#ifndef DICEWARS_MAP_H
#define DICEWARS_MAP_H
	#include "interface.h"
	#include <SDL2/SDL.h>

	#define WIDTH 1024
	#define HEIGHT 768
	#define nbDicesPerCell 3
	#define minDistBetweenCells 8 // distance en pixels

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
	void giveDices(unsigned int nbPlayer, unsigned int nbNodes, SMap *map);
	void displayMap(Pixel** graph, Centre* cellsList, int nbNodes, SMap *map);
	Centre* generateGraph(Pixel*** graph, int nbNodes, SMap *map);
	void generateBorders(Pixel*** graph, SMap *map);
	void assignNeighbor(int id1, int id2, SMap *map);
	void drawMap(Pixel **graph, SDL_Window *window, SDL_Renderer* renderer, Centre* cellsList, int nbNodes);

	//pour insérer une image BITMAP sur une fenêtre déjà existante /!\ seulement une image bitmap
	//x et y étant le centre de l'image, width et height sa hauteur et sa largeur souhaitées
	//name doit être l'adresse où se trouve l'image
	void insertPicture(char* name, SDL_Window* window, int x, int y, int width, int height);

    // Pour afficher le nombre de dés présent sur chaque cellule au centre de celle-ci
    void displayDices(SMap *map, Centre *cellsList, SDL_Window *window, int nbNodes);

#endif //DICEWARS_MAP_H
