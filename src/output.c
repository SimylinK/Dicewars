#include <stdio.h>

FILE *file = NULL;

void outputOpen(){
    if ((file = fopen("../resultats", "a")) == NULL) {
        printf("L*Erreur : l'ouverture du fichier de sortie a échoué.\n");
    }
}

void outputClose(){
    if (file != NULL)  fclose(file);
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
