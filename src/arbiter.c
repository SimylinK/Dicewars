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

            int cellFrom = -3;
            printf("C'est au joueur %d de jouer\n", player);
            printf("id de la cellule de départ : \n");
            cellFrom = getIdOnClick(mapContext->nbNodes, mapContext->cellsList);

            //on vérifie que le joueur a cliqué sur la bonne case
            if (player != mapContext->map->cells[cellFrom].owner){
                printf("Ce n'est pas à ce joueur de jouer");
            } else {
                printf("%i\n", cellFrom);

                if (cellFrom == -2) {
                    end = 1;
                } else if (cellFrom == -1) {
                    //passage au joueur suivant
                    player = (player + 1) % nbPlayer;
                } else {

                    int cellTo;
                    printf("id de la cellule d'arrivée : \n");
                    cellTo = getIdOnClick(mapContext->nbNodes, mapContext->cellsList);
                    if (cellTo == -2) {
                        end = 1;
                    } else {

                        turn->cellFrom = cellFrom;
                        turn->cellTo = cellTo;
                        idWinner = runTurn(turn, mapContext);
                        printf("Coup joué : %d vers %d\n", cellFrom, cellTo);

                        // On redessine la map
                        drawMap(mapContext->map, mapContext->cellsList, mapContext->nbNodes);
            }



                }
            }
        }
            //Tour d'une IA
        else {
            printf("Tour de l'IA\n");

            //tant que l'IA veut rejouer
            while(interfaces[player]->PlayTurn(mapContext->map, turn)){

                idWinner = runTurn(turn, mapContext);
                printf("idWinner : %d", idWinner);
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

        if (idWinner == turn->cellFrom) {
            idLoser = turn->cellTo;
        } else {
            idLoser = turn->cellFrom;
        }

        //on actualise le nombre de dés sur chaque cellule
        updateDices(&(mapContext->map->cells[idWinner]), &(mapContext->map->cells[idLoser]));
    }

    return idWinner;
}

//renvoie l'id de la cellule gagnant le lancé de dé
int whoWins(SCell cellFrom, SCell cellTo){
    int dicesValueFrom = randomBounds(1,6)*cellFrom.nbDices;
    int dicesValueTo = randomBounds(1,6)*cellTo.nbDices;

    if(dicesValueFrom > dicesValueTo) {
        return cellFrom.id;
    }else{
        return cellTo.id;
    }
}

void updateDices(SCell *cellWinner, SCell *cellLoser){
    cellLoser->owner = cellWinner->owner;
    cellLoser->nbDices = cellWinner->nbDices - 1;
    cellWinner->nbDices = 1;
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
            printf("on a trouvé cell2 dans la liste des voisins de cell1\n");
            neighbor = 1;
        }
    }
    //on n'a pas trouvé cell2 dans la liste des voisins de cell1
    return neighbor;
}