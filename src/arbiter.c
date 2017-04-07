#include <malloc.h>
#include <stdio.h>
#include "arbiter.h"
#include "output.h"
#include "interface.h"


//isFirst vaut 1 si c'est le premier appel de gameLoop, 0 sinon
//isLast vaut 1 si c'est le dernier appel de gameLoop, 0 sinon
void gameLoop(MapContext *mapContext, SPlayer *players, SInterface *interfaces, int nbPlayer, int isFirst, int isLast) {

    // Initialisation des IA
    for (int i=0; i<nbPlayer; i++) {
        if (players[i].interface != -1){
            SPlayerInfo info;
            interfaces[players[i].interface].InitGame(i, nbPlayer, &info);
            players[i].playerInfo = info;
        }
    }

    // Cas du premier appel de gameLoop
    if (isFirst) {
        outputOpen();

        //Logs
        for(int i=0; i<nbPlayer; i++){
            if(players[i].interface>=0){ // joueur avec IA
                outputPlayer(players[i].id, players[i].playerInfo);
            }
        }
    }

    outputInit(mapContext->map); //Logs

    //Tableau répertoriant le nombre de cellules de chaque joueurs
    //Permet de connaitre les joueurs éliminés
    int nbPlayersCells[nbPlayer];
	getNbPlayersCells(nbPlayersCells, nbPlayer, mapContext->map->cells, mapContext->map->nbCells);

    int playerTurn = 0;
    int end = 0;


    //Boucle de jeu
    STurn *turn = malloc(sizeof(STurn));

    while (!end) {

        if(nbPlayersCells[playerTurn] == 0){
            // Cas d'un joueur éliminé
            playerTurn = (playerTurn + 1) % nbPlayer;
        } else {

            if (players[playerTurn].interface == -1) {
                //Récupération du choix du joueur
                //Tour d'un joueur humain
                int click;
                printf("C'est au joueur %d de jouer : ", playerTurn + 1);
                printColourOfPlayer(playerTurn);
                click = getIdOnClick(mapContext->nbNodes, mapContext->cellsList);

                if (click == -2) {
                    end = 1;
                } else if (click == -1) {
                    //passage au joueur suivant
                    printf("On change de joueur\n");
                    giveReinforcements(mapContext, nbPlayer, playerTurn); // On donne les renforts
                    playerTurn = (playerTurn + 1) % nbPlayer;
                } else {

                    //on vérifie que le joueur a cliqué sur la bonne case
                    if (playerTurn != mapContext->map->cells[click].owner) {
                        printf("Ce n'est pas a ce joueur de jouer\n");
                    } else {
                        int cellFrom = click;
                        int cellTo;
                        cellTo = getIdOnClick(mapContext->nbNodes, mapContext->cellsList);
                        if (cellTo == -2) {
                            end = 1;
                        } else if (cellTo == -1) {
                            //passage au joueur suivant
                            printf("On change de joueur\n");
                            giveReinforcements(mapContext, nbPlayer, playerTurn); // On donne les renforts
                            playerTurn = (playerTurn + 1) % nbPlayer;
                        } else {
                            turn->cellFrom = (unsigned int) cellFrom;
                            turn->cellTo = (unsigned int) cellTo;
                            runTurn(turn, mapContext);
                        }
                    }
                }
                // On redessine la map
                drawMap(mapContext->cellsList, mapContext->nbNodes, &mapContext->graph);
            }

            //Tour d'une IA
            else {
                SMap *mapCopy;

                //tant que l'IA veut rejouer
                int continueTurn;
                do {

                    mapCopy = copyMap(mapContext, nbPlayer);
                    continueTurn = interfaces[players[playerTurn].interface].PlayTurn(playerTurn, mapContext->map, turn);

                    updateMapContext(mapCopy, mapContext);
                    if (continueTurn){
                        runTurn(turn, mapContext);
                        drawMap(mapContext->cellsList, mapContext->nbNodes, &mapContext->graph);
                    }

                }
                while (continueTurn);

                //Quand l'ia termine son tour ou coup incorrect
                giveReinforcements(mapContext, nbPlayer, playerTurn); // On donne les renforts

                drawMap(mapContext->cellsList, mapContext->nbNodes, &mapContext->graph);
                playerTurn = (playerTurn + 1) % nbPlayer;
            }
        }

        getNbPlayersCells(nbPlayersCells, nbPlayer, mapContext->map->cells, mapContext->map->nbCells);
        drawMap(mapContext->cellsList, mapContext->nbNodes, &mapContext->graph);
        if (gameIsOver(nbPlayersCells, nbPlayer)) end = 1;

    }


    //Pour déterminer le gagnant
    int idGagnant = -1;
    int i = 0;
    while(idGagnant == -1){
        if(nbPlayersCells[i] > 0) idGagnant = i;
        i++;
    }

    outputEndGame(players[idGagnant].id); //log

    // Cas du dernier appel de gameLoop
    if (isLast) outputClose();

    //Libération des ressources
    free(turn);
    destroyMap(mapContext);

}

