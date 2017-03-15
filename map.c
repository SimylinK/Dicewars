#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "map.h"
#include "util.h"

// Fichier chargé de déterminer les paramètres aléatoires et de créer la map

void initMap(int nbPlayer){

	SMap *map = malloc(sizeof(SMap));
	// Nombre de cases sur la map
	unsigned int nbNodes = randomBounds(30, 60);
	assignSCell(nbPlayer, nbNodes, map);
	displayMap();

}

// Assigne les SCell aux joueurs
void assignSCell(unsigned int nbPlayer, unsigned int nbNodes, SMap *map) {

	// Initialisation de la map
	map->nbCells = nbNodes;
	map->cells = malloc(sizeof(SCell) * nbNodes);
	map->stack = malloc(sizeof(unsigned int) * nbPlayer);

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

	SDL_Init(SDL_INIT_VIDEO);              // Initialise SDL2

	// Crée une fenêtre SDL:
	window = SDL_CreateWindow(
			"Dicewars",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			1024,                               // width, in pixels
			768,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Initialise le renderer et la texture
	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);

	SDL_SetRenderDrawColor(renderer, 77, 77, 77, 255); // On définit la couleur
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	while(1){
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT)
			break;

		SDL_Point points[100];
		for (int i=0; i<99;i++){
			points[i].x = i;
			points[i].y = i;
		}



		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // On définit la couleur
		SDL_RenderDrawPoints(renderer, points, 100);
		SDL_RenderPresent(renderer);

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
