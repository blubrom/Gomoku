#ifndef TREE_H
#define TREE_H

#include "cst.h"

  typedef struct node{
    double score;
    int nbSimul;
    int indexParent;
    int indexEnfants[TAILLE * TAILLE]; 
    Coup coup;
    int playerTurn; // n° du joueur qui doit jouer ce coup
    int nbChilds;
    int nbCoupsJoues;
    bool isTerminalState; //si la partie est terminée à ce noeud
  }NODE, *TREE;

  //fonctions d'implémentation de l'arbre
  void initNode(NODE *n, int nbCoupsJoues, int indexParent, int player, Coup coup);


  void erreurAlloc();

#endif
