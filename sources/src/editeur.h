/*
    fichier editeur.h
----------

Par Baptiste CHASSAIS
Inclus les fonctions de editeur.c

Création le 26/03/2018 14:25
Dernière Modification le 20/04/2018 10:37

*/

#ifndef EDITEUR_H
#define EDITEUR_H

void editeur(SDL_Renderer *prender, SDL_Texture *blocs[], const bool personnalise);
void placement(bool key[], char niveau[][NB_BLOCS_HAUTEUR], SDL_Event ev, int *objetActu);
void sauvegarderNiveau(char niveau[][NB_BLOCS_HAUTEUR]);
bool verificationNiveau(char niveau[][NB_BLOCS_HAUTEUR]);
void niveauEdit(char niveau[][NB_BLOCS_HAUTEUR]);
void cleanNiveau(char niveau[][NB_BLOCS_HAUTEUR]);

#endif // EDITEUR_H
