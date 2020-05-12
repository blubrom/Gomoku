#include "architecture.h"
#include <stdio.h>

void menu(bool *isRunning, int *joueurActuel, int *nbJoueurs, int ***plateau, bool *mcts){
  int stade = 0;
  while(*isRunning){
    switch(stade){
      case 0 :
            renderMenu();
            break;

      case 1 :
            renderRules();
            break;

      case 2 :
            renderChoixModeJeu();
            break;

      case 3 :
            renderChoixIA();
            break;

      case 4 :
            renderChoixJoueurDebut();
            break;
    }
    handleEventsMenu(isRunning, &stade, plateau, joueurActuel, nbJoueurs, mcts);
    if(stade == 5)
      return;
  }
}

void handleEventsMenu(bool *isRunning, int *stade, int ***plateau, int *joueurActuel, int *nbJoueurs, bool *mcts){
  SDL_Event event;
  switch(*stade){
    case 0 :
      handleMenu(event, isRunning, stade, plateau, joueurActuel, nbJoueurs);
      break;

    case 1 :
      handleRules(event, isRunning, stade);
      break;

    case 2 :
      handleChoixModeJeu(event, isRunning, stade, nbJoueurs);
      break;

    case 3 :
      handleChoixIA(event, isRunning, stade, mcts);

    case 4 :
      handleChoixJoueurDebut(event, isRunning, stade, joueurActuel);
      break;

  }
}

void handleMenu(SDL_Event event, bool *isRunning, int *stade, int ***plateau, int *joueurActuel, int *nbJoueurs){
  while(SDL_PollEvent(&event)){ // PollEvent capte tous les évènements et laisse tourner le programme
    if(event.type == SDL_QUIT)
      *isRunning = false;
    else if(event.type == SDL_MOUSEBUTTONDOWN ){
      if(event.button.button == SDL_BUTTON_LEFT){
        Coup mousePos = {event.button.y, event.button.x};
        int w = 600;
        int h = 150;
        int decalx = 65;
        int decaly = 150;
        int espacement_entre = 20;
        if(mousePos.colonne >= decalx && mousePos.ligne >= decaly  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + h)){ // bouton Règles
          *stade = 1;
        }
        else if(mousePos.colonne >= decalx && mousePos.ligne >= (decaly + h + espacement_entre)  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + 2*h + espacement_entre)){ // bouton Nouvelle Partie
          *stade = 2;
        }
        else if(mousePos.colonne >= decalx && mousePos.ligne >= (decaly + 2*h + 2*espacement_entre)  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + 3*h + 2*espacement_entre)){ // bouton charger
          //gérer le chargement d'une partie
          printf("CHARGE");
        }
        else if(mousePos.colonne >= decalx && mousePos.ligne >= (decaly + 3*h + 3*espacement_entre)  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + 4*h + 3*espacement_entre)) // bouton quitter
          *isRunning = false;
        }
    }
  }
}

void handleRules(SDL_Event event, bool *isRunning, int *stade){
  while(SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT)
      *isRunning = false;
    else if(event.type == SDL_MOUSEBUTTONDOWN ){
      if(event.button.button == SDL_BUTTON_LEFT){
        Coup mousePos = {event.button.y, event.button.x};
        if(mousePos.colonne >= 370 && mousePos.ligne >= 790 && mousePos.colonne <= (370 + 345) && mousePos.ligne <= (790 + 100)){ // retour Menu
            *stade = 0;
        }
      }
    }
  }
}

void handleChoixModeJeu(SDL_Event event, bool *isRunning, int *stade, int *nbJoueurs){
  while(SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT)
      *isRunning = false;
    else if(event.type == SDL_MOUSEBUTTONDOWN ){
      if(event.button.button == SDL_BUTTON_LEFT){
        Coup mousePos = {event.button.y, event.button.x};
        int w = 600;
        int h = 150;
        int decalx = 65;
        int decaly = 250;
        int espacement_entre = 150;
        if(mousePos.colonne >= 370 && mousePos.ligne >= 790  && mousePos.colonne <= (370 + 365) && mousePos.ligne <= (790 + 100)) // retour Menu
          *stade = 0;

        else if(mousePos.colonne >= decalx && mousePos.ligne >= decaly  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + h)){ // un joueur
          *nbJoueurs = 1;
          *stade = 3;
        }

        else if(mousePos.colonne >= decalx && mousePos.ligne >= (decaly + h + espacement_entre)  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + 2*h + espacement_entre)){ // deux joueurs
          *nbJoueurs = 2;
          *stade = 3;
        }
      }
    }
  }
}

