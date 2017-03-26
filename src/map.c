#include "map.h"
#include "util.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
// Fichier chargé de déterminer les paramètres aléatoires et de créer la map
void mainMap(MapContext *mapContext) {

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(
			"Dicewars",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			WIDTH,                               // width, in pixels
			HEIGHT,                               // height, in pixels
			SDL_WINDOW_SHOWN                  // flags - see below
	);

	renderer = SDL_CreateRenderer(window, -1, 0);

	// Boucle d'affichage principale
	drawMap(mapContext->map, mapContext->cellsList, mapContext->nbNodes);

	//Destruction des ressources
}



void destroyMap(SDL_Window *window, SDL_Renderer* renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	//TODO : des free ?
}


// Colore les pixels de bordure en noir
void drawBorders(SMap *map, SDL_Renderer* renderer, Centre *cellsList, int nbNodes){

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
			if (closer.cell->id != rightNeighborCloser.cell->id)
			{
				// On dessine en noir
				SDL_RenderDrawPoint(renderer, x, y);

			} else if  (closer.cell->id != downNeighborCloser.cell->id) {
				// On dessine en noir
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}
}



void drawPixels(SDL_Renderer* renderer, Centre *cellsList, int nbNodes){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Couleur du background
	Centre closer; // Le centre le plus près
	for (unsigned int x=0; x<WIDTH; x++){
		for (unsigned int y=0; y<HEIGHT; y++){
			closer=getCloser(cellsList, nbNodes, x, y);
			switch (closer.cell->owner){ // On définit les couleurs des joueurs
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

// Utiliser SDL_RenderPresent après cette fonction
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

void drawMap(SMap *map, Centre *cellsList, int nbNodes){

	// On dessine les pixels
	drawPixels(renderer, cellsList, nbNodes);
	// On dessine les bordures
	drawBorders(map, renderer, cellsList, nbNodes);
	// On affiche les dés
	displayDices(map, window, cellsList, nbNodes);

	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

}
