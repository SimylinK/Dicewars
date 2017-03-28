#include "arbiter.h"
#include "init.h"
#include "interface.h"
#include "map.h"
#include "util.h"

void gameLoop(MapContext *mapContext, SInterface **interfaces, int nbPlayer) {
    int player = 0;
    int end = 0;
    //Boucle de jeu
    while (!end) {
        int idWinner = -1;
        STurn *turn = malloc(sizeof(STurn));

        if (interfaces[player] == NULL) {
            //Récupération du choix du joueur
            //Tour d'un joueur humain

            int click;
            printf("C'est au joueur %d de jouer\n", player);
            click = getIdOnClick(mapContext->nbNodes, mapContext->cellsList);
            printf("click : %d\n", click);
            if (click == -2) {
                end = 1;
            } else if (click == -1) {
                //passage au joueur suivant
                printf("On change de joueur \"");
                player = (player + 1) % nbPlayer;
            } else {

                //on vérifie que le joueur a cliqué sur la bonne case
                if (player != mapContext->map->cells[click].owner) {
                    printf("Ce n'est pas à ce joueur de jouer");
                } else {
                    int cellFrom = click;
                    int cellTo;
                    cellTo = getIdOnClick(mapContext->nbNodes, mapContext->cellsList);
                    if (cellTo == -2) {
                        end = 1;
                    } else {

                        turn->cellFrom = cellFrom;
                        turn->cellTo = cellTo;
                        idWinner = runTurn(turn, mapContext);
                    }
                  }
            }
            // On redessine la map
            drawMap(mapContext->map, mapContext->cellsList, mapContext->nbNodes);

        }
            //Tour d'une IA
        else {
            printf("Tour de l'IA\n");

            //tant que l'IA veut rejouer
            while (interfaces[player]->PlayTurn(mapContext->map, turn)) {

                idWinner = runTurn(turn, mapContext);
            }
            //Quand l'ia termine son tour ou coup incorrect
            player = (player + 1) % nbPlayer;
        }
    }
}

int runTurn(STurn *turn, MapContext *mapContext) {
    int idWinner = -1;
    int idLoser = -1;

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

    return idWinner;
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
    int autorized = 1;
    if(mapContext->map->cells[turn->cellFrom].nbDices <= 1){
        printf("Erreur : une cellule ne peut pas attaquer si elle n'a qu'un dé\n");
        autorized = 0;
    }else if (!isNeighbor(&(mapContext->map->cells[turn->cellFrom]), &(mapContext->map->cells[turn->cellTo]), mapContext)) {
        printf("Erreur : une cellule ne peut pas attaquer une cellule qui n'est pas voisine\n");
        autorized =  0;
    }
    return autorized;
}

//renvoie 1 si les cellules passées en paramètre sont voisines, 0 sinon
int isNeighbor(SCell *cell1, SCell *cell2, MapContext *mapContext){
    int neighbor = 0;
    for (int i = 0 ;  i < mapContext->nbNodes ; i ++){
        if (cell1->neighbors[i] == cell2){
            //on a trouvé cell2 dans la liste des voisins de cell1
            neighbor = 1;
        }
    }
    //on n'a pas trouvé cell2 dans la liste des voisins de cell1
    return neighbor;
}
