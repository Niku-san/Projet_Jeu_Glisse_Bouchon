#include "includes.h"

void jeu(SDL_Renderer *prender, SDL_Texture *blocs[], const bool personnalise, int nbLevels) //Gère la partie jeu
{

    char number[4];  //tableau qui va contenir le numéro du niveau actuel
    int z = 0;  //z est utilisé pour la boucle for suivante
    //int nbLevels = NB_LEVELS;
    int fixed = 1, depcase = 0;

    if (personnalise)
        nbLevels = 1;

    unsigned int save = 1;

    if(!personnalise)
        save = avancement(false, 0);

    for (z = 1; z <= nbLevels; z++)  //boucle for qui fait défiler les niveaux lorsqu'un est terminé
    {

        sprintf(number, "%d", z);  //permet de stocker un int (z) en char
        char niveau[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};  //tableau du niveau actuel

        SDL_Rect personnage = {0, 0, NB_PIXEL_LARGEUR-10, NB_PIXEL_HAUTEUR-10}; //définission du joueur
        bool jouer = true; //booléen pour la boucle while

        level levels = {0, 0};    //variable contenant les paramètres du niveau (nb obj, nb objcapturé)
        SDL_Texture *pcharglvlerr = NULL;


        float deltaTemps = 0.0, moveTime = 0.0; // variable gérant
        int tempActu = 0, tempPrecedent = 0;    // le temps

        if (chargerNiveau(niveau, &personnage, number, &levels, personnalise) == true) //charge le niveau est vérifie si ça a échoué
        {
            jouer = false;
            z = nbLevels + 1;
            loadImg(&prender, &pcharglvlerr, "img/Erreur_Chargement_Niveau.png");
            SDL_RenderCopy(prender, pcharglvlerr, NULL, NULL);
            SDL_RenderPresent(prender);
            SDL_Delay(2000);
        }

        bool key[SDL_NUM_SCANCODES] = {false};   //tableau contenant les touches
        int direction = FIXE;       //Initialisation position départ

        SDL_Event ev;   //variable event

        while (jouer) //boucle du jeu
        {
            tempPrecedent = tempActu;  // On gère le temps dans le jeu
            tempActu = SDL_GetTicks();  //
            deltaTemps = (tempActu - tempPrecedent)/1000.0f;  //
            moveTime += deltaTemps; //

            tableauTouches(key, &ev, false); //on remplit le tableau de touche

            if (ev.type == SDL_KEYDOWN)
                key[ev.key.keysym.scancode] = true;

            if (ev.type == SDL_KEYUP)
                key[ev.key.keysym.scancode] = false;

            if (ev.type == SDL_QUIT || key[SDL_SCANCODE_ESCAPE] || key[SDL_SCANCODE_R]) //vérification si la personne veut quitter le jeu
                jouer = false;

            if(key[SDL_SCANCODE_TAB] && z != save && save > 1)
                jouer = false;

            if(key[SDL_SCANCODE_PAGEUP] ==  true && z < save && z < nbLevels) //niveau suivant
            {
                jouer = false;
            }
            if(key[SDL_SCANCODE_PAGEDOWN] == true && z > 1) //niveau précédent
            {
                z-= 2;
                jouer = false;
            }

            if (moveTime >= 1.0/MOVESPEED) //permet un déplacement fluide en pixels par seconde
            {
                moveTime = 0;
                depcase = touches(&personnage, key, &direction, niveau, &levels, &fixed); //on fait des actions en fonction des touches appuyées
            }

            if(fixed >= 1 && depcase >= 1)
            {
                fixed = 0;
                changeWall(niveau);
            }

            if (levels.nbObjCapt == levels.nbObj) // vérification si le joueur à gagner
            {
                z++;
                jouer = false;
            }

            SDL_SetRenderDrawColor(prender, 255, 255, 255, 255); // reset
            SDL_RenderClear(prender); // de l'écran

            afficheNiveau(niveau, prender, blocs); //affichage du niveau
            SDL_SetRenderDrawColor(prender, 255, 0, 0, 255); // affichage du
            SDL_RenderFillRect(prender, &personnage); // personnage
            SDL_RenderPresent(prender); // actualisation

            if (direction == STOP)
            {
                direction = FIXE;
                SDL_Delay(200);
            }

        }

        if(key[SDL_SCANCODE_TAB])
        {
            key[SDL_SCANCODE_TAB] = false;
            z = save-1;
        }

        if(z >= save && !personnalise && z <= nbLevels)
        {
            save = z;
            avancement(true, save);
        }

        if(levels.nbObjCapt == levels.nbObj)
        {
            z--;
        }

        SDL_Delay(500);
        if (key[SDL_SCANCODE_ESCAPE] == true || ev.type == SDL_QUIT) // vérification si le joueur avait décider de quitter le jeu
            z = nbLevels + 1;
        if (key[SDL_SCANCODE_R])
            z--;

        SDL_DestroyTexture(pcharglvlerr);
    }

}


