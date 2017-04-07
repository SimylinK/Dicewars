#ifndef DICEWARS_INIT_H
#define DICEWARS_INIT_H

	#include "interface.h"

	// Initialisation de la map
	#define WWIDTH 800 //largeur de la fenetre
	#define WHEIGHT 600 //hauteur de la fenetre

    #define BORDERLANDR 50 //bord sur les cotés gauche et droite de la map
    #define BORDERTOP 50 //bord sur le coté haut de la map

    #define WIDTH WWIDTH-BORDERLANDR //largeur de la map = 750
    #define HEIGHT WHEIGHT-100 //hauteur de la map = 500

	#define nbDicesPerCell 3
	#define minDistBetweenCells 0 // distance en pixels



//Caracteristiques du bouton endturn
#define BUTTONX WWIDTH+15-BUTTONW //coordonnée x du centre du bouton
#define BUTTONY WHEIGHT-50 //coordonnée y du centre du bouton
#define BUTTONW 130
#define BUTTONH 55


//Caracteristiques du logo dicewars
#define TITLEX TITLEW+10 //coordonnée x du centre du bouton
#define TITLEY TITLEH/2 //coordonnée y du centre du bouton
#define TITLEW 230
#define TITLEH 50


    #define DICESWIDTH 15 //largeur de la fenetre
    #define DICESHEIGHT 15 //hauteur de la fenetre

// Une SCell avec des coordonnées
	typedef struct Centre{

		SCell *cell;
		int x;
		int y;
	}Centre;

    typedef SCell* Graph[WIDTH][HEIGHT];

	typedef struct MapContext{
        SMap *map;
        Centre* cellsList;
        unsigned int nbNodes;
        Graph graph;
    } MapContext;


	MapContext* initMap(MapContext *mapContext, unsigned int nbPlayer);
	void assignSCell(unsigned int nbPlayer, unsigned int nbNodes, SMap *map);
	void giveDices(unsigned int nbPlayer, unsigned int nbNodes, SMap *map);
	void neighbors(Centre *cellsList, SMap *map, unsigned int nbNodes);
    void initGraph(MapContext *mapContext, Centre *cellsList, unsigned int nbNodes);

	// Initialisation des joueurs
	typedef void (*pfInitGame)(unsigned int, unsigned int, SPlayerInfo*);
	typedef int (*pfPlayTurn)(unsigned int, const SMap*, STurn*);
	typedef void (*pfEndGame)(unsigned int, unsigned int);

	typedef struct  {
		pfInitGame InitGame;
		pfPlayTurn PlayTurn;
		pfEndGame EndGame;

		void *plib;
	} SInterface;

	typedef struct {
		int id; // l'id du joueur (de 0 a 7)
		int interface; // le numéro de l'interface a utiliser (-1 si joueur humain)
		SPlayerInfo playerInfo;
	} SPlayer;

	int initPlayers(int nbPlayer, SPlayer *players, SInterface *interfaces, int argc, char *argv[]);



#endif //DICEWARS_INIT_H
