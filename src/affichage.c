#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cst.h"
#include "affichage.h"

void SDL_ExitWhileRunning(const char * message){
  destroyTextures();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Log("Erreur : %s > %s",message, SDL_GetError());
  SDL_Quit();
  exit(EXIT_FAILURE);
}

void initSDLGame(){

  //initialisation de la SDL avec gestion des erreurs d'initialisation
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    SDL_ExitWhileRunning("Initialisation echouee");

  // création de la fenêtre avec test de la bonne execution de la fonction
  window = SDL_CreateWindow("Jeu de gomoku",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,LARGEUR_FENETRE,HAUTEUR_FENETRE,0);

  if(window == NULL)
    SDL_ExitWhileRunning("creation de fenetre echouee");

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  if(renderer == NULL)
    SDL_ExitWhileRunning("creation rendu echouee");

  textureMenu = initTexture("../src/images/menuPrincipal.bmp", renderer);
  textureRegles = initTexture("../src/images/menuRègles.bmp", renderer);
  textureMenuChoixJoueur = initTexture("../src/images/menuChoixJoueurDebut.bmp", renderer);
  textureMenuChoixMode = initTexture("../src/images/menuChoixModeJeu.bmp", renderer);
  textureMenuChoixIA = initTexture("../src/images/menuChoixIA.bmp", renderer);

  textureExAeqo = initTexture("../src/images/Ex_aeqo.bmp", renderer);
  texturePionNoir = initTexture("../src/images/pion_noir.bmp", renderer);
  texturePionBlanc = initTexture("../src/images/pion_blanc.bmp", renderer);
  textureJeu = initTexture("../src/images/jeu.bmp", renderer);

  textureMenuFin = initTexture("../src/images/menuFin.bmp", renderer);
  textureRetourMenuFin = initTexture("../src/images/retourFin.bmp", renderer);
}

SDL_Texture* initTexture(const char *fileName, SDL_Renderer *renderer){
  SDL_Surface *imagetmp = SDL_LoadBMP(fileName);
  if(imagetmp == NULL)
    SDL_ExitWhileRunning("impossible de charger l'image");
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, imagetmp);
  if(texture == NULL)
    SDL_ExitWhileRunning("impossible de créer la texture");
  SDL_FreeSurface(imagetmp);
  return texture;
}

void destroyTextures(){
  SDL_DestroyTexture(textureMenu);
  SDL_DestroyTexture(textureRegles);
  SDL_DestroyTexture(textureMenuChoixJoueur);
  SDL_DestroyTexture(textureMenuChoixMode);
  SDL_DestroyTexture(textureJeu);
  SDL_DestroyTexture(texturePionNoir);
  SDL_DestroyTexture(texturePionBlanc);
  SDL_DestroyTexture(textureMenuFin);
  SDL_DestroyTexture(textureMenuChoixIA);
}

void renderMenu(){
  if(SDL_RenderClear(renderer) != 0) // clear le rendu
    SDL_ExitWhileRunning("Effacement rendu echoue");

  SDL_RenderCopy(renderer, textureMenu, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void renderRules(){
  if(SDL_RenderClear(renderer) != 0) // clear le rendu
    SDL_ExitWhileRunning("Effacement rendu echoue");

  SDL_RenderCopy(renderer, textureRegles, NULL, NULL);

  SDL_RenderPresent(renderer);
}

void renderChoixModeJeu(){

  if(SDL_RenderClear(renderer) != 0) // clear le rendu
    SDL_ExitWhileRunning("Effacement rendu echoue");

  SDL_RenderCopy(renderer, textureMenuChoixMode, NULL, NULL);

  SDL_RenderPresent(renderer);
}

void renderChoixIA(){
  if(SDL_RenderClear(renderer) != 0) // clear le rendu
    SDL_ExitWhileRunning("Effacement rendu echoue");

  SDL_RenderCopy(renderer, textureMenuChoixIA, NULL, NULL);

  SDL_RenderPresent(renderer);
}

void renderChoixJoueurDebut(){

  if(SDL_RenderClear(renderer) != 0) // clear le rendu
    SDL_ExitWhileRunning("Effacement rendu echoue");

  SDL_RenderCopy(renderer, textureMenuChoixJoueur, NULL, NULL);

  SDL_RenderPresent(renderer);
}

void renderGame(int **plateau, int joueurActuel, bool ended){

  if(SDL_RenderClear(renderer) != 0) // clear le rendu
    SDL_ExitWhileRunning("Effacement rendu echoue");

  SDL_Rect rectPions = {DECALAGE_HORIZONTAL , DECALAGE_VERTICAL_HUD + DECALAGE_VERTICAL_PLATEAU, TAILLE_TEXTURE, TAILLE_TEXTURE};
  SDL_Rect rect = {410, 75, TAILLE_TEXTURE, TAILLE_TEXTURE};

  SDL_RenderCopy(renderer, textureJeu, NULL, NULL);

  // affichage du joueur actuel
  if(!ended){
    if(joueurActuel == 1)
      SDL_RenderCopy(renderer, texturePionNoir, NULL, &rect);
    else
      SDL_RenderCopy(renderer, texturePionBlanc, NULL, &rect);
  }else{
    SDL_Rect rectHUD = {0,0, 730, 170};
    SDL_RenderCopy(renderer, textureRetourMenuFin, NULL, &rectHUD);
  }
  // affichage des pions sur le plateau
  for(int i =0; i<TAILLE; i++){
    for(int j = 0; j<TAILLE; j++){
      switch(plateau[i][j]){
        case 1:
          SDL_RenderCopy(renderer, texturePionNoir, NULL, &rectPions);
          break;

        case 2:
          SDL_RenderCopy(renderer, texturePionBlanc, NULL, &rectPions);
          break;

        default :
          break;
      }
      rectPions.x += TAILLE_CASE;
    }
    rectPions.x = DECALAGE_HORIZONTAL ;
    rectPions.y += TAILLE_CASE;
  }

  SDL_RenderPresent(renderer);
}

void renderMenuFin(Gagnant gagnant){

  if(SDL_RenderClear(renderer) != 0) // clear le rendu
    SDL_ExitWhileRunning("Effacement rendu echoue");
  SDL_RenderCopy(renderer, textureMenuFin, NULL, NULL);
  SDL_Rect rectPion = {400, 180, TAILLE_TEXTURE, TAILLE_TEXTURE};
  // affichage du vainqueur
  if(gagnant == J1)
    SDL_RenderCopy(renderer, texturePionNoir, NULL, &rectPion);
  else if(gagnant == J2 || gagnant == IA)
    SDL_RenderCopy(renderer, texturePionBlanc, NULL, &rectPion);
  else
    SDL_RenderCopy(renderer, textureExAeqo, NULL, &rectPion);

  SDL_RenderPresent(renderer);
}
