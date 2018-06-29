#include "includes.h"

void editeur(SDL_Renderer *prender, SDL_Texture *blocs[], const bool personnalise)
{
    int i = 0;

    char niveau[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};

    if (!personnalise)
    {
        niveauEdit(niveau);
    }

    bool key[SDL_NUM_SCANCODES] = {false}, aide = false, quit = false, erreur = false;

    SDL_Event ev;

    int objetActu = MUR;
    SDL_Rect souris = {WIDHT/2 - 20, HEIGHT/2 - 20, 40, 40};
    SDL_Texture *paide = NULL, *perreur = NULL, *pcharglvlerr = NULL;

    loadImg(&prender, &paide, "img/Aide_Editeur.png");
    loadImg(&prender, &perreur, "img/Erreur_Editeur.png");
    loadImg(&prender, &pcharglvlerr, "img/Erreur_Chargement_Niveau.png");

    float deltaTemps = 0, delaiTab = 0, delaiErr = 0;
    int tempPrece = 0, tempActu = 0;

    if (personnalise)
    {
        if (chargerNiveauEdition(niveau))
        {
            SDL_RenderCopy(prender, pcharglvlerr, NULL, NULL);
            SDL_RenderPresent(prender);
            SDL_Delay(2000);
            SDL_DestroyTexture(pcharglvlerr);
            return;
        }
    }

    while (!quit && !key[SDL_SCANCODE_ESCAPE] && ev.type != SDL_QUIT)
    {
        tempPrece = tempActu;
        tempActu = SDL_GetTicks();
        deltaTemps = tempActu - tempPrece;

        tableauTouches(key, &ev, true);

        delaiTab += deltaTemps;
        delaiErr += deltaTemps;

        SDL_SetRenderDrawColor(prender, 255, 255 ,255 ,255);
        SDL_RenderClear(prender);

        placement(key, niveau, ev, &objetActu);

        if (ev.type == SDL_MOUSEMOTION)
        {
            souris.x = ev.motion.x - NB_PIXEL_LARGEUR/2;
            souris.y = ev.motion.y - NB_PIXEL_HAUTEUR/2;
        }

        afficheNiveau(niveau, prender, blocs);

        SDL_RenderCopy(prender, blocs[objetActu], NULL, &souris);

        if (key[SDL_SCANCODE_TAB] && delaiTab >= 200.0)
        {
            delaiTab = 0;
            if (!aide)
            {
                aide = true;
            }
            else
            {
                aide = false;
            }
        }

        SDL_SetRenderDrawColor(prender, 0, 0, 255, 255);
        for (i = NB_PIXEL_LARGEUR; i < WIDHT; i += NB_PIXEL_LARGEUR)
        {
            SDL_RenderDrawLine(prender, i, 0, i, HEIGHT);
            SDL_RenderDrawLine(prender, 0, i, WIDHT, i);
        }

        if (key[SDL_SCANCODE_S] && verificationNiveau(niveau))
        {
            quit = true;
            erreur = false;
        }
        else if (key[SDL_SCANCODE_S] && !verificationNiveau(niveau))
        {
            aide = false;
            erreur = true;
        }

        if (erreur == true && delaiErr <= 3000.0)
        {
            SDL_RenderCopy(prender, perreur, NULL, NULL);
        }
        else if ((erreur && delaiErr >= 3000.0) || !erreur)
        {
            delaiErr = 0;
            erreur = false;
        }

        if(key[SDL_SCANCODE_J] && verificationNiveau(niveau))
        {
            key[SDL_SCANCODE_J] = false;
            sauvegarderNiveau(niveau);
            jeu(prender, blocs, true, 0);
        }

        if (aide)
            SDL_RenderCopy(prender, paide, NULL, NULL);

        SDL_RenderPresent(prender);

    }

    if (key[SDL_SCANCODE_S])
        sauvegarderNiveau(niveau);

    cleanNiveau(niveau);

    SDL_DestroyTexture(paide);
    paide = NULL;
    SDL_DestroyTexture(perreur);
    perreur = NULL;

}