void handleChoixIA(SDL_Event event, bool *isRunning, int *stade, bool *mcts){
  while(SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT)
      *isRunning = false;
    else if(event.type == SDL_MOUSEBUTTONDOWN ){
      if(event.button.button == SDL_BUTTON_LEFT){
        Coup mousePos = {event.button.y, event.button.x};
        int w = 600;
        int h = 150;
        int decalx = 65;
        int decaly = 250;
        int espacement_entre = 150;
        if(mousePos.colonne >= 370 && mousePos.ligne >= 790  && mousePos.colonne <= (370 + 345) && mousePos.ligne <= (790 + 100)) // retour Menu
          *stade = 0;

        else if(mousePos.colonne >= decalx && mousePos.ligne >= decaly  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + h)){ // Monte Carlo simple
          *mcts = false;
          *stade = 4;
        }

        else if(mousePos.colonne >= decalx && mousePos.ligne >= (decaly + h + espacement_entre)  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + 2*h + espacement_entre)){ // Monte Carlo Tree Search
          *mcts = true;
          *stade = 4;
        }
      }
    }
  }
}

void handleChoixJoueurDebut(SDL_Event event, bool *isRunning, int *stade, int *joueurActuel){

  while(SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT)
      *isRunning = false;
    else if(event.type == SDL_MOUSEBUTTONDOWN ){
      if(event.button.button == SDL_BUTTON_LEFT){
        Coup mousePos = {event.button.y, event.button.x};
        int w = 345;
        int h = 400;
        int decalx = 15;
        int decaly = 390;
        int espacement_entre = 10;
        if(mousePos.colonne >= 370 && mousePos.ligne >= 790  && mousePos.colonne <= (370 + 345) && mousePos.ligne <= (790 + 100)) // retour Menu
          *stade = 0;

        else if(mousePos.colonne >= decalx && mousePos.ligne >= decaly  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + h)){ // joueur 1 commence
          *joueurActuel = 1;
          *stade = 5;
        }

        else if(mousePos.colonne >= (decalx + w + espacement_entre) && mousePos.ligne >= decaly  && mousePos.colonne <= (decalx + 2*w + espacement_entre) && mousePos.ligne <= (decaly + h)){ // joueur 2 commence
          *joueurActuel = 2;
          *stade = 5;
        }
      }
    }
  }
}



Coup tour_jeu(bool *isRunning, int *** plateau, int joueurActuel){
  Coup coup = {-1, -1};
  bool played = false;
  while(*isRunning && !played){
    while(*isRunning && (coup.ligne < 0 || coup.colonne < 0) ){
      renderGame(*plateau, joueurActuel, false);
      coup = handleGame(isRunning);
    }
    if(coupValide(coup, *plateau)){
      (*plateau)[coup.ligne][coup.colonne] = joueurActuel;
      played = true;
    }
  }
  return coup;
}



void menuFin(bool *isRunning, Gagnant gagnant, int **plateau){
  bool seeBoard = false;
  bool continuePlaying = false;
  while(*isRunning && !continuePlaying){
    if(seeBoard){
      renderGame(plateau, 1, true);
    }
    else{
      renderMenuFin(gagnant);
    }
    handleEventsMenuFin(isRunning, &seeBoard, &continuePlaying);
  }
}

void handleEventsMenuFin(bool *isRunning, bool *seeBoard, bool *continuePlaying){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT)
      *isRunning = false;
    else if(event.type == SDL_MOUSEBUTTONDOWN ){
      if(event.button.button == SDL_BUTTON_LEFT){
        Coup mousePos = {event.button.y, event.button.x};
        if(*seeBoard){ // si l'utilisateur a demandé à revoir le plateau
          if(mousePos.colonne >= 90 && mousePos.ligne >= 50  && mousePos.colonne <= (90 + 820) && mousePos.ligne <= (170)){
              *seeBoard = false;
          } // retour Menu
        }
        else{
          int w = 600;
          int h = 150;
          int decalx = 65;
          int decaly = 250;
          int espacement_entre = 50;
          if(mousePos.colonne >= decalx && mousePos.ligne >= decaly  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + h)) // bouton revoir plateau
            *seeBoard = true;
          else if(mousePos.colonne >= decalx && mousePos.ligne >= (decaly + espacement_entre + h)  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (decaly + espacement_entre + 2*h)) // retour menu principal
            *continuePlaying = true;
          else if(mousePos.colonne >= decalx && mousePos.ligne >= (decaly + 2* espacement_entre + 2*h)  && mousePos.colonne <= (decalx + w) && mousePos.ligne <= (h + decaly + 2* espacement_entre + 3*h )) // bouton quitter
            *isRunning = false;
        }

      }
    }
  }
}