void runTurn(STurn *turn, MapContext *mapContext) {
    int idWinner;
    int idLoser;

    //si le coup est autorisé
    if (checkMove(turn, mapContext)) {

        //on lance les dés pour savoir qui est le gagnant
        idWinner = whoWins(mapContext->map->cells[turn->cellFrom], mapContext->map->cells[turn->cellTo]);
        int attackWin = 1;

        if (idWinner == turn->cellFrom) {
            idLoser = turn->cellTo;
        } else {
            idLoser = turn->cellFrom;
            attackWin = 0;
        }

        //on actualise le nombre de dés sur chaque cellule
        updateDices(&(mapContext->map->cells[idWinner]), &(mapContext->map->cells[idLoser]), attackWin);

        outputTurn(turn->cellFrom, turn->cellTo, attackWin);//logs
    }
}

//renvoie l'id de la cellule gagnant le lancé de dé
int whoWins(SCell cellFrom, SCell cellTo){
    int thisDiceValueFrom = 0;
    int dicesValueFrom = 0;
    for (int i=0; i<cellFrom.nbDices; i++) {
        thisDiceValueFrom = randomBounds(1,6); // AJOUT
        //      drawScore(0, thisDiceValueFrom, i); // AJOUT
        dicesValueFrom += thisDiceValueFrom; // AJOUT
    }

    int thisDiceValueTo = 0;
    int dicesValueTo = 0;
    for (int i=0; i<cellTo.nbDices; i++) {
        thisDiceValueTo = randomBounds(1,6); // AJOUT
        //      drawScore(1, thisDiceValueTo, i); // AJOUT
        dicesValueTo += thisDiceValueTo; // AJOUT
    }

    if(dicesValueFrom > dicesValueTo) {
        return cellFrom.id;
    }else{
        return cellTo.id;
    }
}

// Met à jour le nombre de dés après une attaque
// cellWinner : la cellule gagnante du jet de dés
// cellLoser : la sellule perdante du jet de dés
// attackWin : 1 si c'est la cellule attaquante qui a gagné, 0 si c'est la cellule défensive qui a gagné
void updateDices(SCell *cellWinner, SCell *cellLoser, int attackWin){
    if (attackWin) {
        cellLoser->owner = cellWinner->owner;
        cellLoser->nbDices = cellWinner->nbDices - 1;
        cellWinner->nbDices = 1;
    } else {
        cellLoser->nbDices = 1;
    }
}

