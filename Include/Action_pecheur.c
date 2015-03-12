#include "Define.h"

extern objet **grille;
extern pecheurs *joueurs;
extern espece_default *espece_d;
extern pecheur_default joueur_d;
extern pont_default pont_d;
extern int **peut_manger;
extern int TAILLE;
extern int Nb_objet;
extern int taille_cadre_X;
extern int taille_cadre_Y;
extern int fini;
extern int cpt_peut_marcher;
/*
Selon le numéro J_numero récupéré en argument, 
la fonction crée un objet,le vide,puis défini cet
objet comme une case pecheur ayant le numéro de 
joueur J_numero
*/
objet convertion_pecheur_to_objet(int J_numero){
	objet a;
	vide(&a);
	a.nom=joueur_d.numero;
	a.est_pecheur=OUI;
	a.J_numero=J_numero;
	a.est_pont=OUI;
	a.est_vide=NON;
	a.soliditee=pont_d.soliditee;
	return a;
}
/*
Crée un objet,le vide,puis le défini comme un pont
*/
objet convertion_pont_to_objet(){
	objet a;
	vide(&a);
	a.nom=pont_d.numero;
	a.soliditee=pont_d.soliditee;
	a.est_pont=OUI;
	a.est_vide=NON;
	return a;
}
/*
Fait spawner aléatoirement un joueur (lors de l'initialisation)
*/
void spawn_rand_pecheur(int J_numero){
	int rand_cote_pecheur,rand_num_case,Jx,Jy,arrivee;
	/*
	Tant que la case choisi est prise par un joueur,
	on cherche une case libre pour y placer le joueur 
	et on définit son but du jeu : arrivee
	*/
	do{
		rand_cote_pecheur=rand()%4;
		rand_num_case=rand()%(TAILLE-2)+1;
		if(rand_cote_pecheur==BAS){
			Jx=rand_num_case;
			Jy=TAILLE-1;
			arrivee=HAUT;
		}
		else if(rand_cote_pecheur==HAUT){
			Jx=rand_num_case;
			Jy=0;
			arrivee=BAS;
		}
		else if(rand_cote_pecheur==DROITE){
			Jx=TAILLE-1;
			Jy=rand_num_case;
			arrivee=GAUCHE;
		}
		else{
			Jx=0;
			Jy=rand_num_case;
			arrivee=DROITE;
		}
	}while(grille[Jx][Jy].est_pecheur);
	/*
	On initialise le joueur.
	*/
	joueurs[J_numero-1].sac_contient=NULL;
	joueurs[J_numero-1].arrivee=arrivee;
	joueurs[J_numero-1].XP=0;
	joueurs[J_numero-1].sac=0;
	joueurs[J_numero-1].new_poisson=0;
	joueurs[J_numero-1].X=Jx;
	joueurs[J_numero-1].Y=Jy;
	joueurs[J_numero-1].SpawnX=Jx;
	joueurs[J_numero-1].SpawnY=Jy;
	joueurs[J_numero-1].nage=NON;
	joueurs[J_numero-1].peut_nager=NON;
	joueurs[J_numero-1].nombre_de_poissons=0;
	joueurs[J_numero-1].nombre_planctons=0;
	joueurs[J_numero-1].nombre_predateurs=0;
	joueurs[J_numero-1].nombre_herbivores=0;
/*
	On vide la case qui va contenir le joueur;
	*/
	vide(&grille[Jx][Jy]);
	/*
	On converti la case cible en pecheur.
	*/
	grille[Jx][Jy]=convertion_pecheur_to_objet(J_numero);
	grille[Jx][Jy].soliditee=0;
}
void respawn(int J_numero){
	/*
	On fait respawn le joueur dans sa case spawn
	*/
	grille[joueurs[J_numero-1].SpawnX][joueurs[J_numero-1].SpawnY]=convertion_pecheur_to_objet(J_numero);
	grille[joueurs[J_numero-1].SpawnX][joueurs[J_numero-1].SpawnY].soliditee=0;
	/*
	On redéfini les informations du joueur
	*/
	joueurs[J_numero-1].nage=NON;/*Si il était en train de nager*/
	joueurs[J_numero-1].X=joueurs[J_numero-1].SpawnX;
	joueurs[J_numero-1].Y=joueurs[J_numero-1].SpawnY;
	joueurs[J_numero-1].peut_nager=NON;
}
/*
Empile un poisson dans le sac d'un joueur
*/
void empile(int qui,int quoi){
	pile *p;
	p=malloc(sizeof(pile));
	p->numero=quoi;
	p->reste=espece_d[quoi-2].taille;
	p->suivant=NULL;
	if(joueurs[qui].sac_contient==NULL)
		joueurs[qui].sac_contient=p;
	else{
		p->suivant=joueurs[qui].sac_contient;
		joueurs[qui].sac_contient=p;
	}
}
/*
Depile un poisson du sac d'un joueur
*/
int depile(int qui){
	if(joueurs[qui].sac_contient!=NULL){
		pile * p=joueurs[qui].sac_contient->suivant;
		free(joueurs[qui].sac_contient);
		joueurs[qui].sac_contient=p;
		return 0;
	}
	else
		return 1;
}
/*
Lance un poisson provenant du sac d'un joueur vers une case.
Si l'action est réussi, renvoi 1 sinon 0
*/
int lancer(int Joueur_courant,int x,int y){
	int done=0,i,j;
	if(x<=taille_cadre_X*3/4 && y<=taille_cadre_Y*3/4){
		/*
		Converti les coordonnées SDL en coordonnées grille
		*/
		i=TAILLE*x/(taille_cadre_X*3/4);
		j=TAILLE*y/(taille_cadre_Y*3/4);
		/*
		Vérifie que le sac n'est pas vide et que le choix n'est pas trop loin
		*/
		if(joueurs[Joueur_courant-1].sac>0 && (abs(joueurs[Joueur_courant-1].X-i)+abs(joueurs[Joueur_courant-1].Y-j))<3*joueur_d.taille_canne_a_peche){
			if(!grille[i][j].est_pont && !grille[i][j].est_pecheur){
				grille[i][j]=convertion_animal_to_objet(espece_d[joueurs[Joueur_courant-1].sac_contient->numero-2]);
				joueurs[Joueur_courant-1].sac-=joueurs[Joueur_courant-1].sac_contient->reste;
				depile(Joueur_courant-1);
				done=1;
			}
			else
				afficher_message_erreur("Case invalide");
		}
		else if(joueurs[Joueur_courant-1].sac==0)
			afficher_message_erreur("Sac vide");
		else
			afficher_message_erreur("Trop loin");
	}
	else
		afficher_message_erreur("Hors grille");
	return done;
}

