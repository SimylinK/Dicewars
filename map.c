#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "map.h"
#include "util.h"

// Fichier chargé de déterminer les paramètres aléatoires et de créer la map

void initMap(unsigned int nbPlayer){

	SMap *map = malloc(sizeof(SMap));
	// Nombre de cases sur la map
	unsigned int nbNodes = randomBounds(30,60);
	assignSCell(nbPlayer, nbNodes, map);
	Pixel** graph = malloc(WIDTH * sizeof(Pixel*));
	for (int i=0; i< WIDTH; i++) graph[i] = malloc(HEIGHT * sizeof(Pixel));
	generateGraph(&graph, nbNodes, map);
	printf("graph generated\n");
	displayMap();

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

void displayMap(){

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

	SDL_Point points[100];
	for (int i=0; i<99;i++) {
		points[i].x = WIDTH;
		points[i].y = i;

	}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // On définit la couleur
		SDL_RenderDrawPoints(renderer, points, 100);
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
	int same = 0;
	for (int i=0; i<nbNodes; i++){
		same = 0;

		do {
			x = randomBounds(0, WIDTH);
			y = randomBounds(0, HEIGHT);

			// On vérifie que ces coordonnées n'ont pas déjà été tirées par une autre SCell
			for (int j = 0; j<i; j++){
				if (cellsList[j].x == x && cellsList[j].y == y){
					same = 1;
				}
			}
		} while (same==1);



	}

	 // On trouve les distances minimales
	double minDist;
	int minIndex;
	double dist;
	for (unsigned int x=0; x<WIDTH; x++){
		for (unsigned int y=0; y<HEIGHT; y++){
			// On trouve le SCell le plus près dans la liste de SCells
			minDist = sqrt(pow((cellsList[0].x - (double)x), 2) + pow((cellsList[0].y - (double)y), 2));
			minIndex = 0;

			for (int i=1; i<nbNodes; i++) {
				dist = sqrt(pow((cellsList[i].x - x), 2) + pow((cellsList[i].y - y), 2)); // Distance euclidienne
				if (dist < minDist) {
					minIndex = i;
					minDist = dist;
				}
			}
			(*graph)[x][y].owner = cellsList[minIndex].owner; // On associe au pixel le même owner et id de SCell
			(*graph)[x][y].id = cellsList[minIndex].id;
		}
	}
}