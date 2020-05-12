/*
    Objectif : main du projet Gokomu
    Auteur : Clément Morand
    Date : 14/01/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <SDL.h>

#include "IA.h"
#include "cst.h"
#include "plateau.h"
#include "affichage.h"
#include "architecture.h"

int main(int argc, char ** argv){

  srand((unsigned)time(NULL));

  //déclaration des variables
  int ** plateauJeu ; // pour un tableau dynamique 2D d ’ entiers
  int joueurActuel;
  int nbJoueurs;
  Gagnant gagnant;
  bool isRunning = true;
  bool mcts = false;

  //initialisation du jeu
  initSDLGame(); // initialisation des élements relatifs à SDL
  initialiserTableau(&plateauJeu);

  // coeur du jeu
  while(isRunning){
    miseAZero(&plateauJeu);
    menu(&isRunning, &joueurActuel, &nbJoueurs, &plateauJeu, &mcts);
    if(isRunning){
      if(nbJoueurs == 1){
        gagnant = partieSeul(&plateauJeu, &joueurActuel, &isRunning, mcts);
      }else{
        gagnant = partieDeuxJoueurs(&plateauJeu, &joueurActuel, &isRunning);
      }

      menuFin(&isRunning, gagnant, plateauJeu);
    }
  }

  //fin du programme
  rendreMemoire(&plateauJeu);
  destroyTextures();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
