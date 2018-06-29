/*
    fichier main.c
----------

Par Baptiste CHASSAIS
Contient le menu du jeu

Création le 26/03/2018 14:25
Dernière Modification le 20/04/2018 10:38

*/

#include "includes.h"

int menu(SDL_Renderer *prender, SDL_Texture *pmenu, const int menuActu, rectclick menup[], rectclick menuj[], rectclick menued[]);
void pause(SDL_Event *ev, bool key[], bool *click);
int clicked(int menuActu, rectclick menup[], rectclick menuj[], rectclick menued[], SDL_Event ev);
bool boutonsInfos(rectclick **menup, rectclick **menuj, rectclick **menued, rectclick **menutu, int *nbLevels);
void valeursBoutons(FILE *menus, int imax, rectclick *menul);


int main (int argc, char *argv[])
{
    bool error = false;
    SDL_Window *pwindow = NULL;
    SDL_Renderer *prender = NULL;
    SDL_Texture *pmenu = NULL, *pmenu2 = NULL, *pmenu3 = NULL;

    error = initSDL(&pwindow, &prender, "Glisse-Bouchon"); //initialisation SDL

    bool quitMenu = false;
    SDL_Texture *blocs[OBJETMAX] = {NULL};

    loadImg(&prender, &pmenu, "img/menu.png");
    loadImg(&prender, &pmenu2, "img/menu2.png");
    loadImg(&prender, &pmenu3, "img/menu_edition.png");
    chargerBlocs(prender, blocs);
    bool personnalise = false;
    int nbLevels = 1;

    rectclick *menup = NULL, *menuj = NULL, *menued = NULL, *menutu = NULL;
    error = boutonsInfos(&menup, &menuj, &menued, &menutu, &nbLevels);


    while (!quitMenu && !error)
    {
        switch (menu(prender, pmenu, MPRINCIPAL, menup, menuj, menued))
        {
        case JOUER:

            switch (menu(prender, pmenu2, MJOUER, menup, menuj, menued))
            {
            case NORMAL:
                personnalise = false;
                jeu(prender, blocs, personnalise, nbLevels); //lance le jeu avec les niveaux normaux
                break;
            case PERSONNALISE:
                personnalise = true;
                jeu(prender, blocs, personnalise, nbLevels);  //lance le jeu avec le niveau de l'editeur
                break;
            case QUITTER:
                quitMenu = true;
                SDL_Delay(20);
                break;
            case ESCAPE:
                quitMenu = false;
                SDL_Delay(20);
            default:
                break;
            }

            break;
        case EDITEUR:
            switch (menu(prender, pmenu3, MEDITEUR, menup, menuj, menued))
            {
            case NORMAL:
                personnalise = false;
                editeur(prender, blocs, personnalise);
                break;
            case PERSONNALISE:
                personnalise = true;
                editeur(prender, blocs, personnalise);
                break;
            case QUITTER:
                quitMenu = true;
                SDL_Delay(20);
                break;
            case ESCAPE:
                quitMenu = false;
                SDL_Delay(20);
            default:
                break;
            }
            break;
        case TUTO:
            if (tutoriel(prender, menutu) == true)
                quitMenu = true;
            else
                quitMenu = false;
            break;
        case RIEN:
            break;
        case QUITTER:
        case ESCAPE:
            quitMenu = true;
            break;
        default:
            break;
        }
    }

    //Quitte la SDL et détruit la fenêtre et le Renderer
    int i = 0;
    for (i = 0; i < OBJETMAX; i++)
    {
        SDL_DestroyTexture(blocs[i]);
        blocs[i] = NULL;
    }

    SDL_DestroyTexture(pmenu);
    pmenu = NULL;
    SDL_DestroyTexture(pmenu2);
    pmenu2 = NULL;
    SDL_DestroyTexture(pmenu3);
    pmenu3 = NULL;
    SDL_DestroyRenderer(prender);
    prender = NULL;
    SDL_DestroyWindow(pwindow);
    pwindow = NULL;

    SDL_Quit();

    free(menup);
    free(menuj);
    free(menued);

    return error;
}

int menu(SDL_Renderer *prender, SDL_Texture *pmenu, const int menuActu, rectclick menup[], rectclick menuj[], rectclick menued[])
{
    int choix = 0;
    bool click = false;
    SDL_Event ev;
    bool key[SDL_NUM_SCANCODES] = {false};

    SDL_RenderCopy(prender, pmenu, NULL, NULL);
    SDL_RenderPresent(prender);

    pause(&ev, key, &click);

    if (key[SDL_SCANCODE_1])
        choix = JOUER;
    else if (key[SDL_SCANCODE_2])
        choix = EDITEUR;
    else if (key[SDL_SCANCODE_3])
        choix = TUTO;
    else if (key[SDL_SCANCODE_ESCAPE])
        choix = ESCAPE;
    else if (ev.type == SDL_QUIT)
        choix = QUITTER;
    else
        choix = RIEN;

    if(click == true && ev.button.button == SDL_BUTTON_LEFT)
    {
        click = false;
        choix = clicked(menuActu, menup, menuj, menued, ev);
    }


    return choix;
}

void pause(SDL_Event *ev, bool key[], bool *click)
{
    bool stop = false;
    int i = 0;


    while (ev->type != SDL_QUIT && stop == false)
    {
        tableauTouches(key, ev, true);

        if(ev->type == SDL_MOUSEBUTTONUP)
        {
            stop = true;
            *click = true;
            ev->type = SDL_AUCUN;
        }

        for (i = 0; i < SDL_NUM_SCANCODES && stop == false; i++)
        {
           if (key[i] == true)
           {
               i = SDL_NUM_SCANCODES+1;
               stop = true;
           }
        }
    }
}

