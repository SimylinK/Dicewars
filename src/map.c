#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <zconf.h>
#include "map.h"
#include "util.h"

// Fichier chargé de déterminer les paramètres aléatoires et de créer la map
void initMap(unsigned int nbPlayer) {

	// Nombre de SCell sur la map
	unsigned int nbNodes = randomBounds(30 , 60);

	// Initialisation de la map
	SMap *map = malloc(sizeof(SMap));
	map->nbCells = nbNodes;
	map->cells = malloc(sizeof(SCell) * nbNodes);
	for (int i = 0; i < nbNodes; i++) {
		// On s'assure de mettre tous les pointeurs à 0 au début
		map->cells[i].neighbors = calloc(nbNodes, sizeof(SCell *) * nbNodes);
	}
	map->stack = malloc(sizeof(unsigned int) * nbPlayer);

	// On assigne les SCell aux joueurs
	assignSCell(nbPlayer, nbNodes, map);

	// On génère la liste des SCell
	Centre *cellsList = generateList(nbNodes, map);

	// Boucle d'affichage principale
	displayMap(cellsList, nbNodes, map);

	//Destruction des ressources
}

// Assigne les SCell aux joueurs
void assignSCell(unsigned int nbPlayer, unsigned int nbNodes, SMap *map) {

	// Attribue aux SCell leurs ID, l'ID du joueur qui la possède
	int idPlayer = 0;
	for (int i = 0; i < nbNodes; i++) {
		map->cells[i].id = i;
		map->cells[i].owner = idPlayer;
		map->cells[i].nbDices = 0;

		idPlayer++;
		if (idPlayer == nbPlayer) idPlayer = 0;
	}
	// On donne les dés
	giveDices(nbPlayer, nbNodes, map);
}

// Idée : on AJOUTE (+=) par un random des dés à chaque cellule, et si la somme au final n'est pas à 0, on rerentre dans la boucle
void giveDices(unsigned int nbPlayer, unsigned int nbNodes, SMap *map) {

	// Nombre de SCell minimum par joueur
	int nbCellsMin = nbNodes/nbPlayer;
	// Nombre de SCell en plus (distribuées sur les premiers joueurs)
	int nbCellsLeft = nbNodes%nbPlayer;

	// Nombre de SCell exact par joueur
	int nbCellPerPlayer[nbPlayer];
	for (int i=0; i<nbPlayer; i++){
		if (nbCellsLeft != 0){
			nbCellPerPlayer[i] = nbCellsMin +1;
			nbCellsLeft--;
		} else
			nbCellPerPlayer[i] = nbCellsMin;
	}

	// On considère que le nombre de dés par SCell est environ de 3
	unsigned int dicesToGive[nbPlayer];
	for (int i=0; i<nbPlayer; i++) {
		dicesToGive[i] = (nbNodes * nbDicesPerCell)/ nbPlayer;
	}

	int somme;
	unsigned int dicesGiven = 0;
	int idPlayer;
	do {
		somme = 1; // La somme a été atteinte

		idPlayer = 0;
		for (int i = 0; i < nbNodes; i++) {
			if (dicesToGive[idPlayer] >=  nbCellPerPlayer[idPlayer])
				dicesGiven = randomBounds(1, floor(dicesToGive[idPlayer]/nbCellPerPlayer[idPlayer]))%(9-map->cells[i].nbDices); // On ne donne pas plus qu'on peut et on excède pas 8 dés par case
			else if (dicesToGive[idPlayer] + map->cells[i].nbDices > 8)
				dicesGiven = 1;
			else
				dicesGiven = dicesToGive[idPlayer];

			map->cells[i].nbDices += dicesGiven;
			dicesToGive[idPlayer] -= dicesGiven;

			idPlayer++;
			if (idPlayer == nbPlayer) idPlayer = 0;
			if (i== nbNodes -1) ;

		}
		// On vérifie que les sommes sont toutes à 0, sinon on recommence
		for (int j = 0; j<nbPlayer; j++) {
			if (dicesToGive[j] != 0) {
				somme = 0; // La somme n'a pas été atteinte
			}
		}
	} while (!somme);
}


