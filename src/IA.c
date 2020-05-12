/*
    Objectif : fonctions relatives à l'IA
    Auteur : Clément MORAND
    Date : 20/04/2020
*/

// possibilité d'implémenter minimax avec alpha-beta prunning et une methode monte carlo comme fonction d'évaluation
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "IA.h"
#include "cst.h"
#include "plateau.h"
#include "tree.h"

bool estTourIA(int joueurActuel){
  return (joueurActuel == 2);
}

Coup tourAlea(int ** plateau){
  Coup coupAlea;
  do{
    coupAlea.ligne = rand()%TAILLE;
    coupAlea.colonne = rand()%TAILLE;
  }while(!coupValide(coupAlea,plateau));

  return coupAlea;
}

Gagnant partieTest(int *** plateauTest, int tourActuel, int joueurActuel, Coup c){
  // fini la partie en utilisant uniquement des coups aléatoires
  int joueur = joueurActuel;
  Coup coup = c;
  int tour = tourActuel;
  Gagnant gagnantTest = estTermine(*plateauTest, coup, joueurActuel, true, tour);
  int **plateau = NULL;
  initialiserTableau(&plateau);
  if(plateau == NULL)
    erreurAlloc();
  dupliquerTableau(plateauTest, &plateau);

  while(gagnantTest == NONE){
    joueur = (joueur%2) + 1;
    coup = tourAlea(plateau);
    plateau[coup.ligne][coup.colonne] = joueur;
    tour++;
    gagnantTest = estTermine(plateau, coup, joueur, true, tour);
  }
  rendreMemoire(&plateau);
  return gagnantTest;
}

Coup tourIA(int *** plateauJeu, int tourActuel, Coup precCoup, bool mcts){
  Coup coupIa;

  if(tourActuel == 0){
    int x = (TAILLE + 1) / 2 - 1;
    coupIa.ligne = x;
    coupIa.colonne = x;
  }

  else{
    if(mcts)
      coupIa = monteCarloTreeSearch(plateauJeu, tourActuel);
    else
      coupIa = monteCarlo(plateauJeu, precCoup, tourActuel);
  }
  (*plateauJeu)[coupIa.ligne][coupIa.colonne] = 2;
  return coupIa;
}

Coup monteCarlo(int *** plateau, Coup precCoup, int tourActuel){
  Coup coupIa;
  Coup coupTmp;
  int ct = 0;
  int ctTest;
  // pour chaque case
  for(int i = -COTE_DEMI_CARRE; i<= COTE_DEMI_CARRE; i++){
    for(int j = -COTE_DEMI_CARRE; j<= COTE_DEMI_CARRE; j++){
      coupTmp.ligne = precCoup.ligne + i;
      coupTmp.colonne = precCoup.colonne + j;
      if(coupValide(coupTmp, *plateau)){
        ctTest = 0;
        (*plateau)[coupTmp.ligne][coupTmp.colonne] = 2;
        for(int k = 0; k < NB_PARTIES_MC; k++){
          int gagnantTest = partieTest(plateau, tourActuel, 2, coupTmp);
          if(gagnantTest == IA)
            ctTest++;
        }
        (*plateau)[coupTmp.ligne][coupTmp.colonne] = 0;
        // si on a un meilleur résultat que le récédent, on change de coup à jouer
        if(ctTest > ct){
          ct = ctTest;
          coupIa.ligne = coupTmp.ligne;
          coupIa.colonne = coupTmp.colonne;
        }
      }
    }
  }
  return coupIa;
}


// renvoie l'index du sous arbre selectionné
int selection(TREE arbreJeu, int indexNodeActuelle, int ***boardState){
  /*
    calculer pour chaque fils, la valeur de UCT (Upper bound Confidence for Trees),
    selectioner le fils avec la plus grande valeur puis relancer jusqu'à atteindre une feuille
  */
  //printf("Se");
  //on met le plateau de jeu à jour
  Coup c = arbreJeu[indexNodeActuelle].coup;
  if(c.ligne != -1 && c.colonne != -1)
    (*boardState)[c.ligne][c.colonne] = arbreJeu[indexNodeActuelle].playerTurn;


  int numFils = TAILLE * TAILLE - arbreJeu[indexNodeActuelle].nbCoupsJoues;

  //cas trivial d'arrêt de la réccurence
  if(arbreJeu[indexNodeActuelle].nbChilds < numFils)
    return indexNodeActuelle;

  // si tous les fils ont étés générés on choisi celui qui maximise la valeur UCB
  int indexFilsSelect = 0;
  double coeffchoix = 0;
  for(int i = 0; i < numFils; i++){
    int nbSimulEnfant = arbreJeu[(arbreJeu[indexNodeActuelle].indexEnfants[i])].nbSimul;
    int nbSimulNodeActuelle = arbreJeu[indexNodeActuelle].nbSimul;
    int scoreEnfant = arbreJeu[(arbreJeu[indexNodeActuelle].indexEnfants[i])].score;
    // si on peut calculer la valeur UCT du noeud, on la calcule et on la compare à la valeur max obtenue précédemment
    double UCB_enfant =  scoreEnfant / nbSimulEnfant + rac2 * sqrt(log(nbSimulNodeActuelle) / nbSimulEnfant);
    // si la valeur UCT du noeud est plus grande que la précédente valeur maximale, on change la valeur du n° fils choisi et on update le coeffmax
    if(UCB_enfant > coeffchoix){
      indexFilsSelect = i;
      coeffchoix = UCB_enfant;
    }
  }
  selection(arbreJeu, arbreJeu[indexNodeActuelle].indexEnfants[indexFilsSelect], boardState);
}

