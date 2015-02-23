#ifndef _POSIX_SOURCE

#define _POSIX_SOURCE

typedef struct clr
{
	int r;
	int g;
	int b;
}clr;

typedef struct espece_default{
	char nom[80];
	int numero;
	int duree_survie;
	int taille_du_bide;
	int frequence_reproduction;
	int taille;
	int saut_max;
	int metabolisme;
	int gestation;
	int nombre_espece;
	int ratio;
}espece_default;

typedef struct pecheur_default{
	int numero;
	int taille;
	int taille_canne_a_peche;
	int distance_peche_filet;
	int debloque_filet;
	int nb_depl_par_tour;
}pecheur_default;

typedef struct pont_default{
	int numero;
	int soliditee;
	int taille;
}pont_default;

typedef struct objet{
	int nom;
 	/*--------Animal--------*/
 	int est_vivant;
	int satiete;
	int dernier_repas;
	int derniere_reproduction;
	int a_manger;
	int a_bouger;

	/*--------pecheur-------*/
	int est_pecheur;
	int J_numero;
	
	
	/*-------Pont------------*/
	int est_pont;
	int soliditee;

	/*--------Vide-----------*/
	int est_vide;

}objet;

typedef struct pile{
	int numero;
	int reste;
	struct pile *suivant;
}pile;

typedef struct pecheurs{
	char nom[20];
	pile *sac_contient;
	int arrivee;
	int sac;
	int new_poisson;
	int XP;
	int X;
	int Y;
	int SpawnX;
	int SpawnY;
	int nage;
	int peut_nager;
}pecheurs;

#define PONT 1
#define PECHEUR 0

#define OUI 1
#define NON 0

#define Limite_nb_joueur 4

#define CANNE 0
#define FILET 1

#define HAUT 0
#define DROITE 1
#define BAS 2
#define GAUCHE 3
#define HAUTDROITE 4
#define HAUTGAUCHE 5
#define BASDROITE 6
#define BASGAUCHE 7

#define Jeux 0
#define Information 1
#define Sac 2

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "Regle_de_vie.h"
#include "Action_pecheur.h"
#include "init_fin.h"
#include "affichage.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#endif
