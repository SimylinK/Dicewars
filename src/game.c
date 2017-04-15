#include <malloc.h>
#include <stdio.h>

#include "game.h"
#include "arbiter.h"
#include "logs/output.h"


//Fonction qui fais jouer une partie
//mapContext : la map initiale, sur laquelle la partie va être jouée
//players : tableau des joueurs qui vont jouer la partie
//interfaces : tableau contenant toutes fonctions de chaque IA
//nbPlayer : nombre de joueurs dans le tableau players
//isFirst : 1 si c'est le premier appel de gameLoop, 0 sinon
//isLast : vaut 1 si c'est le dernier appel de gameLoop, 0 sinon
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
                drawMapTurn(mapContext->cellsList, mapContext->nbNodes, &mapContext->graph, playerTurn, mapContext->map);
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
                    drawMapTurn(mapContext->cellsList, mapContext->nbNodes, &mapContext->graph, playerTurn, mapContext->map);
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

//Fonction qui joue un coup d'un joueur
//turn : le coup qui veut être joué
//mapContext : la carte sur laquelle va être joué le coup
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

// Fonction qui met à jour le nombre de dés après une attaque
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

//Fonction qui copie une map
//mapContextToCopy : la map qui va être copiée
//nbPlayer : le nombre de joueur dans la partie
//retourne une nouvelle map indépendante de la map en paramètre
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

//Fonction qui met à jour une map, à partir d'une autre map
//mapCopy : map qui va être copié, puis détruite
//mapContextToUpdate : map qui va être mise à jour
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

//Fonction qui libère les ressources d'un MapContext
//mapContextToDestroy : map ddont on veux libérer les ressources en mémoire
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
