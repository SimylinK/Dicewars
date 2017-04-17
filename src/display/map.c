#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "map.h"
#include "../utils/util.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Window *menuWindow = NULL;
SDL_Renderer *menuRenderer = NULL;

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

    // Affichage de la map une première fois
	drawMap(mapContext->cellsList, mapContext->nbNodes, &mapContext->graph);

}

void drawDiceOfPlayer(int id){
    switch (id){ // On définit les couleurs des joueurs
        case 0:
            insertPicture("sprites/dicesOfPlayer/yellowdice.bmp", window, DICESCOLOURX, DICESCOLOURY, DICESCOLOURWIDTH, DICESCOLOURHEIGHT);
            break;
        case 1:
            insertPicture("sprites/dicesOfPlayer/whitedice.bmp", window, DICESCOLOURX, DICESCOLOURY, DICESCOLOURWIDTH, DICESCOLOURHEIGHT);
            break;
        case 2:
            insertPicture("sprites/dicesOfPlayer/cyandice.bmp", window, DICESCOLOURX, DICESCOLOURY, DICESCOLOURWIDTH, DICESCOLOURHEIGHT);
            break;
        case 3:
            insertPicture("sprites/dicesOfPlayer/reddice.bmp", window, DICESCOLOURX, DICESCOLOURY, DICESCOLOURWIDTH, DICESCOLOURHEIGHT);
            break;
        case 4:
            insertPicture("sprites/dicesOfPlayer/bluedice.bmp", window, DICESCOLOURX, DICESCOLOURY, DICESCOLOURWIDTH, DICESCOLOURHEIGHT);
            break;
        case 5:
            insertPicture("sprites/dicesOfPlayer/pinkdice.bmp", window, DICESCOLOURX, DICESCOLOURY, DICESCOLOURWIDTH, DICESCOLOURHEIGHT);
            break;
        case 6:
            insertPicture("sprites/dicesOfPlayer/orangedice.bmp", window, DICESCOLOURX, DICESCOLOURY, DICESCOLOURWIDTH, DICESCOLOURHEIGHT);
            break;
        case 7:
            insertPicture("sprites/dicesOfPlayer/greendice.bmp", window, DICESCOLOURX, DICESCOLOURY, DICESCOLOURWIDTH, DICESCOLOURHEIGHT);
            break;
        default:
            printf("Pas de couleur\n");
            break;
    }
}

void destroyWindow() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

void drawBorders(SDL_Renderer* renderer, Graph *graph){

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // noir

    for (unsigned int x = BORDERLANDR; x < WIDTH - 1; x++) {
        for (unsigned int y = BORDERTOP; y < HEIGHT- 1; y++) {
            // Quand on trouve un changement d'id
            if ((*graph)[x][y]->id !=(*graph)[x+1][y]->id)
            {

                SDL_RenderDrawPoint(renderer, x, y);	// On dessine en noir

            } else if ((*graph)[x][y]->id !=(*graph)[x][y+1]->id) {

                SDL_RenderDrawPoint(renderer, x, y); // On dessine en noir
            }
        }
    }
}

