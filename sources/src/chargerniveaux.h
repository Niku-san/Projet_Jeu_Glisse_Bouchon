#ifndef CHARGERNIVEAUX_H
#define CHARGERNIVEAUX_H

bool chargerNiveau(char niveau[][NB_BLOCS_HAUTEUR], SDL_Rect *dst, char levelnb[], level *levels, const bool personnalise);
void afficheNiveau(const char niveau[][NB_BLOCS_HAUTEUR], SDL_Renderer *prender, SDL_Texture *blocs[]);
bool chargerNiveauEdition(char niveau[][NB_BLOCS_HAUTEUR]);

#endif // CHARGERNIVEAUX_H
