#include "includes.h"

bool tutoriel(SDL_Renderer *prender, rectclick *menutu)
{
    SDL_Texture *ptuto[NB_PAGES_TUTO] = {NULL};

    loadImg(&prender, &ptuto[0], "img/Tuto_1.png");
    loadImg(&prender, &ptuto[1], "img/Tuto_2.png");
    loadImg(&prender, &ptuto[2], "img/Tuto_3.png");

    bool quit = false, key[SDL_NUM_SCANCODES] = {false};
    unsigned int page = 0;
    SDL_Event ev;

    while (!quit)
    {
        tableauTouches(key, &ev, true);

        if(ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT)
        {
            if (ev.button.x >= menutu[0].x && ev.button.x <= (menutu[0].x + menutu[0].xl) && ev.button.y >= menutu[0].y && ev.button.y <= (menutu[0].y + menutu[0].yl) && page < NB_PAGES_TUTO-1)
            {
                page ++;
            }

            if (ev.button.x >= menutu[1].x && ev.button.x <= (menutu[1].x + menutu[1].xl) && ev.button.y >= menutu[1].y && ev.button.y <= (menutu[1].y + menutu[1].yl) && page > 0)
            {
                page --;
            }
            ev.type = SDL_AUCUN;
        }

        if(ev.type == SDL_KEYDOWN)
        {
            if (key[SDL_SCANCODE_RIGHT] && page < NB_PAGES_TUTO-1)
                page ++;
            if (key[SDL_SCANCODE_LEFT] && page > 0)
                page --;
            SDL_Delay(300);
        }


        if (key[SDL_SCANCODE_ESCAPE] || ev.type == SDL_QUIT)
            quit = true;

        SDL_RenderCopy(prender, ptuto[page], NULL, NULL);
        SDL_RenderPresent(prender);
    }

    bool croix = false;
    if (ev.type == SDL_QUIT)
        croix = true;

    SDL_DestroyTexture(ptuto[0]);
    ptuto[0] = NULL;
    SDL_DestroyTexture(ptuto[1]);
    ptuto[1] = NULL;

    return croix;
}