//renvoie 1 si le coup est autorisé, 0 sinon
int checkMove(STurn *turn, MapContext *mapContext){
    int authorized = 1;
    if(mapContext->map->cells[turn->cellFrom].nbDices <= 1){
        printf("Erreur : une cellule ne peut pas attaquer si elle n'a qu'un de\n");
        authorized = 0;
    }else if (!isNeighbor(&(mapContext->map->cells[turn->cellFrom]), &(mapContext->map->cells[turn->cellTo]))) {
        printf("Erreur : une cellule ne peut pas attaquer une cellule qui n'est pas voisine\n");
        authorized =  0;
    } else if (mapContext->map->cells[turn->cellFrom].owner == mapContext->map->cells[turn->cellTo].owner){
        authorized = 0;
        printf("Erreur : on ne peut pas s'attaquer soi-meme\n");

    }
    return authorized;
}

SMap* copyMap(MapContext *mapContextToCopy, int nbPlayer){
    // Initialisation de la copie de la map
    SMap *newMap = malloc(sizeof(SMap));

    newMap->nbCells = mapContextToCopy->map->nbCells;
    newMap->cells = malloc(sizeof(SCell) * newMap->nbCells);

    newMap->stack = calloc(nbPlayer, sizeof(unsigned int));


    for (int i = 0; i < newMap->nbCells; i++) {
        newMap->cells[i].id = mapContextToCopy->map->cells[i].id;
        newMap->cells[i].owner = mapContextToCopy->map->cells[i].owner;
        newMap->cells[i].nbDices = mapContextToCopy->map->cells[i].nbDices;
        newMap->cells[i].nbNeighbors = mapContextToCopy->map->cells[i].nbNeighbors;
        newMap->cells[i].neighbors = malloc(newMap->cells[i].nbNeighbors * sizeof(SCell *));
    }

    for (int i = 0; i < newMap->nbCells; i++) {
        for (int j = 0; j < mapContextToCopy->map->cells[i].nbNeighbors; j++) {
            newMap->cells[i].neighbors[j] =  &(newMap->cells[mapContextToCopy->map->cells[i].neighbors[j]->id]);
        }
    }

    for (int i = 0 ; i < nbPlayer ; i ++){
        newMap->stack[i] = mapContextToCopy->map->stack[i];
    }

    return newMap;
}

void updateMapContext(SMap *mapCopy, MapContext *mapContextToUpdate){

    Centre *cellsList = malloc(sizeof(Centre)*(mapCopy->nbCells));
    for (int i = 0 ; i < mapCopy->nbCells ; i++){
        cellsList[i].x = mapContextToUpdate->cellsList[i].x;
        cellsList[i].y = mapContextToUpdate->cellsList[i].y ;
        cellsList[i].cell = &(mapCopy->cells[i]);
    }

    updateGraph(cellsList, mapContextToUpdate);

    destroyMap(mapContextToUpdate);
    mapContextToUpdate->map = mapCopy;

    mapContextToUpdate->cellsList = cellsList;
}

void destroyMap(MapContext *mapContextToDestroy) {
    // On détruit la map
    for (int i = 0; i < mapContextToDestroy->map->nbCells; i++) {
        free(mapContextToDestroy->map->cells[i].neighbors);
    }

    free(mapContextToDestroy->map->cells);
    free(mapContextToDestroy->map->stack);

    free(mapContextToDestroy->map);

	// On détruit cellsList
	free(mapContextToDestroy->cellsList);
}

//Met à jour la liste du nombre des cellules de chaque joueur
void getNbPlayersCells(int *nbPlayersCells, int nbPlayer, SCell *cells, int nbCells){
    for(int i=0; i<nbPlayer; i++){
        nbPlayersCells[i]=0;
    }
    for(int i=0; i<nbCells; i++){
        int owner = cells[i].owner;
        nbPlayersCells[owner]++;
    }
}

//Regarde s'il ne reste plus qu'un joueur possédant des cellules
int gameIsOver(int *nbPlayersCells, int nbPlayer){
    int nbPlayersWIthCells = 0;
    for (int i=0; i<nbPlayer; i++) {
        if(nbPlayersCells[i] > 0) nbPlayersWIthCells++;
    }
    return (nbPlayersWIthCells == 1);
}
