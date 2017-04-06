#include <stdio.h>
#include "output.h"

FILE *file = NULL;

void outputOpen(){
    if ((file = fopen("../resultats", "a")) == NULL) {
        printf("L*Erreur : l'ouverture du fichier de sortie a échoué.\n");
    }
}

void outputClose(){
    if (file != NULL)  fclose(file);
}

void outputInit(SMap *map){
    for (int i=0; i<map->nbCells; i++){
        fprintf(file, "cell %d %d %d %d", map->cells[i].id, map->cells[i].owner, map->cells[i].nbDices, map->cells[i].nbNeighbors);

        //affichage des ids des cellules voisines
        for(int j=0; j<map->cells[i].nbNeighbors; j++){
            fprintf(file, " %d", map->cells[i].neighbors[j]->id);
        }
        fputc('\n', file);
    }
}

void outputTurn(int cellFrom, int cellTo, int attackWin){
    printf("turn %d %d %d\n", cellFrom, cellTo, attackWin);

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
