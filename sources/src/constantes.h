#ifndef CONSTANTES_H
#define CONSTANTES_H

#define HEIGHT 600
#define WIDHT 600

#define NB_BLOCS_HAUTEUR 15
#define NB_BLOCS_LARGEUR 15

#define NB_PIXEL_HAUTEUR 40
#define NB_PIXEL_LARGEUR 40

//#define NB_LEVELS 8
#define SDL_AUCUN 66

#define NB_PAGES_TUTO 3

enum
{
    FIXE, HAUT, GAUCHE, DROITE, BAS, STOP
};

#define OBJETMAX 11
enum
{
    VIDE, MUR, OBJECTIF, SPAWN, STOPPER, REHAUT, REDROITE, REBAS, REGAUCHE, MURINV, MUROPA
};

/*enum
{
    REDHAUT, REDDROITE, REDBAS, REDGAUCHE
};*/

enum
{
    JOUER, EDITEUR, QUITTER, RIEN, TUTO, ESCAPE
};

enum
{
    MPRINCIPAL, MJOUER, MEDITEUR
};

enum
{
    NORMAL, PERSONNALISE
};

typedef struct
{
    char nbObj;
    char nbObjCapt;

}level;

typedef struct rectclick
{
    int x;
    int y;
    int xl;
    int yl;
}rectclick;

#define MOVESPEED 60.0


#endif // CONSTANTES_H
