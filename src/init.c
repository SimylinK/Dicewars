#include "init.h"
#include "util.h"

MapContext* initMap(MapContext *mapContext, unsigned int nbPlayer){
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