#include <stdio.h>
#include <string.h>
#include "output.h"

FILE *file = NULL;

//Fonction qui ouvre l'écriture sur le fichier de log
void outputOpen(){
    if ((file = fopen(OUTPUTFILE, "w")) == NULL) {
        printf("Erreur : l'ouverture du fichier de sortie a échoué.\n");
    }
}

//Fonction qui ferme l'écriture sur le fichier de log
void outputClose(){
    if (file != NULL)  fclose(file);
}

//Fonction qui logs les infos d'un IA
//id : l'id de Player de l'IA
//player : les informations du player
void outputPlayer(int id, SPlayerInfo player){
    if (file != NULL) {
        int nameLen = strlen(player.name);
        fprintf(file, "player %d %d %s", id, nameLen, player.name);

        //affichage du nom des créateurs de l'IA
        for(int i=0; i<6; i++){ //6 vient de members[6][50] de interface.h, il faudrait pouvoir mettre une variable globale dans interface.h
            int memberLen = strlen(player.members[i]);
            fprintf(file, " %d %s", memberLen, player.members[i]);
        }
        fputc('\n', file);

    } else {
        printf("Un problème a eu lieu lors de l'écriture des logs.\n");
    }
}

//Fonction qui logs les informations de chaque cellule de la map
//map : la map dont on veut log les informations
void outputInit(SMap *map){
    if (file != NULL) {
        for (int i=0; i<map->nbCells; i++){
            fprintf(file, "cell %d %d %d %d", map->cells[i].id, map->cells[i].owner, map->cells[i].nbDices, map->cells[i].nbNeighbors);

            //affichage des ids des cellules voisines
            for(int j=0; j<map->cells[i].nbNeighbors; j++){
                fprintf(file, " %d", map->cells[i].neighbors[j]->id);
            }
            fputc('\n', file);
        }
    } else {
        printf("Un problème a eu lieu lors de l'écriture des logs.\n");
    }
}

//Fonction qui logs les informations d'un coup joué
//cellFrom : l'id de la cellule d'où part l'attaque
//cellTo : l'id de la cellule qui est attaquée
//attackWin : 1 si l'attque a réussi, 0 si elle a échoué
void outputTurn(int cellFrom, int cellTo, int attackWin){
    if (file != NULL) {
        fprintf(file, "turn %d %d %d\n", cellFrom, cellTo, attackWin);
    } else {
        printf("Un problème a eu lieu lors de l'écriture des logs.\n");
    }

}

//Fonction qui logs le vainqueur de la partie
//idWin : id du joueur qui a gagné
void outputEndGame(int idWin){

    if (file != NULL) {
        fprintf(file, "victory %d\n", idWin);
    } else {
        printf("Un problème a eu lieu lors de l'écriture des logs.\n");
    }
}
