#include "includes.h"

bool initSDL(SDL_Window **pwindow, SDL_Renderer **prender, char title[]) //initialise la SDL2
{
    bool error = false;

    *pwindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDHT, HEIGHT, SDL_WINDOW_SHOWN);
    if (*pwindow == NULL)
    {
        fprintf(stderr, "ERREUR CREATION FENETRE : %s", SDL_GetError());
        error = true;
    }

    *prender = SDL_CreateRenderer(*pwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*prender == NULL)
    {
        fprintf(stderr, "ERREUR CREATION RENDERER : %s", SDL_GetError());
        error = true;
    }

    SDL_Surface *picon = NULL;
    picon = IMG_Load("img/icon.png");

    SDL_SetWindowIcon(*pwindow, picon);

    SDL_FreeSurface(picon);
    picon = NULL;

    return error;
}

bool loadImg (SDL_Renderer **prender, SDL_Texture ** ptexture, const char localisation[]) //charge une image
{
    bool error = false;
    SDL_Surface *tmp = NULL;
    tmp = IMG_Load(localisation);
    if (tmp == NULL)
    {
        fprintf(stderr, "ERREUR CHARGEMENT IMAGE : %s", SDL_GetError());
        error = true;
    }

    *ptexture = SDL_CreateTextureFromSurface(*prender, tmp);
    if (*ptexture == NULL)
    {
        fprintf(stderr, "ERREUR DEPLACEMENT IMAGE DANS LA TEXTURE : %s", SDL_GetError());
        error = true;
    }

    return error;
}

bool chargerBlocs(SDL_Renderer *prender, SDL_Texture *blocs[])
{
    bool errors[OBJETMAX] = {false};
    errors[0] = loadImg(&prender, &blocs[MUR], "img/mur.png");
    errors[1] = loadImg(&prender, &blocs[OBJECTIF], "img/bouchon.png");
    errors[2] = loadImg(&prender, &blocs[STOPPER], "img/stopper.png");
    errors[3] = loadImg(&prender, &blocs[SPAWN], "img/spawn.png");
    errors[4] = loadImg(&prender, &blocs[REHAUT], "img/Redirect_Haut.png");
    errors[5] = loadImg(&prender, &blocs[REDROITE], "img/Redirect_Droite.png");
    errors[6] = loadImg(&prender, &blocs[REBAS], "img/Redirect_Bas.png");
    errors[7] = loadImg(&prender, &blocs[REGAUCHE], "img/Redirect_Gauche.png");
    errors[8] = loadImg(&prender, &blocs[MURINV], "img/mur_inv.png");
    errors[9] = loadImg(&prender, &blocs[MUROPA], "img/mur_opa.png");

    int i = 0;
    for (i = 0; i < OBJETMAX-1; i++)
    {
        if (errors[i])
        {
            i = OBJETMAX;
            errors[i] = true;
        }
    }

    return errors[OBJETMAX];
}

unsigned int avancement(bool ecriture, unsigned int tosav)
{
    unsigned int save = 0;
    bool pass = 0;
    FILE *ficsav = NULL;

    if(!ecriture)
    {
        ficsav = fopen("levels/data.bin", "rb");
    }

    if(ecriture)
    {
        ficsav = fopen("levels/data.bin", "wb+");
    }

    if(ficsav == NULL)
    {
        save = 1;
        pass = true;
    }

    if(!pass && !ecriture)
    {
        fread(&save, sizeof(int), 1, ficsav);
        save = save / 314;
    }

    if(ecriture)
    {
        tosav = tosav * 314;
        fwrite(&tosav, sizeof(int), 1, ficsav);
    }

    fclose(ficsav);
    return save;
}
