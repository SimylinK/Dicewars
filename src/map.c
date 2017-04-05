#include <stdio.h>
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
			WWIDTH,                               // width, in pixels
            WHEIGHT,                               // height, in pixels
			SDL_WINDOW_SHOWN                  // flags - see below
	);

	renderer = SDL_CreateRenderer(window, -1, 0);

	// Boucle d'affichage principale
	drawMap(mapContext->cellsList, mapContext->nbNodes);

	//Destruction des ressources
}


void destroyMap(SMap *mapToDestroy) {
    free(mapToDestroy->cells->neighbors);
    free(mapToDestroy->cells);
    free(mapToDestroy->stack);

    free(mapToDestroy);
}



void destroyWindow(SDL_Window *window, SDL_Renderer* renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	//TODO : des free ?
}


// Colore les pixels de bordure en noir
void drawBorders(SDL_Renderer* renderer, Centre *cellsList, unsigned int nbNodes){

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // noir

	Centre closer;
	Centre rightNeighborCloser;
	Centre downNeighborCloser;

	for (unsigned int x = BORDERLANDR; x < WIDTH - 1; x++) {
		for (unsigned int y = BORDERTOP; y < HEIGHT - 1; y++) {
			// Quand on trouve un changement d'id
			closer = getCloser(cellsList, nbNodes, x, y);
			rightNeighborCloser = getCloser(cellsList, nbNodes, x+1, y);
			downNeighborCloser = getCloser(cellsList, nbNodes, x, y+1);
			if (closer.cell->id != rightNeighborCloser.cell->id)
			{
				SDL_RenderDrawPoint(renderer, x, y);	// On dessine en noir

			} else if  (closer.cell->id != downNeighborCloser.cell->id) {

				SDL_RenderDrawPoint(renderer, x, y); // On dessine en noir
			}
		}
	}
}


// Utiliser SDL_RenderPresent après cette fonction
void drawPixels(SDL_Renderer* renderer, Centre *cellsList, unsigned int nbNodes){
	SDL_SetRenderDrawColor(renderer, 205, 181, 205, 255); // Couleur du background
    SDL_RenderClear(renderer);
	Centre closer; // Le centre le plus près
	for (unsigned int x=BORDERLANDR; x<WIDTH; x++){
		for (unsigned int y=BORDERTOP; y<HEIGHT; y++){
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
				default:
					printf("Cellule sans owner\n");
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

	surface = SDL_LoadBMP(name);

	texture = SDL_CreateTextureFromSurface(renderer,surface); // Préparation du sprite
	surface->w = width; surface->h = height;//on adapte la taille de la surface aux dimensions demandées en paramètres


	SDL_Rect dest = {x - surface->w/2, y - surface->h/2, surface->w, surface->h};
	SDL_RenderCopy(renderer,texture,NULL,&dest); // Copie du sprite grâce au SDL_Renderer

    SDL_DestroyTexture(texture); // Libération de la mémoire associée à la texture
	SDL_FreeSurface(surface); // Libération de la ressource occupée par le sprite
}

// Utiliser SDL_RenderPresent après cette fonction
void displayDices(SDL_Window *window, Centre *cellsList, int nbNodes)
{
    for (int i=0; i<nbNodes; i++) {
        switch (cellsList[i].cell->nbDices) {
            case 1:
            insertPicture("../sprites/1.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 2:
            insertPicture("../sprites/2.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 3:
            insertPicture("../sprites/3.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 4:
            insertPicture("../sprites/4.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 5:
            insertPicture("../sprites/5.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 6:
            insertPicture("../sprites/6.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 7:
              insertPicture("../sprites/7.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 8:
                insertPicture("../sprites/8.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
			default:
				printf("Cellule sans de\n");
				break;
        }
    }
}


void drawScore(int joueur, int diceValue, int i){
	
	int positionY;
	int positionX = BUTTONX+60+20*i;
	int largeurDes = DICESWIDTH*2;
	int hauteurDes = DICESHEIGHT*2;

	if (joueur == 0){
        positionY = BUTTONY-20;
    } else {
        positionY = BUTTONY+20;
    }

	switch (diceValue) {

        case 1:
            insertPicture("../sprites/1.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;

        case 2:
            insertPicture("../sprites/2.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;

        case 3:
            insertPicture("../sprites/3.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;

        case 4:
            insertPicture("../sprites/4.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;

        case 5:
            insertPicture("../sprites/5.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;

        case 6:
            insertPicture("../sprites/6.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;
    }

	SDL_RenderPresent(renderer);
}


void drawMap(Centre *cellsList, unsigned int nbNodes){

	// On dessine les pixels
	drawPixels(renderer, cellsList, nbNodes);
	// On dessine les bordures
	drawBorders(renderer, cellsList, nbNodes);
	// On affiche les dés
	displayDices(window, cellsList, nbNodes);
    //On ajoute le bouton "tour suivant"
    insertPicture("../sprites/end_turn.bmp", window, BUTTONX, BUTTONY, BUTTONW, BUTTONH);

	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

}
