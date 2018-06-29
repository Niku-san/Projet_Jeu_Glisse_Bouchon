#ifndef JEU_H
#define JEU_H

void jeu(SDL_Renderer *prender, SDL_Texture *blocs[], const bool personnalise, int nbLevels);
int touches (SDL_Rect *personnage, const bool key[], int *direction, char niveau[][NB_BLOCS_HAUTEUR], level *levelActu, int *fixed);
void tableauTouches (bool key[], SDL_Event *ev, const bool key_edit);
void changeWall(char niveau[][NB_BLOCS_HAUTEUR]);


#endif // JEU_H
