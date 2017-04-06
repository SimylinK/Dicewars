#include <stdio.h>
#include "output.h"

FILE *file = NULL;

void outputOpen(){
    if ((file = fopen("../resultats", "w")) == NULL) {
        printf("L*Erreur : l'ouverture du fichier de sortie a échoué.\n");
    }
}

void outputClose(){
    if (file != NULL)  fclose(file);
}

//Affiche info sur les IA
void outputPlayer(int id, SPlayerInfo player){
    if (file != NULL) {
        fprintf(file, "player %d %s", id, player.name);

        //affichage du nom des créateurs de l'IA
        for(int i=0; i<6; i++){ //6 vient de members[6][50] de interface.h, il faudrait pouvoir mettre une variable globale dans interface.h
            fputc(' ', file);
            fputs(player.members[i], file);
        }
        fputc('\n', file);

    } else {
        printf("Un problème a eu lieu lors de l'écriture des logs.\n");
    }
}

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

void outputTurn(int cellFrom, int cellTo, int attackWin){
    if (file != NULL) {
        fprintf(file, "turn %d %d %d\n", cellFrom, cellTo, attackWin);
    } else {
        printf("Un problème a eu lieu lors de l'écriture des logs.\n");
    }

}

void outputEndGame(int idWin){
    char victory[9] = "victory  ";

    if (file != NULL) {
        fputs(victory, file);// le séparateur entre 2 résultats
        fputc(idWin + '0', file);
        fputc('\n', file);
    } else {
        printf("Un problème a eu lieu lors de l'écriture des logs.\n");
    }
}
