#ifndef INIT_H
#define INIT_H

bool initSDL(SDL_Window **pwindow, SDL_Renderer **prender, char title[]);
bool loadImg (SDL_Renderer **prender, SDL_Texture ** ptexture, const char localisation[]);
bool chargerBlocs(SDL_Renderer *prender, SDL_Texture *blocs[]);
unsigned int avancement(bool ecriture, unsigned int tosav);

#endif // INIT_H
