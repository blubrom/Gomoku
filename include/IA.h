#include "cst.h"
#include "tree.h"
#include <stdlib.h>
#include <stdio.h>

#ifndef IA_H

#define IA_H

  Coup monteCarloTreeSearch(int ***plateau, int tourActuel); // MCTS
  // fonctions utilisées par MCTS
  int selection(TREE arbreJeu, int indexNodeActuelle, int ***boardState);
  int expansion(TREE arbreJeu, int indexNodeSelect, int ***boardState, int numeroSimul);
  int simulation(TREE arbreJeu, int numeroNodeSimul, int ***board);
  void backpropagation(TREE arbreJeu, int indexNodeActuelle, int score);

  // fonctions pour gestion de l'ia hors MCTS
  Coup tourIA(int *** plateauJeu, int tourActuel, Coup precCoup, bool mcts);
  bool estTourIA(int joueurActuel);
  Coup tourAlea(int ** plateau);
  Gagnant partieTest(int *** plateauTest, int tourActuel, int joueurActuel, Coup c);
  Coup monteCarlo(int *** plateau, Coup precCoup, int tourActuel);
#endif