void placement(bool key[], char niveau[][NB_BLOCS_HAUTEUR], SDL_Event ev, int *objetActu)
{
    static bool boutonLeft = false, boutonRight = false;

    int i = 0, j = 0;

    switch (ev.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        if (ev.button.button == SDL_BUTTON_LEFT)
        {
            if (*objetActu == SPAWN)
            {
                for (i = 0; i < NB_BLOCS_LARGEUR; i++)
                {
                    for (j = 0; j < NB_BLOCS_HAUTEUR; j++)
                    {
                        if (niveau[i][j] == SPAWN)
                            niveau[i][j] = VIDE;
                    }
                }
            }

            niveau[ev.button.x / NB_PIXEL_LARGEUR][ev.button.y / NB_PIXEL_HAUTEUR] = *objetActu;
            boutonLeft = true;
        }
        else if (ev.button.button == SDL_BUTTON_RIGHT)
        {
            niveau[ev.button.x / NB_PIXEL_LARGEUR][ev.button.y / NB_PIXEL_HAUTEUR] = VIDE;
            boutonRight = true;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (ev.button.button == SDL_BUTTON_LEFT)
            boutonLeft = false;
        else if (ev.button.button == SDL_BUTTON_RIGHT)
            boutonRight = false;
        break;

    case SDL_MOUSEMOTION:
        if(boutonLeft)
        {
            if (*objetActu == SPAWN)
            {
                for (i = 0; i < NB_BLOCS_LARGEUR; i++)
                {
                    for (j = 0; j < NB_BLOCS_HAUTEUR; j++)
                    {
                        if (niveau[i][j] == SPAWN)
                            niveau[i][j] = VIDE;
                    }
                }
            }

            niveau[ev.motion.x/NB_PIXEL_LARGEUR][ev.motion.y/NB_PIXEL_HAUTEUR] = *objetActu;
        }
        else if(boutonRight)
        {
            niveau[ev.motion.x/NB_PIXEL_LARGEUR][ev.motion.y/NB_PIXEL_HAUTEUR] = VIDE;
        }
        break;

    case SDL_KEYDOWN:
        switch(ev.key.keysym.scancode)
        {
        case SDL_SCANCODE_KP_0:
        case SDL_SCANCODE_0:
            *objetActu = VIDE;
            break;
        case SDL_SCANCODE_KP_1:
        case SDL_SCANCODE_1:
            *objetActu = MUR;
            break;
        case SDL_SCANCODE_KP_2:
        case SDL_SCANCODE_2:
            *objetActu = OBJECTIF;
            break;
        case SDL_SCANCODE_KP_3:
        case SDL_SCANCODE_3:
            *objetActu = SPAWN;
            break;
        case SDL_SCANCODE_KP_4:
        case SDL_SCANCODE_4:
            *objetActu = STOPPER;
            break;
        case SDL_SCANCODE_KP_5:
        case SDL_SCANCODE_5:
            switch(*objetActu)
            {
            case REHAUT:
               *objetActu = REDROITE;
               break;
            case REDROITE:
                *objetActu = REBAS;
                break;
            case REBAS:
                *objetActu = REGAUCHE;
                break;
            case REGAUCHE:
                *objetActu = REHAUT;
                break;
            default:
                *objetActu = REHAUT;
                break;
            }
            break;
        case SDL_SCANCODE_KP_6:
        case SDL_SCANCODE_6:
            switch(*objetActu)
            {
            case MURINV:
                *objetActu = MUROPA;
                break;
            case MUROPA:
                *objetActu = MURINV;
                break;
            default:
                *objetActu = MURINV;
                break;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

}

void sauvegarderNiveau(char niveau[][NB_BLOCS_HAUTEUR])
{
    FILE *creation = NULL;
    creation = fopen("levels/custom_level.lvl", "w+");
    int i = 0, j = 0;

    for (j = 0; j < NB_BLOCS_LARGEUR; j++)
    {
        for (i = 0; i < NB_BLOCS_HAUTEUR-1; i++)
        {
            if(niveau[i][j] != 10)
                fprintf(creation, "%d  ", niveau[i][j]);

            if(niveau[i][j] == 10)
                fprintf(creation, "%c  ", '&');
        }
        if(niveau[i][j] != 10)
            fprintf(creation, "%d\n", niveau[i][j]);
        if(niveau[i][j] == 10)
            fprintf(creation, "%c\n", '&');
    }

    fclose(creation);
}

bool verificationNiveau(char niveau[][NB_BLOCS_HAUTEUR])
{
    int i = 0, j = 0;
    bool ok = false, okObj = false;

    for (i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        for (j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
            if (niveau[i][j] == SPAWN)
            {
                ok = true;
            }
            if (niveau[i][j] == OBJECTIF)
            {
                okObj = true;
            }
            if (okObj && ok)
            {
                i = NB_BLOCS_LARGEUR;
                j = NB_BLOCS_HAUTEUR;
            }
        }
    }

    if (ok && !okObj)
        ok = false;

    return ok;
}

void niveauEdit(char niveau[][NB_BLOCS_HAUTEUR])
{
    int i = 0, j = 0;

    for (i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        niveau[i][0] = MUR;
        niveau[i][NB_BLOCS_HAUTEUR-1] = MUR;
        for (j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
            niveau[0][j] = MUR;
            niveau[NB_BLOCS_LARGEUR-1][j] = MUR;
        }
    }
}

void cleanNiveau(char niveau[][NB_BLOCS_HAUTEUR])
{
    int i = 0, j = 0;

    for (i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        for (j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
           niveau[i][j] = VIDE;
        }
    }
}