void displayMap(Centre* cellsList, int nbNodes, SMap *map){

	SDL_Window *window;
	SDL_Renderer* renderer;
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

	// Initialise le renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	drawMap(map, window, renderer, cellsList, nbNodes);
	sleep(2);
	drawMap(map, window, renderer, cellsList, nbNodes);

	while(!end){
		while(SDL_PollEvent(&event)) {// WaitEvent ou PollEvent ?
			if (event.type == SDL_QUIT)
				end = 1;
		}

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


// Popule un tableau avec des Centre aléatoirement répartis sur l'image
Centre* generateList(int nbNodes, SMap *map){

	// On attribue à chaque SCell une place aléatoire dans ce tableau
	Centre *cellsList = malloc(sizeof(Centre)*(map->nbCells));
	unsigned int x;
	unsigned int y;
	int same;
	for (unsigned int i=0; i<nbNodes; i++){
		do {
			same = 0;
			x = distRandomBounds(minDistBetweenCells, minDistBetweenCells, WIDTH-minDistBetweenCells, cellsList, i, 1); // On force à être éloigné des bords
			y = distRandomBounds(minDistBetweenCells, minDistBetweenCells, HEIGHT-minDistBetweenCells, cellsList, i, 0);

			// On vérifie que ces coordonnées n'ont pas déjà été tirées par une autre SCell
			for (int j = 0; j<i; j++){
				if (cellsList[j].x == x && cellsList[j].y == y){
					same = 1;
				}
			}
		} while (same);

		cellsList[i].x = x;
		cellsList[i].y = y;
		cellsList[i].owner = map->cells[i].owner;
		cellsList[i].id = map->cells[i].id;
	}
	return cellsList;
}

// Colore les pixels de bordure en noir
void drawBorders(SMap *map, SDL_Window *window, SDL_Renderer* renderer, Centre *cellsList, int nbNodes){

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // noir

	Centre closer;
	Centre rightNeighborCloser;
	Centre downNeighborCloser;
	for (unsigned int x = 0; x < WIDTH - 1; x++) {
		for (unsigned int y = 0; y < HEIGHT - 1; y++) {
			// Quand on trouve un changement d'id
			closer = getCloser(cellsList, nbNodes, x, y);
			rightNeighborCloser = getCloser(cellsList, nbNodes, x+1, y);
			downNeighborCloser = getCloser(cellsList, nbNodes, x, y+1);
			if (closer.id != rightNeighborCloser.id)
			{
				// On assigne les voisins dans les deux sens
				assignNeighbor(closer.id, rightNeighborCloser.id, map);
				assignNeighbor(rightNeighborCloser.id, closer.id, map);

				// On dessine en noir
				SDL_RenderDrawPoint(renderer, x, y);


			} else if  (closer.id != downNeighborCloser.id) {
				// On assigne les voisins dans les deux sens
				assignNeighbor(closer.id, downNeighborCloser.id, map);
				assignNeighbor(downNeighborCloser.id, closer.id, map);

				// On dessine en noir
				SDL_RenderDrawPoint(renderer, x, y);

			}
		}
	}
}

void assignNeighbor(int id1, int id2, SMap *map) {
	// On assigne seulement si id2 n'est pas déjà dans les voisins de id1
	if (map->cells[id1].neighbors[id2] != &(map->cells[id2])) {
		map->cells[id1].neighbors[id2] = &(map->cells[id2]);
		map->cells[id1].nbNeighbors++;
	}
}

void drawPixels(SDL_Window *window, SDL_Renderer* renderer, Centre *cellsList, int nbNodes){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Couleur du background
	Centre closer; // Le centre le plus près
	for (unsigned int x=0; x<WIDTH; x++){
		for (unsigned int y=0; y<HEIGHT; y++){
			closer=getCloser(cellsList, nbNodes, x, y);
			switch (closer.owner){ // On définit les couleurs des joueurs
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
			}
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
}

void insertPicture(char* name, SDL_Window* window, int x, int y, int width, int height)
{
	SDL_Renderer* renderer = SDL_GetRenderer(window);
	SDL_Surface* surface = NULL;
	SDL_Texture* texture;

	int w_width = 0, w_height = 0; //largeur et hauteur de la fenêtre

	surface = SDL_LoadBMP(name);

	texture = SDL_CreateTextureFromSurface(renderer,surface); // Préparation du sprite
	surface->w = width; surface->h = height;//on adapte la taille de la surface aux dimensions demandées en paramètres


	SDL_GetWindowSize(window, &w_width, &w_height);
	SDL_Rect dest = {x - surface->w/2, y - surface->h/2, surface->w, surface->h};
	SDL_RenderCopy(renderer,texture,NULL,&dest); // Copie du sprite grâce au SDL_Renderer
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(texture); // Libération de la mémoire associée à la texture
	SDL_FreeSurface(surface); // Libération de la ressource occupée par le sprite
}

void displayDices(SMap *map,  SDL_Window *window, Centre *cellsList, int nbNodes)
{
    for (int i=0; i<nbNodes; i++) {
        if (map->cells[i].nbDices == 1) {
            insertPicture("../sprites/1.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 2) {
            insertPicture("../sprites/2.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 3) {
            insertPicture("../sprites/3.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 4) {
            insertPicture("../sprites/4.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 5) {
            insertPicture("../sprites/5.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 6) {
            insertPicture("../sprites/6.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 7) {
            insertPicture("../sprites/7.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 8) {
            insertPicture("../sprites/8.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        }
    }
}

void drawMap(SMap *map, SDL_Window *window, SDL_Renderer* renderer, Centre *cellsList, int nbNodes){

	// On dessine les pixels
	drawPixels(window, renderer, cellsList, nbNodes);

	// On dessine les bordures
	drawBorders(map, window, renderer, cellsList, nbNodes);

	// On affiche les dés
	displayDices(map, window, cellsList, nbNodes);
}