#include <SDL.H>
#include "cst.h"

#ifndef AFFICHAGE_H

#define AFFICHAGE_H
  void SDL_ExitWhileRunning(const char * message);
  SDL_Texture* initTexture(const char *fileName, SDL_Renderer *renderer);
  void initSDLGame();
  void destroyTextures();


  void renderMenu();
  void renderRules();
  void renderChoixModeJeu();
  void renderChoixIA();
  void renderChoixJoueurDebut();

  void renderGame(int **plateau, int joueurActuel, bool ended);

  void renderMenuFin(Gagnant gagnant);
#endif
