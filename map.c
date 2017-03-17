#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "map.h"
#include "util.h"
#include "interface.h"

// Fichier chargé de déterminer les paramètres aléatoires et de créer la map

void initMap(unsigned int nbPlayer){

	SMap *map = malloc(sizeof(SMap));
	// Nombre de SCell sur la map
	unsigned int nbNodes = randomBounds(30, 60);

	assignSCell(nbPlayer, nbNodes, map);

	Pixel** graph = malloc(WIDTH * sizeof(Pixel*));
	for (int i=0; i< WIDTH; i++) graph[i] = malloc(HEIGHT * sizeof(Pixel));


	generateGraph(&graph, nbNodes, map);

	generateBorders(&graph, map);
	displayMap(graph);
}

// Assigne les SCell aux joueurs
void assignSCell(unsigned int nbPlayer, unsigned int nbNodes, SMap *map) {

	// Initialisation de la map
	map->nbCells = nbNodes;
	map->cells = malloc(sizeof(SCell)*nbNodes);
	map->stack = malloc(sizeof(unsigned int)*nbPlayer);

	// Attribue aux SCell leurs ID, l'ID du joueur qui la possède et le nombre de dés
	int idPlayer = 0;
	for (int i = 0; i < nbNodes; i++) {
		map->cells[i].id = i;
		map->cells[i].owner = idPlayer;
		map->cells[i].nbDices = 0;

		idPlayer++;
		if (idPlayer == nbPlayer) idPlayer = 0;
	}
}

void displayMap(Pixel** graph){

	SDL_Window *window;
	SDL_Renderer* renderer;
	SDL_Texture *texture;
	SDL_Event event;
	int end = 0;

	SDL_Init(SDL_INIT_VIDEO);              // Initialise SDL2

	// Crée une fenêtre SDL:
	window = SDL_CreateWindow(
			"Dicewars",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			WIDTH,                               // width, in pixels
			HEIGHT,                               // height, in pixels
			SDL_WINDOW_SHOWN                  // flags - see below
	);

	// Initialise le renderer et la texture
	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // On définit la couleur
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	for (unsigned int x=0; x<WIDTH; x++){
		for (unsigned int y=0; y<HEIGHT; y++){
			switch (graph[x][y].owner){ // On définit les couleurs des joueurs
				case 0:
					SDL_SetRenderDrawColor(renderer, 255, 255, 0,255); // jaune
					break;
				case 1:
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // blanc
					break;
				case 2:
					SDL_SetRenderDrawColor(renderer, 20, 134, 107, 255); // cyan
					break;
				case 3:
					SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255); // rouge
					break;
				case 4:
					SDL_SetRenderDrawColor(renderer, 0, 66, 100, 255); // bleu
					break;
				case 5:
					SDL_SetRenderDrawColor(renderer, 229, 91, 176, 255); // rose
					break;
				case 6:
					SDL_SetRenderDrawColor(renderer, 255, 60, 4, 255); // orange
					break;
				case 7:
					SDL_SetRenderDrawColor(renderer, 22, 128, 0, 255); // vert
					break;
				case 8: // Bordure
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // noir
					break;
			}
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}

	SDL_RenderPresent(renderer);

	while(!end){
		while(SDL_PollEvent(&event)) {// WaitEvent ou PollEvent ?
			if (event.type == SDL_QUIT)
				end = 1;
		}

	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


// Popule un tableau avec les SCell
void generateGraph(Pixel*** graph, int nbNodes, SMap *map){

	// On attribue à chaque SCell une place aléatoire dans ce tableau
	Centre *cellsList = malloc(sizeof(Centre)*(map->nbCells));
	unsigned int x;
	unsigned int y;
	int same;
	for (int i=0; i<nbNodes; i++){
		do {
			same = 0;
			x = randomBounds(0, WIDTH);
			y = randomBounds(0, HEIGHT);

			// On vérifie que ces coordonnées n'ont pas déjà été tirées par une autre SCell
			for (int j = 0; j<i; j++){
				if (cellsList[j].x == x && cellsList[j].y == y){
					same = 1;
				}
			}
		} while (same==1);

		cellsList[i].x = x;
		cellsList[i].y = y;
		cellsList[i].owner = map->cells[i].owner;
		cellsList[i].id = map->cells[i].id;
	}

	 // On trouve les distances minimales
	double minDist;
	int minIndex;
	double dist;
	for (unsigned int x=0; x<WIDTH; x++){
		for (unsigned int y=0; y<HEIGHT; y++){
			// On trouve le SCell le plus près dans la liste de SCells
			// minDist = sqrt(pow((cellsList[0].x - (double)x), 2) + pow((cellsList[0].y - (double)y), 2));// Distance euclidienne
			minDist = abs(cellsList[0].x - x) + abs(cellsList[0].y - y);// Distance de manatthan
			minIndex = 0;

			for (int i=1; i<nbNodes; i++) {
				// dist = sqrt(pow((cellsList[i].x - (double)x), 2) + pow((cellsList[i].y - (double)y), 2)); // Distance euclidienne
				dist = abs(cellsList[i].x - x) + abs(cellsList[i].y - y); // Distance de manatthan
				if (dist <= minDist) {
					minIndex = i;
					minDist = dist;
				}
			}
			(*graph)[x][y].owner = cellsList[minIndex].owner; // On associe au pixel le même owner et id de SCell
			(*graph)[x][y].id = cellsList[minIndex].id;
		}
	}
}

// Change les pixels
void generateBorders(Pixel*** graph, SMap *map) {
	for (unsigned int x = 0; x < WIDTH - 1; x++) {
		for (unsigned int y = 0; y < HEIGHT - 1; y++) {
			if ((*graph)[x][y].id != (*graph)[x + 1][y].id)
			{
				assignNeighbor((*graph)[x][y].id, (*graph)[x + 1][y].id, map);
				(*graph)[x][y].owner = 8;

			} else if  ((*graph)[x][y].id != (*graph)[x][y + 1].id) {
				assignNeighbor((*graph)[x][y].id, (*graph)[x][y + 1].id, map);
				(*graph)[x][y].owner = 8;
			}
		}
	}
}

void assignNeighbor(int id1, int id2, SMap *map){
	int trouve = 0;
	int voisin = 0;
	int i = 0;
	while (!trouve && i<map->cells[id1].nbNeighbors){
		if (map->cells->neighbors[i]


	}
	for (int i=0; i<map->cells[id1].nbNeighbors; i++){

	}


}