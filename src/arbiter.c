#include <stdio.h>
#include "arbiter.h"

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
