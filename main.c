#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

int main (int argc, char *argv[]){
    unsigned int nbGame = (unsigned int)atoi(argv[1]);
    unsigned int nbPlayer = (unsigned int)atoi(argv[2]);

    SPlayerInfo info = {.name = "stratGroupe5", .members = {"Monvoisin Mathilde\0", "Le Priol Yoann\0", "Maraval Nathan\0", "Pagano Lucas\0"}};

    for (int i = 0; i<nbGame; i++) InitGame(i, nbPlayer, &info);


    return 0;
}