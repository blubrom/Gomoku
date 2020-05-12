#include <stdbool.h>

#include "plateau.h"
#include "affichage.h"


#ifndef ARCHITEXTURE_H

#define ARCHITEXTURE_H
  void menu(bool *isRunning, int *joueurActuel, int *nbJoueurs, int ***plateau, bool *mcts);
  void handleEventsMenu(bool *isRunning, int *stade, int ***plateau, int *joueurActuel, int *nbJoueurs, bool *mcts);
  void handleMenu(SDL_Event event, bool *isRunning, int *stade, int ***plateau, int *joueurActuel, int *nbJoueurs);
  void handleRules(SDL_Event event, bool *isRunning, int *stade);
  void handleChoixModeJeu(SDL_Event event, bool *isRunning, int *stade, int *nbJoueurs);
  void handleChoixIA(SDL_Event event, bool *isRunning, int *stade, bool *mcts);
  void handleChoixJoueurDebut(SDL_Event event, bool *isRunning, int *stade, int *joueurActuel);


  Coup tour_jeu(bool *isRunning, int *** plateau, int joueurActuel);



  void menuFin(bool *isRunning, Gagnant gagnant, int **plateau);
  void handleEventsMenuFin(bool *isRunning, bool *seeBoard, bool *continuePlaying);

#endif
