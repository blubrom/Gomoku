#include "cst.h"

#ifndef PLATEAU_H

#define PLATEAU_H
  void dupliquerTableau(int *** plateausrc, int *** plateaudest);
  void initialiserTableau(int *** plateauJeu);
  void miseAZero(int *** plateauJeu);



  Gagnant partieDeuxJoueurs(int *** plateauJeu, int *joueurActuel, bool *isRunning);
  Gagnant  partieSeul(int *** plateauJeu, int *joueurActuel, bool *isRunning, bool mcts);


  Coup tourJeu(int *** plateauJeu, int  joueurActuel, bool *isRunning);
  Gagnant estTermine(int ** plateauJeu, Coup precedentCoupJoueur, int precedentJoueur, bool partieSolo, int tourActuel);
  int nbCoupsValide(int ** plateau, int departLig, int departCol);
  bool coupValide(Coup coup, int ** plateau);
  bool plateauRempli(int ** plateauJeu);
  Gagnant verif_victoire(Coup dernierCoup, int ** plateau, int dernierJoueur, bool partieSolo);
  Coup handleGame(bool *isRunning);



  void rendreMemoire(int *** plateauJeu);
#endif