// permet la création de nouveaux noeuds et de sélectionner le coup pour simulation
int expansion(TREE arbreJeu, int indexNodeSelect, int ***boardState, int numeroSimul){

  //printf("Ex");
  // gérer le cas où on est sur un état terminal de la partie ou de la première simulation (il ne sert à rien de simuler sur le plateau à vide)
  if((arbreJeu[indexNodeSelect].nbSimul == 0 && numeroSimul != 1)|| arbreJeu[indexNodeSelect].isTerminalState)
    return indexNodeSelect;
  else{
    // création d'un fils par coup possible
    int newCurrPlayer = (arbreJeu[indexNodeSelect].playerTurn)%2 + 1;
    Coup currCoup = arbreJeu[indexNodeSelect].coup;
    bool choix = false;
    Coup c;
    int ct = 0; // compteur de cases essayées pouvant être jouées, correspondant aux case qui doivent être mises comme fils
    int i = 0;
    while( (i <= TAILLE) && !choix){
      int j = 0;
      while((j < TAILLE) && !choix){
        // on commence au milieu du plateau
        c.ligne = (i + (TAILLE + 1)/ 2 - 1) %TAILLE;
        c.colonne = (j + (TAILLE + 1)/ 2 - 1) %TAILLE;
        if(coupValide(c, *boardState)){ // coup possible
          if(ct == arbreJeu[indexNodeSelect].nbChilds){ // si c'est un coup qui n'a pas encore été mis comme fils
            choix = true;
            arbreJeu[indexNodeSelect].indexEnfants[arbreJeu[indexNodeSelect].nbChilds] = numeroSimul;
            arbreJeu[indexNodeSelect].nbChilds += 1;
            initNode(&(arbreJeu[numeroSimul]), arbreJeu[indexNodeSelect].nbCoupsJoues + 1, indexNodeSelect, newCurrPlayer, c);
          }
          else
            ct ++;
        }
        j++;
      }
      i++;
    }
    //mise à jour du plateau de jeu
    (*boardState)[c.ligne][c.colonne] = newCurrPlayer;
    return numeroSimul;
  }

}

// simule la partie jusqu'à son terme et renvoie le score de la partie
int simulation(TREE arbreJeu, int numeroNodeSimul, int ***board){
  /*
    on part de l'état actuel du jeu et on simule des coups aléatoires des deux cotés
    pour faire avancer la partie jusqu'à un terme (vicctoire d'un des joueurs ou égalité)
    on renvoie une valeur correspondant au cas ex: 0 si défaite, 1 si égalité et 2 si victoire
  */
  //printf("Si");
  Gagnant result = estTermine(*board, arbreJeu[numeroNodeSimul].coup, arbreJeu[numeroNodeSimul].playerTurn, true, arbreJeu[numeroNodeSimul].nbCoupsJoues);
  if(result != NONE)
    // si la partiest finie, alors c'est un état terminal
    arbreJeu[numeroNodeSimul].isTerminalState = true;
  else
    result = partieTest(board, arbreJeu[numeroNodeSimul].nbCoupsJoues, arbreJeu[numeroNodeSimul].playerTurn, arbreJeu[numeroNodeSimul].coup);
  switch(result){
    case EX_AEQUO :
      return 2.5;
    case J1 : // défaite
      return 0.0;
    case IA : // victoire
      return 5.0;
    default : //erreur
      return -1.0;
  }
}

// mise à jour de tous les noeuds impliqués
void backpropagation(TREE arbreJeu, int indexChoix, int score){
  /*
    on met à jour le noeud et tous ses parents avec les informations :
    on augmente le nombre de visites et le score de victoires
  */
  //printf("Ba");
  if(score == -1.0)
    printf("Erreur");
  arbreJeu[indexChoix].score += score;
  arbreJeu[indexChoix].nbSimul += 1;
  if(arbreJeu[indexChoix].indexParent != -1)
    backpropagation(arbreJeu, arbreJeu[indexChoix].indexParent, score);
}

Coup monteCarloTreeSearch(int ***plateau, int tourActuel){
  Coup coup = {-1, -1};
  /* initialisation de l' arbre*/
  TREE arbreJeu = malloc((NB_PARTIES_MCTS + 1) * sizeof(NODE)); // l'indice 0 correspond à la racine
  initNode(&(arbreJeu[0]), tourActuel, -1, 1, coup);
  /* répétitions de l'alogrithme jusqu'à avoir fait le nombre de simulations voulues */
  int **simulationBoard;
  initialiserTableau(&simulationBoard);
  for(int i = 1; i< NB_PARTIES_MCTS + 1; i++){
    dupliquerTableau(plateau, &simulationBoard);
    int indexSelect = selection(arbreJeu, 0, &simulationBoard);
    int  indexChoix = expansion(arbreJeu, indexSelect, &simulationBoard, i);
    int score = simulation(arbreJeu, indexChoix, &simulationBoard);
    backpropagation(arbreJeu, indexChoix, score);
  }
  rendreMemoire(&simulationBoard);
  //choix de la branche la plus visitée
  double meilleurScoreMoyen = 0.0;
  int numeroChoix = 0;
  for(int i = 0; i< arbreJeu[0].nbChilds; i++){
    int currNbVisites = arbreJeu[arbreJeu[0].indexEnfants[i]].nbSimul;
    double currScore = arbreJeu[arbreJeu[0].indexEnfants[i]].score;
    double currScoreMoyen = currScore / currNbVisites;
    if(currScoreMoyen > meilleurScoreMoyen){
      meilleurScoreMoyen = currScoreMoyen;
      numeroChoix = i;
    }
  }
  coup.ligne = arbreJeu[arbreJeu[0].indexEnfants[numeroChoix]].coup.ligne;
  coup.colonne = arbreJeu[arbreJeu[0].indexEnfants[numeroChoix]].coup.colonne;
  free(arbreJeu);
  return coup;
}
