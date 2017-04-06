Projet C d'une IA et interface graphique pour le jeu Dicewars : https://jayisgames.com/review/dice-wars.php




Règles implicites :

        Map : Graphe planaire avec une seule composante connexe. de 30 à 60 sommets.
        Choix du nombre de sommets quasi uniforme par équipe (différence max de 1)
        Répartition aléatoire du nombre de dés par sommet d'une équipe (avec au min 1 dé par sommet)
        Pile de dés de 40 éléments maximum
        Pas de temps max à contrôler par tour (mais restez raisonnables !)

Pour les logs :
        Le chemin vers le fichier de sortie est modifiable dans le fichier src/output
        Il est défini par la macro OUTPUTFILE


Pour compiler une bibliothèque dynamique :
  gcc -std=c99 -fPIC -c  [...].c
  gcc -shared -o [nom lib].so [...].o

Exemple d'éxecution :
  ./test 1 2 ./lib.so
  (1 partie, 2 joueurs, l'ia utilise la bibliothèque lib.so)