int Pecher(int Joueur_courant,int x,int y,int mode){
	int xdepart,ydepart,xjoueur,yjoueur;
	int i,imin=1,imax=TAILLE-2;
	int j,jmin=1,jmax=TAILLE-2;
	int distance=0;
	int nb=9;
	/*
	Les variables xdepart et ydepart représente les coordonées du centre
	de la recherche de poisson
	*/
	xjoueur=joueurs[Joueur_courant-1].X;
	yjoueur=joueurs[Joueur_courant-1].Y;
	/*
		Si on utilise une canne
	*/
	if(mode==CANNE)
		distance=joueur_d.taille_canne_a_peche;
	else{
		if(joueurs[Joueur_courant-1].XP<joueur_d.debloque_filet){
			afficher_message_erreur("Xp Insuffisant");
			return 0;
		}
		distance=joueur_d.distance_peche_filet;
	}
	/*
		on convertie les coordonnées SDL en coordonnées
		grille,si on clique a coté de la grille ou trop loin du joueur on return 0
	*/
	if(x<=taille_cadre_X*3/4 && y<=taille_cadre_Y*3/4){
		xdepart=TAILLE*x/(taille_cadre_X*3/4);
		ydepart=TAILLE*y/(taille_cadre_Y*3/4);
	}
	else{
		afficher_message_erreur("Hors grille");
		return 0;
	}
	/*
	Trop loin ?
	*/
	if(abs(xjoueur-xdepart)+abs(yjoueur-ydepart)>distance){
		afficher_message_erreur("Trop loin");
		return 0;
	}
	if(mode==CANNE){
		if(peut_manger[0][grille[xdepart][ydepart].nom]){
			joueurs[Joueur_courant-1].sac+=espece_d[grille[xdepart][ydepart].nom-2].taille;
			joueurs[Joueur_courant-1].XP+=espece_d[grille[xdepart][ydepart].nom-2].taille*7/3;
			empile(Joueur_courant-1,grille[xdepart][ydepart].nom);
			joueurs[Joueur_courant-1].new_poisson++;
			vide(&grille[xdepart][ydepart]);
		}
	}
	else{
		/*
		regarde les coordonées qui sont intéressantes pour la recherche de poisson,
		c'est à dire les cases qui sont situé a x-distance et y-distance
		*/
		if(xdepart-1>1)
			imin=xdepart-1;
		if(xdepart+1<TAILLE-2)
			imax=xdepart+1;
		if(ydepart-1>1)
			jmin=ydepart-1;
		if(ydepart+1<TAILLE-2)
			jmax=ydepart+1;
		/*
		met a jour le nombre de poisson peché
		*/
		joueurs[Joueur_courant-1].new_poisson=0;
		for(i=imin;i<=imax;i++){
			for(j=jmin;j<=jmax;j++){
				if(!grille[i][j].est_pont && !grille[i][j].est_pecheur){
					/*
					Si on se trouve à une distance acceptable:
						-on met à jour la taille du sac
						-on met à jour le nombre d'XP du joueur
						-on empile le poisson
						-on met à jour lenombre de nouveau poisson
						-on vide la case ou se trouvait le poisson
						-on décrement le nombre de poisson que l'on peut encore pecher
						-on dit qu'une action a été réalisé
					*/
					if(nb>0 && ((abs(i-xdepart)+abs(j-ydepart))<distance) && peut_manger[0][grille[i][j].nom]){
						joueurs[Joueur_courant-1].sac+=espece_d[grille[i][j].nom-2].taille;
						joueurs[Joueur_courant-1].XP+=espece_d[grille[i][j].nom-2].taille*7/3;
						empile(Joueur_courant-1,grille[i][j].nom);
						joueurs[Joueur_courant-1].new_poisson++;
						vide(&grille[i][j]);
						if(nb==9)
							joueurs[Joueur_courant-1].XP-=joueur_d.debloque_filet;
						nb--;
					}
				}
			}
		}
	}
	return 1;
}
