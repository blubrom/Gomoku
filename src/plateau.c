/*
    Objectif : fonctions relatives au plateau de jeu
    Auteur : Clément MORAND
    Date : 09/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

#include "affichage.h"
#include "plateau.h"
#include "cst.h"
#include "IA.h"
#include "architecture.h"
#include "tree.h"

void dupliquerTableau(int *** plateausrc, int *** plateaudest){

  for(int i = 0; i<TAILLE; i++){
    memmove((*plateaudest)[i], (*plateausrc)[i], TAILLE*sizeof(int));
  }
}

void initialiserTableau(int *** plateauJeu){
  (*plateauJeu) = NULL;
  (*plateauJeu) = (int **) malloc (TAILLE * sizeof(int *));
  if(*plateauJeu == NULL)
    erreurAlloc();
  // alllouer chacune des lignes
  {
    for (int  i = 0 ; i < TAILLE ; i++){
        (*plateauJeu)[i] = NULL;
        (*plateauJeu)[i] = (int*) malloc (TAILLE * sizeof(int));
        if((*plateauJeu)[i] == NULL)
          erreurAlloc();
    }
  }
}

void miseAZero(int *** plateauJeu){
    for (int i = 0; i<TAILLE; i++){
      for(int j = 0; j<TAILLE; j++){
        (*plateauJeu)[i][j] = 0;
      }
    }
}


Gagnant partieDeuxJoueurs(int *** plateauJeu, int *joueurActuel, bool *isRunning){

  int tourActuel = 0;
  Coup precedentCoupJoueur = {-1, -1};
  Gagnant gagnant = NONE;
  while(*isRunning && gagnant == NONE){
    precedentCoupJoueur = tour_jeu(isRunning, plateauJeu, *joueurActuel);
    if(*isRunning){
      gagnant = estTermine(*plateauJeu, precedentCoupJoueur, *joueurActuel, false, tourActuel);
      *joueurActuel= ((*joueurActuel) % 2) +1;
      tourActuel++;
    }
  }
  return gagnant;
}

Gagnant partieSeul(int *** plateauJeu, int *joueurActuel, bool *isRunning, bool mcts){

  int tourActuel = 0;
  Coup precedentCoup = {-1, -1};
  Gagnant gagnant = NONE;
  while(*isRunning && gagnant == NONE){
    if(estTourIA(*joueurActuel)){
      precedentCoup = tourIA(plateauJeu, tourActuel, precedentCoup, mcts);
    }
    else{
      precedentCoup = tour_jeu(isRunning, plateauJeu, *joueurActuel);
    }
    if(*isRunning){
      gagnant = estTermine(*plateauJeu, precedentCoup, *joueurActuel, true, tourActuel);
      *joueurActuel= ((*joueurActuel) % 2) +1;
      tourActuel++;
    }
  }
  return gagnant;
}



Coup handleGame(bool *isRunning){
  Coup coup = {-1, -1};
  SDL_Event event;

  while(SDL_PollEvent(&event)){ // PollEvent capte tous les évènements et laisse tourner le programme
    switch(event.type){
      case SDL_MOUSEBUTTONDOWN :
        if(event.button.button == SDL_BUTTON_LEFT){ // switch sur le bouton de la souris qui a été clické
          Coup mousePos = {event.button.y,event.button.x};
          if(mousePos.colonne >= DECALAGE_HORIZONTAL && mousePos.ligne >= (DECALAGE_VERTICAL_HUD + DECALAGE_VERTICAL_PLATEAU) && mousePos.colonne <= (LARGEUR_FENETRE -  DECALAGE_HORIZONTAL) && mousePos.ligne <= (HAUTEUR_FENETRE - DECALAGE_VERTICAL_PLATEAU) ){
            coup.colonne = (mousePos.colonne - DECALAGE_HORIZONTAL) / TAILLE_CASE;
            coup.ligne = (mousePos.ligne - DECALAGE_VERTICAL_HUD - DECALAGE_VERTICAL_PLATEAU) / TAILLE_CASE;
          }
          else if(mousePos.colonne >= 480 && mousePos.ligne >= 90  && mousePos.colonne <= (480 + 200) && mousePos.ligne <= (90 + 60)){ // bouton quitter
            *isRunning = false;
            return coup;
          }
          else if(mousePos.colonne >= 480 && mousePos.ligne >= 20  && mousePos.colonne <= (480 + 200) && mousePos.ligne <= (20 + 60)){ // bouton sauvegarder
            // gérer sauvegarde
          }
        }
        break;

      case SDL_QUIT : // lorsque l'utilisateur veut quitter le programme en fermant la fenêtre
        *isRunning = false;
        return coup;
        break;

      default :
        break;
    }
  }
  return coup;
}


int nbCoupsValide(int ** plateau, int departLig, int departCol){
  int ct = 0;
  for(int i = -COTE_DEMI_CARRE; i<COTE_DEMI_CARRE; i++){
    for(int j = -COTE_DEMI_CARRE; j<COTE_DEMI_CARRE; j++){
      Coup c = {i + departLig, j + departCol};
      if(coupValide(c, plateau))
        ct ++;
    }
  }
  return ct;
}

bool coupValide(Coup coup, int ** plateau){
    if((coup.ligne > (TAILLE-1)) || (coup.ligne < 0) || (coup.colonne > (TAILLE - 1)) || (coup.colonne < 0) || plateau[coup.ligne][coup.colonne] != 0)
      return false;
    return true;
}

Gagnant estTermine(int ** plateauJeu, Coup precedentCoupJoueur, int precedentJoueur, bool partieSolo, int tourActuel){

  if(tourActuel == TAILLE * TAILLE)
    return EX_AEQUO;
  Gagnant test = verif_victoire(precedentCoupJoueur, plateauJeu, precedentJoueur, partieSolo);
  return test;
}

Gagnant verif_victoire(Coup dernierCoup, int ** plateau, int dernierJoueur, bool partieSolo){
    int joueurTest = dernierJoueur;
    Gagnant joueurVictoire;
    if(joueurTest == 1)
      joueurVictoire = J1;
    else if(partieSolo)
      joueurVictoire = IA;
    else
      joueurVictoire = J2;
    // si on est au premier tour la partie c'est forcément pas fini
    if(dernierCoup.ligne == -1 && dernierCoup.colonne == -1){
      return NONE;
    }

    int ctVertical = 0, ctHorizontal = 0, ctDiagonalHautVersBas = 0, ctDiagonalBasVersHaut = 0;
    int i = 0;

    //victoire selon la diagonale allant de Haut en bas
    while( (dernierCoup.ligne - i) >= 0 && (dernierCoup.colonne - i) >= 0 && plateau[dernierCoup.ligne-i][dernierCoup.colonne-i] == joueurTest){
      ctDiagonalHautVersBas++;
      i++;
    }
    if(ctDiagonalHautVersBas >= 5){return joueurVictoire;}

    i = 1; // alternance 0 et 1 pour ne pas tester deux fois la case dont on part
    while( (dernierCoup.ligne + i) < TAILLE && (dernierCoup.colonne + i) < TAILLE && plateau[dernierCoup.ligne+i][dernierCoup.colonne+i] == joueurTest){
      ctDiagonalHautVersBas++;
      i++;
    }
    if(ctDiagonalHautVersBas >= 5){return joueurVictoire;}

    //victoire sur la diagonale allant de bas en haut
    i = 0;
    while( (dernierCoup.ligne - i) >= 0 && (dernierCoup.colonne + i)  < TAILLE && plateau[dernierCoup.ligne-i][dernierCoup.colonne+i] == joueurTest){
      ctDiagonalBasVersHaut++;
      i++;
    }
    if(ctDiagonalBasVersHaut >= 5){return joueurVictoire;}

    i = 1;
    while( (dernierCoup.ligne + i) < TAILLE && (dernierCoup.colonne - i)  >= 0 && plateau[dernierCoup.ligne+i][dernierCoup.colonne-i] == joueurTest){
      ctDiagonalBasVersHaut++;
      i++;
    }
    if(ctDiagonalBasVersHaut >= 5){return joueurVictoire;}

    //victoire selon la colonne
    i = 0;
    while( (dernierCoup.colonne - i)  >= 0 && plateau[dernierCoup.ligne][dernierCoup.colonne - i] == joueurTest){
      ctVertical++;
      i++;
    }
    if(ctVertical >= 5){return joueurVictoire;}

    i = 1;
    while( (dernierCoup.colonne + i)  < TAILLE && plateau[dernierCoup.ligne][dernierCoup.colonne+i] == joueurTest){
      ctVertical++;
      i++;
    }
    if(ctVertical >= 5){return joueurVictoire;}

    //victoire selon la ligne
    i = 0;
    while( (dernierCoup.ligne + i) < TAILLE && plateau[dernierCoup.ligne + i][dernierCoup.colonne] == joueurTest){
      ctHorizontal++;
      i++;
    }
    if(ctHorizontal >= 5){return joueurVictoire;}

    i = 1;
    while( (dernierCoup.ligne - i) >= 0 && plateau[dernierCoup.ligne - i][dernierCoup.colonne] == joueurTest){
      ctHorizontal++;
      i++;
    }
    if(ctHorizontal >= 5){return joueurVictoire;}

    return NONE;
}



void rendreMemoire(int *** plateauJeu){
  // rendre la mémoire au système après utilisation
  for(int i = 0 ; i < TAILLE ; i++)
    free((*plateauJeu)[i]) ;
  free(*plateauJeu);
  *plateauJeu = NULL;
}
