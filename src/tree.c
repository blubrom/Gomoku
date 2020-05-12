/*
    Objectif : fonctions relatives à l'arbre de jeu
    Auteur : Clément MORAND
    Date : 20/04/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "plateau.h"

//fonctions d'implémentation de l'arbre
void initNode(NODE *n, int nbCoupsJoues, int indexParent, int player, Coup coup){
  n->score = 0,0;
  n->nbSimul = 0;
  n->indexParent = indexParent;
  n->coup = coup;
  n->playerTurn = player;
  n->nbChilds = 0;
  n->nbCoupsJoues = nbCoupsJoues;
  n->isTerminalState = false;
  for(int i = 0; i < TAILLE * TAILLE; i++){
    n->indexEnfants[i] = 0;
  }
}


void erreurAlloc(){
  fprintf(stderr, "erreur dans l'allocation mémoire");
  exit(EXIT_FAILURE);
}