int clicked(int menuActu, rectclick menup[], rectclick menuj[], rectclick menued[], SDL_Event ev)
{
    int choix = RIEN;

    switch(menuActu)
    {
    case MPRINCIPAL:
        if (ev.button.x >= menup[0].x && ev.button.x <= (menup[0].x + menup[0].xl) && ev.button.y >= menup[0].y && ev.button.y <= (menup[0].y + menup[0].yl))
        {
            choix = JOUER;
        }

        if (ev.button.x >= menup[1].x && ev.button.x <= (menup[1].x + menup[1].xl) && ev.button.y >= menup[1].y && ev.button.y <= (menup[1].y + menup[1].yl))
        {
            choix = EDITEUR;
        }

        if (ev.button.x >= menup[2].x && ev.button.x <= (menup[2].x + menup[2].xl) && ev.button.y >= menup[2].y && ev.button.y <= (menup[2].y + menup[2].yl))
        {
            choix = TUTO;
        }

        break;
    case MJOUER:
        if (ev.button.x >= menuj[0].x && ev.button.x <= (menuj[0].x + menuj[0].xl) && ev.button.y >= menuj[0].y && ev.button.y <= (menuj[0].y + menuj[0].yl))
        {
            choix = NORMAL;
        }

        if (ev.button.x >= menuj[1].x && ev.button.x <= (menuj[1].x + menuj[1].xl) && ev.button.y >= menuj[1].y && ev.button.y <= (menuj[1].y + menuj[1].yl))
        {
            choix = PERSONNALISE;
        }

        break;
    case MEDITEUR:
        if (ev.button.x >= menued[0].x && ev.button.x <= (menued[0].x + menued[0].xl) && ev.button.y >= menued[0].y && ev.button.y <= (menued[0].y + menued[0].yl))
        {
            choix = NORMAL;
        }

        if (ev.button.x >= menued[1].x && ev.button.x <= (menued[1].x + menued[1].xl) && ev.button.y >= menued[1].y && ev.button.y <= (menued[1].y + menued[1].yl))
        {
            choix = PERSONNALISE;
        }

        break;
    default:
        break;
    }


    return choix;
}

bool boutonsInfos(rectclick **menup, rectclick **menuj, rectclick **menued, rectclick **menutu, int *nbLevels)// fonction qui récupère dans un txt les infos sur les positions des boutons du menu du jeu
{
    bool error = 0;// booleen pour les erreurs
    int imax = 0, taillePart = 16;//taillePart = taille mot à rechercher dans le txt
    char mottmp[20];

    FILE *menus = NULL;
    menus = fopen("menu.txt", "r");
    if(menus == NULL)
    {
        error = true;
        return error;
    }

    fscanf(menus, "%d", nbLevels);

    while (strcmp("MENUPRINCIPAL :", mottmp) != 0)//cherche la phrase "MENUPRINCIPAL :" dans le txt
    {
        if (fgets(mottmp, taillePart, menus) == NULL)
            rewind(menus);  //remet le curseur au début du txt si le phrase n'est pas trouvé

    }

    fscanf(menus, "%d\n", &imax);//rentre dans imax le nombre de coordonnées à récupérer


    (*menup) = malloc(imax * sizeof(rectclick));

    if(menup == NULL)
    {
        error = true;
        return error;
    }

    valeursBoutons(menus, imax, *menup);

    taillePart = 12;
    while (strcmp("MENUJOUER :", mottmp) != 0)
    {
        if (fgets(mottmp, taillePart, menus) == NULL)
            rewind(menus);
    }


    fscanf(menus, "%d", &imax);
    (*menuj) = malloc(imax * sizeof(rectclick));

    if(*menuj == NULL)
    {
        error = true;
        return error;
    }

    valeursBoutons(menus, imax, *menuj);

    taillePart = 14;
    while (strcmp("MENUEDITEUR :", mottmp) != 0)
    {
        if (fgets(mottmp, taillePart, menus) == NULL)
            rewind(menus);
    }

    fscanf(menus, "%d", &imax);
    (*menued) = malloc(imax * sizeof(rectclick));

    if(*menued == NULL)
    {
        error = true;
        return error;
    }

    valeursBoutons(menus, imax, *menued);

    taillePart = 11;
    while (strcmp("MENUTUTO :", mottmp) != 0)
    {
        if (fgets(mottmp, taillePart, menus) == NULL)
            rewind(menus);
    }

    fscanf(menus, "%d", &imax);
    (*menutu) = malloc(imax * sizeof(rectclick));

    if(*menutu == NULL)
    {
        error = true;
        return error;
    }

    valeursBoutons(menus, imax, *menutu);

    fclose(menus);
    return error;
}

void valeursBoutons(FILE *menus, int imax, rectclick *menul)
{
    fseek(menus, 0, SEEK_CUR);
    for (int i = 0; i < imax; i++) //rentre dans le tableau de position des points pour les boutons
    {
        fscanf(menus, "%d;%d;%d;%d", &menul[i].x, &menul[i].y, &menul[i].xl, &menul[i].yl);
        //printf("%d %d %d %d\n", menul[i].x, menul[i].y, menul[i].xl, menul[i].yl);
    }
}
