#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include "init.h"
#include "util.h"


MapContext* initMap(MapContext *mapContext, unsigned int nbPlayer){
	// Nombre de SCell sur la map
	unsigned int nbNodes = randomBounds(30, 60);

	// Initialisation de la map
	SMap *map = malloc(sizeof(SMap));
	map->nbCells = nbNodes;
	map->cells = malloc(sizeof(SCell) * nbNodes);
	for (int i = 0; i < nbNodes; i++) {
		map->cells[i].neighbors = malloc(nbNodes * sizeof(SCell *));
	}
	map->stack = malloc(sizeof(unsigned int) * nbPlayer);

	// On assigne les SCell aux joueurs
	assignSCell(nbPlayer, nbNodes, map);

	// On génère la liste des SCell
	Centre *cellsList = generateList(nbNodes, map);

	//On assigne les voisins
	neighbors(cellsList, map, nbNodes);


	mapContext->map = map;
	mapContext->cellsList = cellsList;
	mapContext->nbNodes = nbNodes;
	return mapContext;
}

// Assigne les SCell aux joueurs
void assignSCell(unsigned int nbPlayer, unsigned int nbNodes, SMap *map) {

	// Attribue aux SCell leurs ID, l'ID du joueur qui la possède
	int idPlayer = 0;
	for (int i = 0; i < nbNodes; i++) {
		map->cells[i].id = i;
		map->cells[i].owner = idPlayer;
		map->cells[i].nbDices = 0;
		map->cells[i].nbNeighbors = 0;

		idPlayer++;
		if (idPlayer == nbPlayer) idPlayer = 0;
	}
	// On donne les dés
	giveDices(nbPlayer, nbNodes, map);
}


// Donne les dés sur chaque cellule de map
void giveDices(unsigned int nbPlayer, unsigned int nbNodes, SMap *map) {

	// Nombre de SCell minimum par joueur
	unsigned int nbCellsMin = nbNodes / nbPlayer;
	// Nombre de SCell en plus (distribuées sur les premiers joueurs)
	unsigned int nbCellsLeft = nbNodes % nbPlayer;

    // On considère que le nombre de dés par SCell est environ de 3
    unsigned int dicesToGive = (nbNodes * nbDicesPerCell) / nbPlayer;

	// Nombre de SCell exact par joueur
	unsigned int nbCellPerPlayer[nbPlayer];
	for (int i = 0; i < nbPlayer; i++) {
		if (nbCellsLeft != 0) {
			nbCellPerPlayer[i] = nbCellsMin + 1;
			nbCellsLeft--;
		} else
			nbCellPerPlayer[i] = nbCellsMin;
	}

	// Tableau des cellules des joueurs
	SCell ***playerCells = malloc(sizeof(SCell**)*nbPlayer);
	for (int i = 0; i < nbPlayer; i++) {
		// On alloue le nombre de cellules nécéssaires
		playerCells[i] = malloc(sizeof(SCell*) * nbCellPerPlayer[i]);
	}

	int *indices = calloc(nbPlayer, sizeof(int));
	// On donne à chaque joueur ses cellules
    int owner;
	for (int i = 0; i<nbNodes; i++) {
            owner = map->cells[i].owner;
			playerCells[owner][indices[owner]] = &(map->cells[i]);
			indices[owner] += 1;
	}

	// On remet les indices à 0
	for (int i=0; i<nbPlayer; i++){
        indices[i] = 0;
    }

	// Pour chaque joueur, on donne un dé à chacune de ses SCell
	for (int i = 0; i < nbPlayer; i++) {
		for (int j=0; j < nbCellPerPlayer[i]; j++) {
			playerCells[i][j]->nbDices = 1;
		}
	}

	// Le nombre restant de dés à donner par joueur
	unsigned int dicesLeft[nbPlayer];
	for (int i=0; i<nbPlayer; i++){
		dicesLeft[i] = dicesToGive - nbCellPerPlayer[i]; // On a déjà donné un dé sur chaque cellule
	}
	// On fait un random sur toutes les SCell à chaque tour de boucle
	unsigned int chosenSCell;
	for (int i = 0; i < nbPlayer; i++) {
		for (int j=0; j <dicesLeft[i]; j++) {
			chosenSCell = goodRandom(nbCellPerPlayer[i] - 1); // L'indice de la cellule, entre 0 et nbCell du joueur -1.
			playerCells[i][chosenSCell]->nbDices += 1;
		}
	}

	//Libération des ressources
	for (int i = 0; i < nbPlayer; i++) {
		free(playerCells[i]);
	}
	free(playerCells);
	free(indices);
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

			x = randomBounds(BORDERLANDR+DICESWIDTH+minDistBetweenCells, WIDTH-BORDERLANDR-minDistBetweenCells); // On force à être éloigné des bords
			y = randomBounds(BORDERTOP+DICESWIDTH+minDistBetweenCells, HEIGHT-BORDERTOP-minDistBetweenCells);

			// On vérifie que ces coordonnées n'ont pas déjà été tirées par une autre SCell
			for (int j = 0; j<i; j++){
				if (cellsList[j].x == x && cellsList[j].y == y){
					same = 1;
				}
			}
		} while (same);

		cellsList[i].x = x;
		cellsList[i].y = y;
		cellsList[i].cell = &(map->cells[i]);
	}
	return cellsList;
}

