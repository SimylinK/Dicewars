#include <malloc.h>
#include <stdio.h>
#include "arbiter.h"

void gameLoop(MapContext *mapContext, SPlayer *players, SInterface *interfaces, int nbPlayer) {
    // Initialisation des IA
    for (int i=0; i<nbPlayer; i++) {
      if (players[i].interface != -1){
        SPlayerInfo info;
        interfaces[players[i].interface].InitGame(i, nbPlayer, &info);
        players[i].playerInfo = info;
      }
    }


    int playerTurn = 0;
    int end = 0;

    // La matrice d'adjacence
    int **neighborsMatrix = malloc(sizeof(int*)*mapContext->nbNodes);
    for (int i=0;i<mapContext->nbNodes;i++){
        neighborsMatrix[i] = malloc(sizeof(int)*mapContext->nbNodes);
    }
    generateMatrix(mapContext, &neighborsMatrix);

    //Boucle de jeu
    while (!end) {
        STurn *turn = malloc(sizeof(STurn));

        if (players[playerTurn].interface == -1) {
            //Récupération du choix du joueur
            //Tour d'un joueur humain

            int click;
            printf("C'est au joueur %d de jouer : ", playerTurn+1);
            printColourOfPlayer(playerTurn);
            click = getIdOnClick(mapContext->nbNodes, mapContext->cellsList);

            if (click == -2) {
                end = 1;
            } else if (click == -1) {
                //passage au joueur suivant
                printf("On change de joueur\n");
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
                        playerTurn = (playerTurn + 1) % nbPlayer;
                    } else{

                        turn->cellFrom = (unsigned int)cellFrom;
                        turn->cellTo = (unsigned int)cellTo;
                        runTurn(turn, mapContext);
                    }
                  }
            }
            // On redessine la map
            drawMap(mapContext->cellsList, mapContext->nbNodes);

        }
            //Tour d'une IA
        else {
            printf("Tour de l'IA\n");

            //tant que l'IA veut rejouer
            while (interfaces[players[playerTurn].interface].PlayTurn(mapContext->map, turn)) {

                runTurn(turn, mapContext);
                drawMap(mapContext->cellsList, mapContext->nbNodes);
            }
            //Quand l'ia termine son tour ou coup incorrect
            playerTurn = (playerTurn + 1) % nbPlayer;
        }

    }
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
    }
}

//renvoie l'id de la cellule gagnant le lancé de dé
int whoWins(SCell cellFrom, SCell cellTo){
    int dicesValueFrom = 0;
    for (int i=0; i<cellFrom.nbDices; i++) {
      dicesValueFrom += randomBounds(1,6);
    }

    int dicesValueTo = 0;
    for (int i=0; i<cellTo.nbDices; i++) {
      dicesValueTo += randomBounds(1,6);
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