void drawPixels(SDL_Renderer* renderer, Graph *graph){
    SDL_SetRenderDrawColor(renderer, 205, 181, 205, 255); // Couleur du background
    SDL_RenderClear(renderer);
    for (unsigned int x=BORDERLANDR; x<WIDTH; x++){
        for (unsigned int y=BORDERTOP; y<HEIGHT; y++){
            switch ((*graph)[x][y]->owner){ // On définit les couleurs des joueurs
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
                    break;
            }
            SDL_RenderDrawPoint(renderer, x, y);
            if(x == BORDERLANDR || y == BORDERTOP || x == WIDTH-1 || y == HEIGHT-1){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderDrawPoint(renderer, x-1, y-1);SDL_RenderDrawPoint(renderer, x, y-1);SDL_RenderDrawPoint(renderer, x+1, y-1);
                SDL_RenderDrawPoint(renderer, x-1, y);SDL_RenderDrawPoint(renderer, x, y);SDL_RenderDrawPoint(renderer, x+1, y);
                SDL_RenderDrawPoint(renderer, x-1, y+1);SDL_RenderDrawPoint(renderer, x, y+1);SDL_RenderDrawPoint(renderer, x+1, y+1);
            }

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
	surface->w = width; surface->h = height; //on adapte la taille de la surface aux dimensions demandées en paramètres


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
            insertPicture("sprites/1.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 2:
            insertPicture("sprites/2.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 3:
            insertPicture("sprites/3.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 4:
            insertPicture("sprites/4.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 5:
            insertPicture("sprites/5.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 6:
            insertPicture("sprites/6.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 7:
              insertPicture("sprites/7.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
            case 8:
                insertPicture("sprites/8.bmp", window, cellsList[i].x, cellsList[i].y, DICESWIDTH, DICESHEIGHT);
                break;
			default:
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
            insertPicture("sprites/1.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;

        case 2:
            insertPicture("sprites/2.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;

        case 3:
            insertPicture("sprites/3.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;

        case 4:
            insertPicture("sprites/4.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;

        case 5:
            insertPicture("sprites/5.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;

        case 6:
            insertPicture("sprites/6.bmp", window, positionX, positionY, largeurDes, hauteurDes);
            break;
    }

	SDL_RenderPresent(renderer);
}

void drawMap(Centre *cellsList, unsigned int nbNodes, Graph *graph) {

    // On dessine les pixels
    drawPixels(renderer, graph);
    // On dessine les bordures
    drawBorders(renderer, graph);
    // On affiche les dés
    displayDices(window, cellsList, nbNodes);
    //On ajoute le bouton "tour suivant"
    insertPicture("sprites/end_turn.bmp", window, BUTTONX, BUTTONY, BUTTONW, BUTTONH);
    //On affiche le titre
    insertPicture("sprites/dicewarslogo.bmp", window, TITLEX, TITLEY, TITLEW, TITLEH);
}

void drawMapTurn(Centre *cellsList, unsigned int nbNodes, Graph *graph, int idPlayer, SMap *map){

    // On dessine les pixels
    drawPixels(renderer, graph);
    // On dessine les bordures
    drawBorders(renderer, graph);
    // On affiche les dés
    displayDices(window, cellsList, nbNodes);
    //On ajoute le bouton "tour suivant"
    insertPicture("sprites/end_turn.bmp", window, BUTTONX, BUTTONY, BUTTONW, BUTTONH);
    //On affiche le titre
    insertPicture("sprites/dicewarslogo.bmp", window, TITLEX, TITLEY, TITLEW, TITLEH);
    //On affiche le dé coloré du joueur à qui c'est le tour
    drawDiceOfPlayer(idPlayer);
    //stack
    drawStackOfPlayer(map, idPlayer);


    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

}

void drawStackOfPlayer(SMap *map, int id){
    int digit = 0;
    int taille = 1;
    if (map->stack[id]>9){
        taille = 2;
    }
    for (int i = 0 ; i < taille ; i++){
        if (i == 0){
            digit = map->stack[id]/10;
        } else digit = map->stack[id]%10;

        switch (digit){ // On définit les couleurs des joueurs
            case 0:
                insertPicture("sprites/zero.bmp", window, DIGITX + i*(DIGITWIDTH), DIGITY, DIGITWIDTH, DIGITHEIGHT);
                break;
            case 1:
                insertPicture("sprites/un.bmp", window, DIGITX + i*(DIGITWIDTH), DIGITY, DIGITWIDTH, DIGITHEIGHT);
                break;
            case 2:
                insertPicture("sprites/deux.bmp", window, DIGITX + i*(DIGITWIDTH), DIGITY, DIGITWIDTH, DIGITHEIGHT);
                break;
            case 3:
                insertPicture("sprites/trois.bmp", window, DIGITX + i*(DIGITWIDTH), DIGITY, DIGITWIDTH, DIGITHEIGHT);
                break;
            case 4:
                insertPicture("sprites/quatre.bmp", window, DIGITX + i*(DIGITWIDTH), DIGITY, DIGITWIDTH, DIGITHEIGHT);
                break;
            case 5:
                insertPicture("sprites/cinq.bmp", window, DIGITX + i*(DIGITWIDTH), DIGITY, DIGITWIDTH, DIGITHEIGHT);
                break;
            case 6:
                insertPicture("sprites/six.bmp", window, DIGITX + i*(DIGITWIDTH), DIGITY, DIGITWIDTH, DIGITHEIGHT);
                break;
            case 7:
                insertPicture("sprites/sept.bmp", window, DIGITX + i*(DIGITWIDTH), DIGITY, DIGITWIDTH, DIGITHEIGHT);
                break;
            case 8:
                insertPicture("sprites/huit.bmp", window, DIGITX + i*(DIGITWIDTH), DIGITY, DIGITWIDTH, DIGITHEIGHT);
                break;
            case 9:
                insertPicture("sprites/neuf.bmp", window, DIGITX + i*(DIGITWIDTH), DIGITY, DIGITWIDTH, DIGITHEIGHT);
                break;
            default:
                printf("Pas de couleur\n");
                break;
        }
    }

}

void drawPixelsOnClick(SDL_Renderer* renderer, Graph *graph, int idCell){
    SDL_SetRenderDrawColor(renderer, 205, 181, 205, 255); // Couleur du background
    SDL_RenderClear(renderer);
    for (unsigned int x=BORDERLANDR; x<WIDTH; x++) {
        for (unsigned int y = BORDERTOP; y < HEIGHT; y++) {
            switch ((*graph)[x][y]->owner) { // On définit les couleurs des joueurs
                case 0:
                    if ((*graph)[x][y]->id == idCell) {
                        SDL_SetRenderDrawColor(renderer, 100, 100, 0, 255); // jaune foncé
                    } else SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // jaune
                    break;
                case 1:
                    if ((*graph)[x][y]->id == idCell) {
                        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); // gris foncé
                    } else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // blanc
                    break;
                case 2:
                    if ((*graph)[x][y]->id == idCell) {
                        SDL_SetRenderDrawColor(renderer, 11, 70, 56, 255); // cyan foncé
                    } else SDL_SetRenderDrawColor(renderer, 20, 134, 107, 255); // cyan
                    break;
                case 3:
                    if ((*graph)[x][y]->id == idCell) {
                        SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255); // rouge foncé
                    } else SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255); // rouge
                    break;
                case 4:
                    if ((*graph)[x][y]->id == idCell) {
                        SDL_SetRenderDrawColor(renderer, 0, 22, 43, 255); // bleu foncé
                    } else SDL_SetRenderDrawColor(renderer, 0, 66, 100, 255); // bleu
                    break;
                case 5:
                    if ((*graph)[x][y]->id == idCell) {
                        SDL_SetRenderDrawColor(renderer, 151, 23, 103, 255); // rose foncé
                    } else SDL_SetRenderDrawColor(renderer, 229, 91, 176, 255); // rose
                    break;
                case 6:
                    if ((*graph)[x][y]->id == idCell) {
                        SDL_SetRenderDrawColor(renderer, 125, 29, 0, 255); // orange foncé
                    } else SDL_SetRenderDrawColor(renderer, 255, 60, 4, 255); // orange
                    break;
                case 7:
                    if ((*graph)[x][y]->id == idCell) {
                        SDL_SetRenderDrawColor(renderer, 12, 64, 0, 255); // vert foncé
                    } else SDL_SetRenderDrawColor(renderer, 22, 128, 0, 255); // vert
                    break;
                default:
                    printf("Cellule sans owner\n");
            }
            SDL_RenderDrawPoint(renderer, x, y);
            if(x == BORDERLANDR || y == BORDERTOP || x == WIDTH-1 || y == HEIGHT-1){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderDrawPoint(renderer, x-1, y-1);SDL_RenderDrawPoint(renderer, x, y-1);SDL_RenderDrawPoint(renderer, x+1, y-1);
                SDL_RenderDrawPoint(renderer, x-1, y);SDL_RenderDrawPoint(renderer, x, y);SDL_RenderDrawPoint(renderer, x+1, y);
                SDL_RenderDrawPoint(renderer, x-1, y+1);SDL_RenderDrawPoint(renderer, x, y+1);SDL_RenderDrawPoint(renderer, x+1, y+1);
            }
        }
    }
}

void drawMapTurnOnClick(Centre *cellsList, unsigned int nbNodes, Graph *graph, int idPlayer, SMap *map, int idCell){

    // On dessine les pixels
    drawPixelsOnClick(renderer, graph, idCell);
    // On dessine les bordures
    drawBorders(renderer, graph);
    // On affiche les dés
    displayDices(window, cellsList, nbNodes);
    //On ajoute le bouton "tour suivant"
    insertPicture("sprites/end_turn.bmp", window, BUTTONX, BUTTONY, BUTTONW, BUTTONH);
    //On affiche le titre
    insertPicture("sprites/dicewarslogo.bmp", window, TITLEX, TITLEY, TITLEW, TITLEH);
    //On affiche le dé coloré du joueur à qui c'est le tour
    drawDiceOfPlayer(idPlayer);
    //stack
    drawStackOfPlayer(map, idPlayer);

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

int displayWinner(int idPlayer){

    SDL_Window *finalWindow = NULL;
    SDL_Renderer *finalRenderer = NULL;

    finalWindow = SDL_CreateWindow(
            "Who is the winner ?",                  // window title
            SDL_WINDOWPOS_UNDEFINED,           // initial x position
            SDL_WINDOWPOS_UNDEFINED,           // initial y position
            WWIDTH,                               // width, in pixels
            WHEIGHT,                               // height, in pixels
            SDL_WINDOW_SHOWN                  // flags
    );

    finalRenderer = SDL_CreateRenderer(finalWindow, -1, 0);




    return 0;
}

void menuInit(unsigned int *nbPlayer, unsigned int *nbIA){
    SDL_Init(SDL_INIT_VIDEO);

    menuWindow = SDL_CreateWindow(
            "Let's init the game",                  // window title
            SDL_WINDOWPOS_UNDEFINED,           // initial x position
            SDL_WINDOWPOS_UNDEFINED,           // initial y position
            WWIDTH,                               // width, in pixels
            WHEIGHT,                               // height, in pixels
            SDL_WINDOW_SHOWN                  // flags
    );

    menuRenderer = SDL_CreateRenderer(menuWindow, -1, 0);
    SDL_SetRenderDrawColor(menuRenderer, 205, 181, 205, 255); // Couleur du background
    SDL_RenderClear(menuRenderer);

    drawMenu(-5, -5);

    displayNumberOfPlayer(5);
    displayNumberOfIA(5);

    SDL_RenderPresent(menuRenderer);
    *nbPlayer = 2;
    *nbIA = 0;
    howManyPlayers(nbPlayer, nbIA);
    printf("Nb de joueurs : %d \n", *nbPlayer);

    SDL_DestroyRenderer(menuRenderer);
    SDL_DestroyWindow(menuWindow);
}

void howManyPlayers(unsigned int *nbPlayer, unsigned int *nbIA){
    SDL_Event event;
    int end = 0;

    while (!end){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_MOUSEBUTTONDOWN:
                if (SDL_BUTTON_LEFT){
                    //si le clic se fait sur le bouton "+" d'humains
                    if (event.button.y>NB_PLAYER_PLUS_Y - NB_PLAYER_PLUS_HEIGHT/2
                        && event.button.y<NB_PLAYER_PLUS_Y + NB_PLAYER_PLUS_HEIGHT/2
                        && event.button.x>NB_PLAYER_PLUS_X - NB_PLAYER_PLUS_WIDTH/2
                        && event.button.x<NB_PLAYER_PLUS_X + NB_PLAYER_PLUS_WIDTH/2){
                        printf("PLUS \n");
                        if(*nbIA + *nbPlayer < 8){
                            (*nbPlayer) ++;
                            printf("nbOfplayer : %d  \n", *nbPlayer);
                        }

                    //si le clic se fait sur le bouton "-" d'humains
                    } else if (event.button.y>NB_PLAYER_MOINS_Y - NB_PLAYER_MOINS_HEIGHT/2
                        && event.button.y<NB_PLAYER_MOINS_Y + NB_PLAYER_MOINS_HEIGHT/2
                        && event.button.x>NB_PLAYER_MOINS_X - NB_PLAYER_MOINS_WIDTH/2
                        && event.button.x<NB_PLAYER_MOINS_X + NB_PLAYER_MOINS_WIDTH/2){

                        printf("MOINS\n");
                        if(*nbIA + *nbPlayer > 2) {
                            (*nbPlayer)--;
                            printf("nbOfplayer : %d  \n", *nbPlayer);
                        }

                    //si le clic se fait sur le bouton "-" d'IA
                    } else if (event.button.y>NB_IA_MOINS_Y - NB_IA_MOINS_HEIGHT/2
                               && event.button.y<NB_IA_MOINS_Y + NB_IA_MOINS_HEIGHT/2
                               && event.button.x>NB_IA_MOINS_X - NB_IA_MOINS_WIDTH/2
                               && event.button.x<NB_IA_MOINS_X + NB_IA_MOINS_WIDTH/2){

                        printf("MOINS\n");
                        if(*nbIA + *nbPlayer > 2) {
                            (*nbIA)--;
                            printf("nbOfIA : %d  \n", *nbIA);
                        }

                        //si le clic se fait sur le bouton "+" d'IA
                    }else if (event.button.y>NB_IA_PLUS_Y - NB_IA_PLUS_HEIGHT/2
                              && event.button.y<NB_IA_PLUS_Y + NB_IA_PLUS_HEIGHT/2
                              && event.button.x>NB_IA_PLUS_X - NB_IA_PLUS_WIDTH/2
                              && event.button.x<NB_IA_PLUS_X + NB_IA_PLUS_WIDTH/2){

                        printf("PLUS\n");
                        if(*nbIA + *nbPlayer < 8) {
                            (*nbIA)++;
                            printf("nbOfIA : %d  \n", *nbIA);
                        }

                        //si le clic se fait sur le bouton "ok"
                    } else if (event.button.y>OK_Y - OK_HEIGHT/2
                        && event.button.y<OK_Y + OK_HEIGHT/2
                        && event.button.x>OK_X - OK_WIDTH/2
                        && event.button.x<OK_X + OK_WIDTH/2){

                        printf("OK\n");
                        end = 1;
                    }
                }
                break;
            case SDL_QUIT:
                printf("SDL_QUIT\n");
                end = 1;
                break;
            default:
                end = 0;
                break;
            }
        drawMenu(*nbPlayer, *nbIA);
    }
}

void displayNumberOfPlayer(int nbOfPlayer){
    switch (nbOfPlayer){ // On définit les couleurs des joueurs
        case 0:
            insertPicture("sprites/un.bmp", menuWindow, DIGIT_NB_PLAYER_X, DIGIT_NB_PLAYER_Y, DIGIT_NB_PLAYER_WIDTH, DIGIT_NB_PLAYER_HEIGHT);
            break;
        case 1:
            insertPicture("sprites/un.bmp", menuWindow, DIGIT_NB_PLAYER_X, DIGIT_NB_PLAYER_Y, DIGIT_NB_PLAYER_WIDTH, DIGIT_NB_PLAYER_HEIGHT);
            break;
        case 2:
            insertPicture("sprites/deux.bmp", menuWindow, DIGIT_NB_PLAYER_X, DIGIT_NB_PLAYER_Y, DIGIT_NB_PLAYER_WIDTH, DIGIT_NB_PLAYER_HEIGHT);
            break;
        case 3:
            insertPicture("sprites/trois.bmp", menuWindow, DIGIT_NB_PLAYER_X, DIGIT_NB_PLAYER_Y, DIGIT_NB_PLAYER_WIDTH, DIGIT_NB_PLAYER_HEIGHT);
            break;
        case 4:
            insertPicture("sprites/quatre.bmp", menuWindow, DIGIT_NB_PLAYER_X, DIGIT_NB_PLAYER_Y, DIGIT_NB_PLAYER_WIDTH, DIGIT_NB_PLAYER_HEIGHT);
            break;
        case 5:
            insertPicture("sprites/cinq.bmp", menuWindow, DIGIT_NB_PLAYER_X, DIGIT_NB_PLAYER_Y, DIGIT_NB_PLAYER_WIDTH, DIGIT_NB_PLAYER_HEIGHT);
            break;
        case 6:
            insertPicture("sprites/six.bmp", menuWindow, DIGIT_NB_PLAYER_X, DIGIT_NB_PLAYER_Y, DIGIT_NB_PLAYER_WIDTH, DIGIT_NB_PLAYER_HEIGHT);
            break;
        case 7:
            insertPicture("sprites/sept.bmp", menuWindow, DIGIT_NB_PLAYER_X, DIGIT_NB_PLAYER_Y, DIGIT_NB_PLAYER_WIDTH, DIGIT_NB_PLAYER_HEIGHT);
            break;
        case 8:
            insertPicture("sprites/huit.bmp", menuWindow, DIGIT_NB_PLAYER_X, DIGIT_NB_PLAYER_Y, DIGIT_NB_PLAYER_WIDTH, DIGIT_NB_PLAYER_HEIGHT);
            break;
        default:
            printf("Pas de nombre\n");
            break;
    }
}

void displayNumberOfIA(int nbOfIA){
    switch (nbOfIA){ // On définit les couleurs des joueurs
        case 0:
            insertPicture("sprites/zero.bmp", menuWindow, DIGIT_NB_IA_X, DIGIT_NB_IA_Y, DIGIT_NB_IA_WIDTH, DIGIT_NB_IA_HEIGHT);
            break;
        case 1:
            insertPicture("sprites/un.bmp", menuWindow, DIGIT_NB_IA_X, DIGIT_NB_IA_Y, DIGIT_NB_IA_WIDTH, DIGIT_NB_IA_HEIGHT);
            break;
        case 2:
            insertPicture("sprites/deux.bmp", menuWindow, DIGIT_NB_IA_X, DIGIT_NB_IA_Y, DIGIT_NB_IA_WIDTH, DIGIT_NB_IA_HEIGHT);
            break;
        case 3:
            insertPicture("sprites/trois.bmp", menuWindow,DIGIT_NB_IA_X, DIGIT_NB_IA_Y, DIGIT_NB_IA_WIDTH, DIGIT_NB_IA_HEIGHT);
            break;
        case 4:
            insertPicture("sprites/quatre.bmp", menuWindow, DIGIT_NB_IA_X, DIGIT_NB_IA_Y, DIGIT_NB_IA_WIDTH, DIGIT_NB_IA_HEIGHT);
            break;
        case 5:
            insertPicture("sprites/cinq.bmp", menuWindow, DIGIT_NB_IA_X, DIGIT_NB_IA_Y, DIGIT_NB_IA_WIDTH, DIGIT_NB_IA_HEIGHT);
            break;
        case 6:
            insertPicture("sprites/six.bmp", menuWindow, DIGIT_NB_IA_X, DIGIT_NB_IA_Y, DIGIT_NB_IA_WIDTH, DIGIT_NB_IA_HEIGHT);
            break;
        case 7:
            insertPicture("sprites/sept.bmp", menuWindow, DIGIT_NB_IA_X, DIGIT_NB_IA_Y, DIGIT_NB_IA_WIDTH, DIGIT_NB_IA_HEIGHT);
            break;
        case 8:
            insertPicture("sprites/huit.bmp", menuWindow, DIGIT_NB_IA_X, DIGIT_NB_IA_Y, DIGIT_NB_IA_WIDTH, DIGIT_NB_IA_HEIGHT);
            break;
        default:
            printf("Pas de nombre\n");
            break;
    }
}

void drawMenu(int nbPlayer, int nbIA) {
    //On affiche le titre
    insertPicture("sprites/dicewarslogo.bmp", menuWindow, TITLEX, TITLEY, TITLEW, TITLEH);
    //- humains
    insertPicture("sprites/menu/moins.bmp", menuWindow, NB_PLAYER_MOINS_X, NB_PLAYER_MOINS_Y, NB_PLAYER_MOINS_WIDTH, NB_PLAYER_MOINS_HEIGHT);
    //+ humains
    insertPicture("sprites/menu/plus.bmp", menuWindow, NB_PLAYER_PLUS_X, NB_PLAYER_PLUS_Y, NB_PLAYER_PLUS_WIDTH, NB_PLAYER_PLUS_HEIGHT);
    //- IA
    insertPicture("sprites/menu/moins.bmp", menuWindow, NB_IA_MOINS_X, NB_IA_MOINS_Y, NB_IA_MOINS_WIDTH, NB_IA_MOINS_HEIGHT);
    //+ IA
    insertPicture("sprites/menu/plus.bmp", menuWindow, NB_IA_PLUS_X, NB_IA_PLUS_Y, NB_IA_PLUS_WIDTH, NB_IA_PLUS_HEIGHT);
    //ok
    insertPicture("sprites/menu/ok.bmp", menuWindow, OK_X, OK_Y, OK_WIDTH, OK_HEIGHT);
    //nbOfPlayer
    insertPicture("sprites/menu/nbOfPlayer.bmp", menuWindow, NB_PLAYER_X, NB_PLAYER_Y, NB_PLAYER_WIDTH, NB_PLAYER_HEIGHT);
    //nbOfIA
    insertPicture("sprites/menu/nbOfIA.bmp", menuWindow, NB_IA_X, NB_IA_Y, NB_IA_WIDTH, NB_IA_HEIGHT);
    //
    displayNumberOfPlayer(nbPlayer);
    //
    displayNumberOfIA(nbIA);

    SDL_RenderPresent(menuRenderer);
    SDL_RenderClear(menuRenderer);
}