void neighbors(Centre *cellsList, SMap* map, unsigned int nbNodes){

	Centre closer;
	Centre rightNeighborCloser;
	Centre downNeighborCloser;

	for (unsigned int x = 0; x < WIDTH - 1; x++) {
		for (unsigned int y = 0; y < HEIGHT - 1; y++) {
			// Quand on trouve un changement d'id
			closer = getCloser(cellsList, nbNodes, x, y);
			rightNeighborCloser = getCloser(cellsList, nbNodes, x+1, y);
			downNeighborCloser = getCloser(cellsList, nbNodes, x, y+1);

			if (closer.cell->id != rightNeighborCloser.cell->id) {
				// On assigne les voisins dans les deux sens
				assignNeighbor(closer.cell->id, rightNeighborCloser.cell->id, map);
			}

			if  (closer.cell->id != downNeighborCloser.cell->id) {
				// On assigne les voisins dans les deux sens
				assignNeighbor(closer.cell->id, downNeighborCloser.cell->id, map);
			}
		}
	}

	// On remet les tableaux à la bonne taille
	for (int i=0; i<nbNodes; i++){
		map->cells[i].neighbors = realloc(map->cells[i].neighbors, (map->cells[i].nbNeighbors)*sizeof(SCell*));
	}
}

void assignNeighbor(int id1, int id2, SMap *map) {
	// On assigne seulement si id2 n'est pas déjà dans les voisins de id1
	if (!isNeighbor(&map->cells[id1], &map->cells[id2])){
		map->cells[id1].neighbors[map->cells[id1].nbNeighbors] = &(map->cells[id2]);
		map->cells[id1].nbNeighbors++;
	}

	// Dans l'autre sens
	if (!isNeighbor(&map->cells[id2], &map->cells[id1])){
		map->cells[id2].neighbors[map->cells[id2].nbNeighbors] = &(map->cells[id1]);
		map->cells[id2].nbNeighbors++;
	}

}

//initialise interfaces
//renvoie 0 si ça c'est bien passé, 1 sinon
int initPlayers(int nbPlayer, SPlayer *players, SInterface *interfaces, int argc, char *argv[]){

  // Le nombre d'humains
  int nbHumans = nbPlayer - argc + 3;
	printf("nbHumans : %d\n", nbHumans);

	int indexArgcLibs = 0;
	int indexInterfaces = 0;
  for (int i=0; i<nbPlayer; i++){
    //Initialisation des joueurs humains
    if (i < nbHumans){
      players[i].id = i;
      players[i].interface = -1;

    //Initialisation des joueurs IA
    } else {
      void *ia;
      pfInitGame InitGame;
      pfPlayTurn PlayTurn;
      pfEndGame EndGame;

			printf("Ouverture de : %s\n", argv[3+indexArgcLibs]);
      if ((ia=dlopen(argv[3+indexArgcLibs],RTLD_LAZY))==NULL) {
        // Erreur de chargement de la librairie
        printf("La librairie %s n'a pas pu être chargée\n", argv[3+indexArgcLibs]);
        return(1);
      }

      if ((InitGame=(pfInitGame)dlsym(ia,"InitGame"))==NULL) {
        // Erreur lors du chragement de la fonction
        printf("Une erreur s'est produite lors de la lecture de InitGame de %s\n", argv[3+indexArgcLibs]);
        return(1);
      }
      if ((PlayTurn=(pfPlayTurn)dlsym(ia,"PlayTurn"))==NULL) {
        // Erreur lors du chragement de la fonction
        printf("Une erreur s'est produite lors de la lecture de PlayTurn de %s\n", argv[3+indexArgcLibs]);
        return(1);
      }
      if ((EndGame=(pfEndGame)dlsym(ia,"EndGame"))==NULL) {
        // Erreur lors du chragement de la fonction
        printf("Une erreur s'est produite lors de la lecture de EndGame de %s\n", argv[3+indexArgcLibs]);
        return(1);
      }
      indexArgcLibs++;

      interfaces[indexInterfaces].InitGame = InitGame;
      interfaces[indexInterfaces].PlayTurn = PlayTurn;
      interfaces[indexInterfaces].EndGame = EndGame;

      //Initialisation de l'IA au passage
      SPlayerInfo info;
      interfaces[indexInterfaces].InitGame(0, nbPlayer, &info);

      //Initialisation du joueur représentant l'IA
      players[i].id = i;
      players[i].interface = indexInterfaces;
      players[i].playerInfo = info;

      indexInterfaces++;
    }
  }

  return 0;
}
