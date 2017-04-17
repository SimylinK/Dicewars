#ifndef DICEWARS_MAP_H
#define DICEWARS_MAP_H
	#include <SDL2/SDL.h>
	#include "../utils/interface.h"
	#include "../utils/init.h"

#define NB_PLAYER_PLUS_X 500
#define NB_PLAYER_PLUS_Y 200
#define NB_PLAYER_PLUS_WIDTH 50
#define NB_PLAYER_PLUS_HEIGHT 50

#define NB_PLAYER_MOINS_X 200
#define NB_PLAYER_MOINS_Y 200
#define NB_PLAYER_MOINS_WIDTH 50
#define NB_PLAYER_MOINS_HEIGHT 50

#define NB_PLAYER_X 350
#define NB_PLAYER_Y 100
#define NB_PLAYER_WIDTH 200
#define NB_PLAYER_HEIGHT 70

#define DIGIT_NB_PLAYER_X 350
#define DIGIT_NB_PLAYER_Y 200
#define DIGIT_NB_PLAYER_WIDTH 50
#define DIGIT_NB_PLAYER_HEIGHT 50

#define NB_IA_PLUS_X NB_PLAYER_PLUS_X
#define NB_IA_PLUS_Y NB_PLAYER_PLUS_Y+200
#define NB_IA_PLUS_WIDTH NB_PLAYER_PLUS_WIDTH
#define NB_IA_PLUS_HEIGHT NB_PLAYER_PLUS_HEIGHT

#define NB_IA_MOINS_X NB_PLAYER_MOINS_X
#define NB_IA_MOINS_Y NB_PLAYER_MOINS_Y+200
#define NB_IA_MOINS_WIDTH NB_PLAYER_MOINS_WIDTH
#define NB_IA_MOINS_HEIGHT NB_PLAYER_MOINS_HEIGHT

#define NB_IA_X NB_PLAYER_X
#define NB_IA_Y NB_PLAYER_Y+200
#define NB_IA_WIDTH NB_PLAYER_WIDTH
#define NB_IA_HEIGHT NB_PLAYER_HEIGHT

#define DIGIT_NB_IA_X DIGIT_NB_PLAYER_X
#define DIGIT_NB_IA_Y DIGIT_NB_PLAYER_Y+200
#define DIGIT_NB_IA_WIDTH DIGIT_NB_PLAYER_WIDTH
#define DIGIT_NB_IA_HEIGHT DIGIT_NB_PLAYER_HEIGHT

#define OK_X 500
#define OK_Y 500
#define OK_WIDTH 50
#define OK_HEIGHT 50

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
    void drawMapTurnOnClick(Centre *cellsList, unsigned int nbNodes, Graph *graph, int idPlayer, SMap *map, int idCell);
    void drawPixelsOnClick(SDL_Renderer* renderer, Graph *graph, int idCell);
	int displayWinner(int idPlayer);
	void menuInit(unsigned int *nbPlayer, unsigned int *nbIA);
	void howManyPlayers(unsigned int *nbPlayer, unsigned int *nbIA);
	void displayNumberOfPlayer(int nbOfPlayer);
	void displayNumberOfIA(int nbOfIA);
	void drawMenu(int nbPlayer, int nbIA);

#endif //DICEWARS_MAP_H