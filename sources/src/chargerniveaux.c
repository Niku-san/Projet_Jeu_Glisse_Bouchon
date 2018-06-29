#include "includes.h"

bool chargerNiveau(char niveau[][NB_BLOCS_HAUTEUR], SDL_Rect *dst, char levelnb[], level *levels, const bool personnalise)
{
    FILE *fichier = NULL;
    char adresse[25];
    char extension[5];

    if (!personnalise)
    {
        sprintf(adresse, "levels/level_"); //nom du fichier dans lesquels se trouvent les fichiers levels
        sprintf(extension, ".lvl");
        strcat(adresse, levelnb); //on associe le numéro du niveau a l'adresse
        strcat(adresse, extension); //on ajoute l'extension
    }
    else
    {
         sprintf(adresse, "levels/custom_level.lvl");
    }

    fichier = fopen(adresse, "r"); // on ouvre le fichier en lecture seul

    if (fichier == NULL) // vérification de l'ouverture du fichier
    {
        //printf("----   FICHIER LVL INTROUVABLE !!!\n");
        fclose(fichier);
        return true;
    }
    levels->nbObj = 0;

    char ligne[NB_BLOCS_LARGEUR*3] = {0}; //tableau temporaire stockant les numéro du fichier .lvl

    int i = 0;
    for (i = 0; i < NB_BLOCS_HAUTEUR; i++) //scan le fichier
    {
        fgets(ligne, NB_BLOCS_LARGEUR*3, fichier); //récupère la ligne du fichier
        for (int j = 0; j < NB_BLOCS_LARGEUR; j++)
        {
            switch (ligne[j*3]) //insère les bons numéro dans le tableau du niveau
            {
            case '0': // espace vide/chemin
                niveau[j][i] = 0;
                break;
            case '1': //mur
                niveau[j][i] = 1;
                break;
            case '2': //objectif
                niveau[j][i] = 2;
                levels->nbObj ++;
                break;
            case '3': //endroit ou le joueur aparait
                dst->x = j*NB_PIXEL_LARGEUR+5;
                dst->y = i*NB_PIXEL_HAUTEUR+5;
                niveau[j][i] = 3;
                break;
            case '4': //stopper
                niveau[j][i] = 4;
                break;
            case '5': //Redirection Haut
                niveau[j][i] = 5;
                break;
            case '6': //Redirection Droite
                niveau[j][i] = 6;
                break;
            case '7': //Redirection Bas
                niveau[j][i] = 7;
                break;
            case '8': //Redirection Gauche
                niveau[j][i] = 8;
                break;
            case '9': //mur actif
                niveau[j][i] = 9;
                break;
            case '&': //mur non actif
                niveau[j][i] = 10;
                break;
            default:
                break;
            }
        }
    }

    fclose(fichier); //fermeture du fichier
    return false;
}

void afficheNiveau(const char niveau[][NB_BLOCS_HAUTEUR], SDL_Renderer *prender, SDL_Texture *blocs[]) //affiche le niveau
{
    int i = 0, j = 0;
    SDL_Rect dst = {0, 0, NB_PIXEL_LARGEUR, NB_PIXEL_HAUTEUR};

    for (i = 0; i < NB_BLOCS_LARGEUR; i++) // scan et affiche les blocs correspondants au bonne endroit
    {

        for (j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
            dst.x = i*NB_PIXEL_LARGEUR;
            dst.y = j*NB_PIXEL_HAUTEUR;

            switch (niveau[i][j])
            {
            case 0:
                break;
            case 1:
                SDL_RenderCopy(prender, blocs[MUR], NULL, &dst);
                break;
            case 2:
                SDL_RenderCopy(prender, blocs[OBJECTIF], NULL, &dst);
                break;
            case 3:
                SDL_RenderCopy(prender, blocs[SPAWN], NULL, &dst);
                break;
            case 4:
                SDL_RenderCopy(prender, blocs[STOPPER], NULL, &dst);
                break;
            case 5:
                SDL_RenderCopy(prender, blocs[REHAUT], NULL, &dst);
                break;
            case 6:
                SDL_RenderCopy(prender, blocs[REDROITE], NULL, &dst);
                break;
            case 7:
                SDL_RenderCopy(prender, blocs[REBAS], NULL, &dst);
                break;
            case 8:
                SDL_RenderCopy(prender, blocs[REGAUCHE], NULL, &dst);
                break;
            case 9:
                SDL_RenderCopy(prender, blocs[MURINV], NULL, &dst);
                break;
            case 10:
                SDL_RenderCopy(prender, blocs[MUROPA], NULL, &dst);
                break;
            default:
                SDL_RenderCopy(prender, blocs[MUR], NULL, &dst);
                break;
            }

        }

    }


}

bool chargerNiveauEdition(char niveau[][NB_BLOCS_HAUTEUR])
{
    FILE *fichier = NULL;
    char adresse[25];

    sprintf(adresse, "levels/custom_level.lvl");

    fichier = fopen(adresse, "r+"); // on ouvre le fichier en lecture seul

    if (fichier == NULL) // vérification de l'ouverture du fichier
    {
        //printf("----  FICHIER LVL INTROUVABLE !!\n");
        fclose(fichier);
        return true;
    }

    char ligne[NB_BLOCS_LARGEUR*3] = {0}; //tableau temporaire stockant les numéro du fichier .lvl

    int i = 0;
    for (i = 0; i < NB_BLOCS_HAUTEUR; i++) //scan le fichier
    {
        fgets(ligne, NB_BLOCS_LARGEUR*3, fichier); //récupère la ligne du fichier
        for (int j = 0; j < NB_BLOCS_LARGEUR; j++)
        {
            switch (ligne[j*3]) //insère les bons numéro dans le tableau du niveau
            {
            case '0': // espace vide/chemin
                niveau[j][i] = 0;
                break;
            case '1': //mur
                niveau[j][i] = 1;
                break;
            case '2': //objectif
                niveau[j][i] = 2;
                break;
            case '3': //endroit ou le joueur aparait
                niveau[j][i] = 3;
                break;
            case '4': //stopper
                niveau[j][i] = 4;
                break;
            case '5': //Redirection Haut
                niveau[j][i] = 5;
                break;
            case '6': //Redirection Droite
                niveau[j][i] = 6;
                break;
            case '7': //Redirection Bas
                niveau[j][i] = 7;
                break;
            case '8': //Redirection Gauche
                niveau[j][i] = 8;
                break;
            case '9': //mur actif
                niveau[j][i] = 9;
                break;
            case '&': //mur non actif
                niveau[j][i] = 10;
                break;
            default:
                break;
            }
        }
    }

    fclose(fichier); //fermeture du fichier
    return false;
}
