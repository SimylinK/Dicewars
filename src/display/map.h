#ifndef DICEWARS_MAP_H
#define DICEWARS_MAP_H
	#include <SDL2/SDL.h>
	#include "../utils/interface.h"
	#include "../utils/init.h"

	void mainMap(MapContext *mapContext);
	Centre* generateList(int nbNodes, SMap *map);
	void assignNeighbor(int id1, int id2, SMap *map);
	void drawMap(Centre* cellsList, unsigned int nbNodes, Graph *graph);
	void destroyWindow();
	SMap* copyMap(MapContext *mapContextToCopy, int nbPlayer);
    void updateMapContext(SMap *mapCopy, MapContext *mapContextToUpdate);
    void drawPixels(SDL_Renderer* renderer, Graph *graph);

	void drawBorders(SDL_Renderer* renderer, Graph *graph);


	//pour insérer une image BITMAP sur une fenêtre déjà existante /!\ seulement une image bitmap
	//x et y étant le centre de l'image, width et height sa hauteur et sa largeur souhaitées
	//name doit être l'adresse où se trouve l'image
	void insertPicture(char* name, SDL_Window* window, int x, int y, int width, int height);

    // Pour afficher le nombre de dés présent sur chaque cellule au centre de celle-ci
    void displayDices(SDL_Window *window, Centre *cellsList, int nbNodes);

	void drawMapTurn(Centre *cellsList, unsigned int nbNodes, Graph *graph, int idPlayer, SMap *map);
	void drawDiceOfPlayer(int id);
	void drawStackOfPlayer(SMap *map, int id);


#endif //DICEWARS_MAP_H