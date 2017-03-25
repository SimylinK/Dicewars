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
		// On s'assure de mettre tous les pointeurs à 0 au début
		map->cells[i].neighbors = calloc(nbNodes, sizeof(SCell *) * nbNodes);
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
	for (int i = 0; i<nbNodes; i++) {
			playerCells[map->cells[i].owner][indices[map->cells[i].owner]] = &(map->cells[i]);
			indices[map->cells[i].owner] += 1;
	}
	free(indices);

	// On remet les indices à 0
	indices = calloc(nbPlayer, sizeof(int));

	// Pour chaque joueur, on donne un dé à chacune de ses SCell
	for (int i = 0; i < nbPlayer; i++) {
		for (int j=0; j < nbCellPerPlayer[i]; j++) {
			playerCells[map->cells[i].owner][indices[map->cells[i].owner]]->nbDices = 1;
			indices[map->cells[i].owner] += 1;
		}
	}

	// On considère que le nombre de dés par SCell est environ de 3
	unsigned int dicesToGive = (nbNodes * nbDicesPerCell) / nbPlayer;

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

			if (closer.id != rightNeighborCloser.id)
			{
				// On assigne les voisins dans les deux sens
				assignNeighbor(closer.id, rightNeighborCloser.id, map);
				assignNeighbor(rightNeighborCloser.id, closer.id, map);

			} else if  (closer.id != downNeighborCloser.id) {
				// On assigne les voisins dans les deux sens
				assignNeighbor(closer.id, downNeighborCloser.id, map);
				assignNeighbor(downNeighborCloser.id, closer.id, map);

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
