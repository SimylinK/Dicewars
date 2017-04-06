#ifndef DICEWARS_PROBA_H
#define DICEWARS_PROBA_H

// Tableau des probabiltés des lancers de dés
// tabProbas[0][1] : 0 attaque, 1 défend
double tabProbas[8][8] = {{0.41, 0.83, 0.97, 0.99, 0.99, 0.99, 1.00, 1.00},
                          {0.09, 0.44, 0.78, 0.94, 0.98, 0.99, 0.99, 0.99},
                          {0.01, 0.15, 0.45, 0.74, 0.90, 0.97, 0.99, 0.99},
                          {0.01, 0.04, 0.19, 0.46, 0.72, 0.88, 0.96, 0.98},
                          {0.00, 0.00, 0.06, 0.22, 0.46, 0.70, 0.86, 0.95},
                          {0.00, 0.00, 0.02, 0.08, 0.24, 0.46, 0.68, 0.84},
                          {0.00, 0.00, 0.00, 0.03, 0.10, 0.26, 0.47, 0.67},
                          {0.00, 0.00, 0.00, 0.00, 0.04, 0.12, 0.27, 0.47}};

#endif //DICEWARS_PROBA_H
