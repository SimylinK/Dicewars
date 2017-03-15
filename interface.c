#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

// Structure pour garder les informations du contexte de la partie
typedef struct
{
	int id;	// identifiant de l'IA
	int nbPlayer;	// Noms des personnes participant au projet.
} SContext;

SContext contexte; // variable global du contexte

void InitGame(unsigned int id, unsigned int nbPlayer, SPlayerInfo *info){
    //printf("IA %i initialized\n", id);

    contexte.id = id;
    contexte.nbPlayer = nbPlayer;

    strcpy(info->name, "interface");
}