int touches (SDL_Rect *personnage, const bool key[], int *direction, char niveau[][NB_BLOCS_HAUTEUR], level *levelActu, int *fixed)
{
    int depcase = 0;

    if (((key[SDL_SCANCODE_UP] || key[SDL_SCANCODE_W]) && *direction == FIXE) || *direction == HAUT)
    {
        if (personnage->y/40 == 0)
        {
            if(*direction != FIXE)// && niveau[personnage->x/40][personnage->y/40] != MURINV)
                (*fixed)++;

            *direction = FIXE;
        }
        else if (niveau[personnage->x/40][(personnage->y)/40 - 1] == 0 || niveau[personnage->x/40][(personnage->y-5)/40 - 1] == 2 || niveau[personnage->x/40][(personnage->y-5)/40 - 1] == 3) // vérifie si c'est une case ou le joueur peut aller
        {
            personnage->y -= NB_PIXEL_HAUTEUR;
            *direction = HAUT; //définition de la direction du joueur à la direction approprié
        }
        else if(niveau[personnage->x/40][(personnage->y-5)/40 - 1] == MURINV)
        {
            personnage->y -= NB_PIXEL_HAUTEUR;
            *direction = HAUT;
        }
        else if (niveau[personnage->x/40][(personnage->y)/40 - 1] >= 5 && niveau[personnage->x/40][(personnage->y)/40 - 1] <= 8)
        {
            personnage->y -= NB_PIXEL_HAUTEUR;
            switch(niveau[personnage->x/40][(personnage->y)/40])
            {
            case REHAUT:
                *direction = HAUT;
                break;
            case REDROITE:
                *direction = DROITE;
                break;
            case REBAS:
                *direction = BAS;
                break;
            case REGAUCHE:
                *direction = GAUCHE;
                break;
            default:
                break;
            }
        }
        /*else if(niveau[personnage->x/40][(personnage->y-5)/40 - 1] == MUROPA&& *direction != FIXE)
        {
            personnage->y -= NB_PIXEL_HAUTEUR;
            *direction = FIXE;
            fixed++;
        }*/
        else if (niveau[personnage->x/40][(personnage->y)/40 - 1] == 1 || niveau[personnage->x/40][(personnage->y)/40 - 1] == MUROPA) //vérifie si le personnage rencontre un mur
        {
            if(*direction != FIXE)// && niveau[personnage->x/40][personnage->y/40] != MURINV)
            {
                (*fixed)++;
            }

            if(niveau[personnage->x/40][(personnage->y)/40 - 1] == MUROPA)
            {
                *direction = STOP; //définition du joueur en FIXE
            }
            else
                *direction = FIXE;

        }
        else if (niveau[personnage->x/40][(personnage->y)/40 - 1] == 4)
        {
            personnage->y -= NB_PIXEL_HAUTEUR;
            *direction = STOP;
            (*fixed)++;
        }


    }

    else if (((key[SDL_SCANCODE_DOWN] || key[SDL_SCANCODE_S]) && *direction == FIXE) || *direction == BAS)
    {
        if (personnage->y/40+1 == NB_BLOCS_HAUTEUR)
        {
            if(*direction != FIXE)// && niveau[personnage->x/40][personnage->y/40] != MURINV)
                (*fixed)++;

            *direction = FIXE;
        }
        else if (niveau[personnage->x/40][(personnage->y)/40 + 1] == 0 || niveau[personnage->x/40][(personnage->y+5)/40 + 1] == 2 || niveau[personnage->x/40][(personnage->y+5)/40 + 1] == 3)
        {
            personnage->y += NB_PIXEL_HAUTEUR;
            *direction = BAS;
        }
        else if(niveau[personnage->x/40][(personnage->y+5)/40 + 1] == MURINV)
        {
            personnage->y += NB_PIXEL_HAUTEUR;
            *direction = BAS;
        }
        else if (niveau[personnage->x/40][(personnage->y)/40 + 1] >= 5 && niveau[personnage->x/40][(personnage->y)/40 + 1] <= 8)
        {
            personnage->y += NB_PIXEL_HAUTEUR;
            switch(niveau[personnage->x/40][(personnage->y)/40])
            {
            case REHAUT:
                *direction = HAUT;
                break;
            case REDROITE:
                *direction = DROITE;
                break;
            case REBAS:
                *direction = BAS;
                break;
            case REGAUCHE:
                *direction = GAUCHE;
                break;
            default:
                break;
            }
        }
        /*else if(niveau[personnage->x/40][(personnage->y+5)/40 + 1] == MUROPA&& *direction != FIXE)
        {
            personnage->y += NB_PIXEL_HAUTEUR;
            *direction = FIXE;
            fixed++;
        }*/
        else if (niveau[personnage->x/40][(personnage->y)/40 + 1] == 1 || niveau[personnage->x/40][(personnage->y)/40 + 1] == MUROPA)
        {
            if(*direction != FIXE)// && niveau[personnage->x/40][personnage->y/40] != MURINV)
            {
                (*fixed)++;
            }
            if(niveau[personnage->x/40][(personnage->y)/40 + 1] == MUROPA)
                *direction = STOP;
            else
                *direction = FIXE;
        }
        else if (niveau[personnage->x/40][(personnage->y)/40 + 1] == 4)
        {
            personnage->y += NB_PIXEL_HAUTEUR;
            *direction = STOP;
            (*fixed)++;
        }


    }

    else if (( (key[SDL_SCANCODE_RIGHT] || key[SDL_SCANCODE_D])&& *direction == FIXE) || *direction == DROITE)
    {
        if (personnage->x/40+1 == NB_BLOCS_LARGEUR)
        {
            if(*direction != FIXE)// && niveau[personnage->x/40][personnage->y/40] != MURINV)
                (*fixed)++;

            *direction = FIXE;
        }
        else if (niveau[(personnage->x)/40 + 1][(personnage->y)/40] == 0 || niveau[(personnage->x+5)/40 + 1][(personnage->y)/40] == 2 ||niveau[(personnage->x+5)/40 + 1][(personnage->y)/40] == 3)
        {
            personnage->x += NB_PIXEL_LARGEUR;
            *direction = DROITE;
        }
        else if(niveau[(personnage->x+5)/40 + 1][(personnage->y)/40] == MURINV)
        {
            personnage->x += NB_PIXEL_LARGEUR;
            *direction = DROITE;
        }
        else if (niveau[personnage->x/40 + 1][(personnage->y)/40] >= 5 && niveau[personnage->x/40 + 1][(personnage->y)/40] <= 8)
        {
            personnage->x += NB_PIXEL_HAUTEUR;
            switch(niveau[personnage->x/40][(personnage->y)/40])
            {
            case REHAUT:
                *direction = HAUT;
                break;
            case REDROITE:
                *direction = DROITE;
                break;
            case REBAS:
                *direction = BAS;
                break;
            case REGAUCHE:
                *direction = GAUCHE;
                break;
            default:
                break;
            }
        }
        /*else if(niveau[(personnage->x+5)/40 + 1][(personnage->y)/40] == MUROPA&& *direction != FIXE)
        {
            personnage->x += NB_PIXEL_LARGEUR;
            *direction = FIXE;
            fixed++;
        }*/
        else if (niveau[(personnage->x)/40 + 1][(personnage->y)/40] == 1 || niveau[(personnage->x)/40 + 1][(personnage->y)/40] == MUROPA)
        {
            if(*direction != FIXE)// && niveau[personnage->x/40][personnage->y/40] != MURINV)
            {
                (*fixed)++;
            }
            if(niveau[(personnage->x)/40 + 1][(personnage->y)/40] == MUROPA)
                *direction = STOP;
            else
                *direction = FIXE;
        }
        else if (niveau[(personnage->x)/40 + 1][(personnage->y)/40] == 4)
        {
            personnage->x += NB_PIXEL_LARGEUR;
            *direction = STOP;
            (*fixed)++;
        }


    }

    else if (((key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_A]) && *direction == FIXE) || *direction == GAUCHE)
    {
        if (personnage->x/40 == 0)
        {
            if(*direction != FIXE)// && niveau[personnage->x/40][personnage->y/40] != MURINV)
                (*fixed)++;

            *direction = FIXE;
        }
        else if (niveau[(personnage->x)/40 - 1][(personnage->y)/40] == 0 || niveau[(personnage->x-5)/40 - 1][(personnage->y)/40] == 2 || niveau[(personnage->x-5)/40 - 1][(personnage->y)/40] == 3)
        {
            personnage->x -= NB_PIXEL_LARGEUR;
            *direction = GAUCHE;
        }
        else if(niveau[(personnage->x-5)/40 - 1][(personnage->y)/40] == MURINV)
        {
            personnage->x -= NB_PIXEL_LARGEUR;
            *direction = GAUCHE;
        }
        else if (niveau[personnage->x/40 - 1][(personnage->y)/40] >= 5 && niveau[personnage->x/40 - 1][(personnage->y)/40] <= 8)
        {
            personnage->x -= NB_PIXEL_HAUTEUR;
            switch(niveau[personnage->x/40][(personnage->y)/40 ])
            {
            case REHAUT:
                *direction = HAUT;
                break;
            case REDROITE:
                *direction = DROITE;
                break;
            case REBAS:
                *direction = BAS;
                break;
            case REGAUCHE:
                *direction = GAUCHE;
                break;
            default:
                break;
            }
        }
        /*else if(niveau[(personnage->x-5)/40 - 1][(personnage->y)/40] == MUROPA&& *direction != FIXE)
        {
            personnage->x -= NB_PIXEL_LARGEUR;
            *direction = FIXE;
            fixed++;
        }*/
        else if (niveau[(personnage->x)/40 - 1][(personnage->y)/40] == 1 || niveau[(personnage->x)/40 - 1][(personnage->y)/40] == MUROPA)
        {
            if(*direction != FIXE)// && niveau[personnage->x/40][personnage->y/40] != MURINV)
            {
                (*fixed)++;
            }
            if(niveau[(personnage->x)/40 - 1][(personnage->y)/40] == MUROPA)
                *direction = STOP;
            else
                *direction = FIXE;
        }
        else if (niveau[(personnage->x)/40 - 1][(personnage->y)/40] == 4)
        {
            personnage->x -= NB_PIXEL_LARGEUR;
            *direction = STOP;
            (*fixed)++;
        }


    }

    if(*direction != STOP && *direction != FIXE)
        depcase++;

    if (niveau[(personnage->x)/40][(personnage->y)/40] == 2)
    {
        levelActu->nbObjCapt ++;
        niveau[(personnage->x)/40][(personnage->y)/40] = 0;
    }

    return depcase;
}

void tableauTouches (bool key[], SDL_Event *ev, const bool key_edit)
{
    SDL_PollEvent(ev);

    if (key_edit == true)
    {
        if (ev->type == SDL_KEYDOWN)
            key[ev->key.keysym.scancode] = true;

        if (ev->type == SDL_KEYUP)
            key[ev->key.keysym.scancode] = false;
    }
}

void changeWall(char niveau[][NB_BLOCS_HAUTEUR])
{
    int i = 0, j = 0;

    for(i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        for(j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
            switch(niveau[i][j])
            {
            case MURINV:
                niveau[i][j] = MUROPA;
                break;
            case MUROPA:
                niveau[i][j] = MURINV;
                break;
            default:
                break;
            }
        }
    }
}
