#ifndef DICEWARS_MAP_H
#define DICEWARS_MAP_H
	#include "interface.h"
	#include "init.h"
	#include <SDL2/SDL.h>

	void mainMap(MapContext *mapContext);
	void displayMap(Centre* cellsList, int nbNodes, SMap *map, SDL_Window *window, SDL_Renderer* renderer);
	Centre* generateList(int nbNodes, SMap *map);
	//void drawBorders(SDL_Renderer* renderer, Centre *cellsList, unsigned int nbNodes);
	void assignNeighbor(int id1, int id2, SMap *map);
//	void drawPixels(SDL_Renderer* renderer, Centre* cellsList, unsigned int nbNodes);
	void drawMap(Centre* cellsList, unsigned int nbNodes, Graph *graph);
	void destroyWindow(SDL_Window *window, SDL_Renderer* renderer);
	void drawScore(int joueur, int diceValue, int i);
    SMap* copyMap(MapContext *mapContextToCopy, int nbPlayer);
    void updateMapContext(SMap *mapCopy, MapContext *mapContextToUpdate);
    void drawPixelsAlt(SDL_Renderer* renderer, Graph *graph);

//CHANGEMENT
void drawBordersAlt(SDL_Renderer* renderer, Graph *graph);



	//pour insérer une image BITMAP sur une fenêtre déjà existante /!\ seulement une image bitmap
	//x et y étant le centre de l'image, width et height sa hauteur et sa largeur souhaitées
	//name doit être l'adresse où se trouve l'image
	void insertPicture(char* name, SDL_Window* window, int x, int y, int width, int height);

    // Pour afficher le nombre de dés présent sur chaque cellule au centre de celle-ci
    void displayDices(SDL_Window *window, Centre *cellsList, int nbNodes);

#endif //DICEWARS_MAP_H
