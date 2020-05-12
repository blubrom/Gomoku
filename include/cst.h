#include <SDL.h>

#ifndef CST_H

#define CST_H
  #define TAILLE 19
  #define TAILLE_CASE 34
  #define TAILLE_TEXTURE 32
  #define TAILLE_PLATEAU 1000
  #define HAUTEUR_FENETRE 900
  #define LARGEUR_FENETRE 730
  #define DECALAGE_HORIZONTAL 43
  #define DECALAGE_VERTICAL_PLATEAU 43
  #define DECALAGE_VERTICAL_HUD 170

  #define COTE_DEMI_CARRE 5
  #define NB_PARTIES_MC 1500
  #define NB_PARTIES_MCTS 50000

  typedef struct Coup{
    int ligne;
    int colonne;
  }Coup;

  typedef enum gagnant{
    J1, J2, IA, NONE, EX_AEQUO
  }Gagnant;

  extern const double rac2;
  extern  SDL_Window *window;
  extern  SDL_Renderer *renderer;

  extern SDL_Texture *textureMenu;
  extern SDL_Texture *textureRegles;
  extern SDL_Texture *textureMenuChoixJoueur;
  extern SDL_Texture *textureMenuChoixMode;
  extern SDL_Texture *textureMenuChoixIA;

  extern SDL_Texture *textureExAeqo;
  extern SDL_Texture *texturePionNoir;
  extern SDL_Texture *texturePionBlanc;
  extern SDL_Texture *textureJeu;

  extern SDL_Texture *textureMenuFin;
  extern SDL_Texture *textureRetourMenuFin;
#endif
