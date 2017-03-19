#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "map.h"
#include "util.h"

// Fichier chargé de déterminer les paramètres aléatoires et de créer la map
void initMap(unsigned int nbPlayer) {

	// Nombre de SCell sur la map
	unsigned int nbNodes = randomBounds(30,60);

	// Initialisation de la map
	SMap *map = malloc(sizeof(SMap));
	map->nbCells = nbNodes;
	map->cells = malloc(sizeof(SCell) * nbNodes);
	for (int i = 0; i < nbNodes; i++) {
		// On s'assure de mettre tous les pointeurs à 0 au début
		map->cells[i].neighbors = calloc(nbNodes, sizeof(SCell *) * nbNodes);
	}
	map->stack = malloc(sizeof(unsigned int) * nbPlayer);

	assignSCell(nbPlayer, nbNodes, map);

	Pixel **graph = malloc(WIDTH * sizeof(Pixel *));
	for (int i = 0; i < WIDTH; i++) graph[i] = malloc(HEIGHT * sizeof(Pixel));

	// On génère le graphe de pixels
	Centre *cellsList = generateGraph(&graph, nbNodes, map);
	// On fait les changements pour les bordures et on assigne les voisins
	generateBorders(&graph, map);
	// Boucle d'affichage principale
	displayMap(graph, cellsList, nbNodes, map);

	//Destruction des ressources
	for (int i = 0; i < WIDTH; i++) free(graph[i]);
	free(graph);
}

// Assigne les SCell aux joueurs
void assignSCell(unsigned int nbPlayer, unsigned int nbNodes, SMap *map) {

	// Attribue aux SCell leurs ID, l'ID du joueur qui la possède et le nombre de dés
	int idPlayer = 0;
	for (int i = 0; i < nbNodes; i++) {
		map->cells[i].id = i;
		map->cells[i].owner = idPlayer;
		map->cells[i].nbDices = 0;

		idPlayer++;
		if (idPlayer == nbPlayer) idPlayer = 0;
	}
	giveDices(nbPlayer, nbNodes, map);
}

// Idée : on AJOUTE (+=)	 par un random des dés à chaque cellule, et si la somme au final n'est pas à 0, on rerentre dans la boucle
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


void displayMap(Pixel** graph, Centre* cellsList, int nbNodes, SMap *map){

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

	// On dessine une première fois la map
	drawMap(graph, window, renderer, cellsList, nbNodes);

    // On affiche le nombre de dés sur chaque cellule
    displayDices(map, cellsList, window, nbNodes);

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


// Popule un tableau avec des Centre, destiné à l'affichage
Centre* generateGraph(Pixel*** graph, int nbNodes, SMap *map){

	// On attribue à chaque SCell une place aléatoire dans ce tableau
	Centre *cellsList = malloc(sizeof(Centre)*(map->nbCells));
	unsigned int x;
	unsigned int y;
	int same;
	for (int i=0; i<nbNodes; i++){
		do {
			same = 0;
			x = distRandomBounds(minDistBetweenCells, 0, WIDTH, cellsList, i+1, 1);
			y = distRandomBounds(minDistBetweenCells, 0, HEIGHT, cellsList, i+1, 0);

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

	 // On trouve les distances minimales pour chaque pixel
	double minDist;
	int minIndex;
	double dist;
	for (unsigned int x=0; x<WIDTH; x++){
		for (unsigned int y=0; y<HEIGHT; y++){
			// On trouve le SCell le plus près dans la liste de SCells
			minDist = abs(cellsList[0].x - x) + abs(cellsList[0].y - y);// Distance de manatthan
			minIndex = 0;

			for (int i=1; i<nbNodes; i++) {
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
	return cellsList;
}
// Met à 8 l'id des pixels de bordure pour la coloration en noir
void generateBorders(Pixel*** graph, SMap *map) {
	for (unsigned int x = 0; x < WIDTH - 1; x++) {
		for (unsigned int y = 0; y < HEIGHT - 1; y++) {
			// Quand on trouve un changement d'id
			if ((*graph)[x][y].id != (*graph)[x + 1][y].id)
			{
				// On assigne les voisins dans les deux sens
				assignNeighbor((*graph)[x][y].id, (*graph)[x + 1][y].id, map);
				assignNeighbor((*graph)[x + 1][y].id, (*graph)[x][y].id, map);

				// On met l'id à 8
				(*graph)[x][y].owner = 8;

			} else if  ((*graph)[x][y].id != (*graph)[x][y + 1].id) {
				// On assigne les voisins dans les deux sens
				assignNeighbor((*graph)[x][y].id, (*graph)[x][y + 1].id, map);
				assignNeighbor((*graph)[x][y + 1].id, (*graph)[x][y].id, map);

				(*graph)[x][y].owner = 8;
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

void drawMap(Pixel **graph, SDL_Window *window, SDL_Renderer* renderer, Centre *cellsList, int nbNodes){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Couleur du background
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

void displayDices(SMap *map, Centre *cellsList, SDL_Window *window, int nbNodes)
{
    for (int i=0; i<nbNodes; i++) {
        if (map->cells[i].nbDices == 1) {
            insertPicture("C:/Users/USER/Desktop/Home/dicewars/sprites/1.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 2) {
            insertPicture("C:/Users/USER/Desktop/Home/dicewars/sprites/2.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 3) {
            insertPicture("C:/Users/USER/Desktop/Home/dicewars/sprites/3.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 4) {
            insertPicture("C:/Users/USER/Desktop/Home/dicewars/sprites/4.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 5) {
            insertPicture("C:/Users/USER/Desktop/Home/dicewars/sprites/5.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 6) {
            insertPicture("C:/Users/USER/Desktop/Home/dicewars/sprites/6.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 7) {
            insertPicture("C:/Users/USER/Desktop/Home/dicewars/sprites/7.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        } else if (map->cells[i].nbDices == 8) {
            insertPicture("C:/Users/USER/Desktop/Home/dicewars/sprites/8.bmp", window, cellsList[i].x, cellsList[i].y, 15, 15);
        }
    }
}