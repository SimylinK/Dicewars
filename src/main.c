#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "util.h"
#include "arbiter.h"


int main(int argc, char *argv[]) {
	//Pas assez d'arguments
	if (argc < 2) {
		printf("Erreur !\nIl faut passer au moins 2 arguments : le nombre de parties et le nombre de joueurs\n");
	}
		//Pas assez de joueurs
	else if ((unsigned int) atoi(argv[2]) <= 1 || (unsigned int) atoi(argv[2]) > 8) {
		printf("Erreur !\nLa partie peut comporter de 2 à 8 joueurs\n");
	} else {
		srand((unsigned int) time(NULL));
		unsigned int nbGame = (unsigned int)atoi(argv[1]);
		unsigned int nbPlayer = (unsigned int) atoi(argv[2]);
		// Le nombre d'humains
		//TODO : gérer variable nbHumans (elle est recalculé dans la fonction initPlayers)
		int nbHumans = nbPlayer - argc + 3;

		// Initialisation des joueurs via interfaces

		//Malloc sur les 2 tableaux
		SPlayer *players = malloc(sizeof(SPlayer)*nbPlayer);
		SInterface *interfaces = malloc(sizeof(SInterface)*(nbPlayer - nbHumans));
		if (initPlayers(nbPlayer, players, interfaces, argc, argv) == 1) {
			return 1;
		}

		// nbGame parties vont être lancées
		for (int i=0; i<nbGame; i++){

			MapContext *mapContext = malloc(sizeof(MapContext));
			initMap(mapContext, nbPlayer);

			mainMap(mapContext);

			// L'ordre des joueurs est aléatoire
			SPlayer *playersRandom = malloc(sizeof(SPlayer)*nbPlayer);
			int associate[nbPlayer];
			for (int j=0; j<nbPlayer; j++){
				associate[j]=0;
			}

			for (int j=0; j<nbPlayer; j++){
				int rand = goodRandom(nbPlayer-1);
				while (associate[rand]==1){
					rand = (rand+1)%nbPlayer;
				}
				//TODO : les ligne suivante a été changé pour les tests
				//playersRandom[rand]=players[j];
				//associate[rand]=1;
				playersRandom[j]=players[j];
				associate[j]=1;
			}

			//appel de gameLoop
			int isFirst = 0, isLast = 0;
			if (i == 0) isFirst=1;
			if (i == nbGame-1) isLast=1;

			gameLoop(mapContext, playersRandom, interfaces, nbPlayer, isFirst, isLast);

			// Libération des ressources

			free(playersRandom);
			free(mapContext);
		}
		// Libération des ressources
		freeDl(interfaces, nbPlayer-nbHumans);
		free(interfaces);
		free(players);
	}

	return 0;
}
