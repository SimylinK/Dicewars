#include <stdio.h>

void outputEndGame(int idWin){
  char victory[9] = "victory  ";

  FILE *file = fopen("../resultats", "a");

  if (file != NULL) {
    fputs(victory, file);// le séparateur entre 2 résultats
    fputc(idWin, file);
    fputc("\n", file);
    fclose(file);
  } else {
    printf("Un problème a eu lieu lors de l'écriture des logs.\n");
  }

}
