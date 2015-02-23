#include "Define.h"

extern objet **grille;
extern pont_default pont_d;
extern espece_default *espece_d;
extern pecheur_default joueur_d;
extern pecheurs *joueurs;
extern int **peut_manger;
extern int TAILLE;
extern int Nb_tour;
extern int Nb_objet;
extern int Nb_vivant;
/*
Selon l'espece récupéré en argument, 
la fonction crée un objet,le vide,puis défini cet
objet comme une case animal de l'espece e
*/
objet convertion_animal_to_objet(espece_default e){
	objet a;
	vide(&a);
	a.nom=e.numero;
	a.satiete=e.taille_du_bide;
	a.dernier_repas=Nb_tour;
	a.derniere_reproduction=Nb_tour;
	a.est_vivant=OUI;
	a.est_vide=NON;
	return a;
}
/*
Vide un objet
*/
void vide(objet *obj){
	obj->nom=0;
	obj->est_vivant=NON;
	obj->satiete=0;
	obj->dernier_repas=0;
	obj->derniere_reproduction=0;
	obj->est_pecheur=NON;
	obj->J_numero=0;
	obj->est_pont=NON;
	obj->soliditee=0;
	obj->est_vide=OUI;
	obj->a_bouger=OUI;
	obj->a_manger=OUI;
}
/*
Copie un objet(1) sur un autre(2) puis vide l'objet 1
*/
void transfert(objet *obj1,objet *obj2){
	obj2->nom=obj1->nom;
	obj2->est_vivant=obj1->est_vivant;
	obj2->satiete=obj1->satiete;
	obj2->dernier_repas=obj1->dernier_repas;
	obj2->derniere_reproduction=obj1->derniere_reproduction;
	obj2->est_pecheur=obj1->est_pecheur;
	obj2->est_pont=obj1->est_pont;
	obj2->soliditee=obj1->soliditee;
	obj2->est_vide=obj1->est_vide;
	obj2->a_bouger=obj1->a_bouger;
	obj2->a_manger=obj1->a_manger;
	vide(obj1);
}
/*
Si l'objet est une espece et que sa durée de survie est supérieur à 0(non immortel)
,que son dernier repas remonte à une date trop lointaine et qu'il a faim,on vide l'objet
ou il se trouve.
*/
void Survie(objet *obj){
	if(obj->est_vivant  && espece_d[obj->nom-2].duree_survie>0)
		if(obj->satiete==0 && (Nb_tour-obj->dernier_repas>=espece_d[obj->nom-2].duree_survie))
			vide(obj);
}
/*
Si l'objet est une espece et que sa durée de survie est supérieur à 0(non immortel)
,on augmente sa faim en réduisant sa satiete
*/
void Tour(objet *obj){
	if(obj->est_vivant && espece_d[obj->nom-2].duree_survie>0){
		if(obj->satiete-espece_d[obj->nom-2].metabolisme>0)
			obj->satiete=obj->satiete-espece_d[obj->nom-2].metabolisme;
		else{
			obj->satiete=0;
		}
	}
}
/*
Tente de faire manger un animal, un animal adjacent
*/
void Predation(objet *obj1,int i,int j){
	int try_eat=1,num;
	int k=0;
	objet *tableau_try_eat[8];
	objet *obj2;
	/*
	On va ajouter dans un tableau,toutes les cases qui peuvent etre des proies
	*/
	if(!grille[i][j].a_manger && grille[i][j].est_vivant){
		if(i>0 && !grille[i-1][j].est_vide && (peut_manger[obj1->nom][grille[i-1][j].nom] || (grille[i-1][j].nom==0 && peut_manger[obj1->nom][1])))
			tableau_try_eat[k++]=&grille[i-1][j];
		if(i<TAILLE-1 && !grille[i+1][j].est_vide && (peut_manger[obj1->nom][grille[i+1][j].nom] || (grille[i+1][j].nom==0 && peut_manger[obj1->nom][1])))
			tableau_try_eat[k++]=&grille[i+1][j];
		if(j>0 && !grille[i][j-1].est_vide && (peut_manger[obj1->nom][grille[i][j-1].nom] || (grille[i][j-1].nom==0 && peut_manger[obj1->nom][1])))
			tableau_try_eat[k++]=&grille[i][j-1];
		if(j<TAILLE-1 && !grille[i][j+1].est_vide && (peut_manger[obj1->nom][grille[i][j+1].nom] || (grille[i][j+1].nom==0 && peut_manger[obj1->nom][1])))
			tableau_try_eat[k++]=&grille[i][j+1];
		if(i>0 && j>0 && !grille[i-1][j-1].est_vide && (peut_manger[obj1->nom][grille[i-1][j-1].nom] || (grille[i-1][j-1].nom==0 && peut_manger[obj1->nom][1])))
			tableau_try_eat[k++]=&grille[i-1][j-1];
		if(i>0 && j<TAILLE-1 && !grille[i-1][j+1].est_vide && (peut_manger[obj1->nom][grille[i-1][j+1].nom] || (grille[i-1][j+1].nom==0 && peut_manger[obj1->nom][1])))
			tableau_try_eat[k++]=&grille[i-1][j+1];
		if(i<TAILLE-1 && j>0 && !grille[i+1][j-1].est_vide && (peut_manger[obj1->nom][grille[i+1][j-1].nom] || (grille[i+1][j-1].nom==0 && peut_manger[obj1->nom][1])))
			tableau_try_eat[k++]=&grille[i+1][j-1];
		if(i<TAILLE-1 && j<TAILLE-1 && !grille[i+1][j+1].est_vide && (peut_manger[obj1->nom][grille[i+1][j+1].nom] || (grille[i+1][j+1].nom==0 && peut_manger[obj1->nom][1])))
			tableau_try_eat[k++]=&grille[i+1][j+1];
		if(k==0)
			try_eat=0;
	}
	else try_eat=0;

	if(try_eat){
		/*
		On en choisi une au hasard
		*/
		obj2=tableau_try_eat[rand()%k];
		/*
		Si c'est un pecheur qui nage on le mange
		*/
		if(obj2->nom==0 && !obj2->est_pont && peut_manger[obj1->nom][obj2->nom]){
			respawn(obj2->J_numero);
			obj1->dernier_repas=Nb_tour;
			obj1->satiete+=joueur_d.taille;
			obj1->a_manger=1;
			transfert(obj1,obj2);
		}
		/*
		Si c'est un pont on l'affaiblie,si il tombe a 0 de soliditee,on le mange
		*/
		else if(obj2->nom==1 && obj2->soliditee>0){
			obj2->soliditee--;
			obj1->dernier_repas=Nb_tour;
			obj1->satiete+=pont_d.taille;
			obj1->a_manger=1;
			if(obj2->soliditee==0)		
				transfert(obj1,obj2);
		}
		/*
		Si c'est un joueur sur un pont,on l'affaiblie pour le faire tomber à l'eau
		*/
		else if(obj2->nom==0 && obj2->est_pont && peut_manger[obj1->nom][1]){
			obj2->soliditee--;
			obj1->dernier_repas=Nb_tour;
			obj1->satiete+=pont_d.taille;
			obj1->a_manger=1;
			/*
			Si il tombe à l'eau,le joueur commence à nager
			*/
			if(obj2->soliditee==0){
				obj2->est_pont=NON;
				joueurs[obj2->J_numero-1].nage=OUI;
				/*
				Si il tombe a l'eau près d'un bord,il respawn tout de suite
				*/
				if(joueurs[obj2->J_numero-1].Y==TAILLE-2 || joueurs[obj2->J_numero-1].Y==1 || joueurs[obj2->J_numero-1].X==TAILLE-2 || joueurs[obj2->J_numero-1].X==1){
					num=obj2->J_numero;
					vide(&grille[joueurs[obj2->J_numero-1].X][joueurs[obj2->J_numero-1].Y]);
					respawn(num);
				}
			}
		}
		/*
		Si c'est une espece,on la mange
		*/
		else if(peut_manger[obj1->nom][obj2->nom] && !obj2->est_pont){
			obj1->dernier_repas=Nb_tour;
			obj1->satiete+=espece_d[obj2->nom-2].taille;
			obj1->a_manger=OUI;
			transfert(obj1,obj2);
		}
	}
}
/*
Déplacement dans l'une des 8 directions
*/
void Deplacement_rand(int direction,objet *obj,int nb_case_bis,int x,int y){
	int objX=x,objY=y,nb_case=nb_case_bis;
	int ajouty=0;
	int ajoutx=0;
	if(obj->satiete>nb_case_bis && obj->a_bouger==0 && obj->est_vivant && espece_d[obj->nom-2].saut_max>0 && nb_case!=0){
		/*
		Quels coordonnées vont changer ?
		*/
		if(direction==HAUT)
			ajouty=-1;
		else if(direction==DROITE)
			ajoutx=1;
		else if(direction==BAS)
			ajouty=1;
		else if(direction==GAUCHE)
			ajoutx=-1;
		else if(direction==HAUTDROITE){
			ajoutx=1;
			ajouty=-1;
		}
		else if(direction==HAUTGAUCHE){
			ajouty=-1;
			ajoutx=-1;
		}
		else if(direction==BASDROITE){
			ajouty=1;
			ajoutx=1;
		}
		else if(direction==BASGAUCHE){
			ajouty=1;
			ajoutx=-1;
		}
		/*
		On regarde si on peux le faire déplacer jusqu'à sa nouvelle case
		*/
		while(nb_case>0 && y<TAILLE-1 && y>0 && x<TAILLE-1 && x>0 && grille[x+ajoutx][y+ajouty].est_vide){
			nb_case--;
			y+=ajouty;
			x+=ajoutx;
		}
		/*Si il peut se déplacer,onle transfert jusqu'à sa nouvelle case*/
		if(x!=objX || y!=objY){
			nb_case=nb_case_bis-nb_case;
			obj->satiete-=nb_case;
			if (obj->satiete<0)
				obj->satiete=0;
			obj->a_bouger=1;
			transfert(obj,&grille[x][y]);
		}
	}
}
/*
Cette fonction tente de chercher une proie visible par le prédateur,puis s'en approcher,
si il n'en trouve pas,le déplacement est random.
*/
void Deplacement(objet *obj,int x,int y){
	int objX=x,objY=y;
	int nb_case_bis;
	if(obj->est_vivant)
		nb_case_bis=espece_d[obj->nom-2].saut_max;
	else
		return;
	if(obj->satiete>nb_case_bis && obj->a_bouger==0 && obj->est_vivant && espece_d[obj->nom-2].saut_max>0){
		int trouver=0,nb_case=nb_case_bis;
		while(nb_case>0 && y>0 && grille[x][y-1].est_vide){
			nb_case--;
			y--;
		}
		nb_case=nb_case_bis;
		if((x!=objX || y!=objY)&&(!grille[x][y].est_vide && peut_manger[obj->nom][grille[x][y].nom]))
			trouver=1;
		if(!trouver){
			x=objX;
			y=objY;
			while(nb_case>0 && x<TAILLE-1 && grille[x+1][y].est_vide){
				nb_case--;
				x++;
			}
			nb_case=nb_case_bis;
			if((x!=objX || y!=objY)&&(!grille[x][y].est_vide && peut_manger[obj->nom][grille[x][y].nom]))
				trouver=1;
		}
		if(!trouver){
			x=objX;
			y=objY;
			while(nb_case>0 && y<TAILLE-1 && grille[x][y+1].est_vide){
				nb_case--;
				y++;
			}
			nb_case=nb_case_bis;
			if((x!=objX || y!=objY)&&(!grille[x][y].est_vide && peut_manger[obj->nom][grille[x][y].nom]))
				trouver=1;
		}
		if(!trouver){
			x=objX;
			y=objY;
			while(nb_case>0 && x>0 && grille[x-1][y].est_vide){
				nb_case--;
				x--;
			}
			nb_case=nb_case_bis;
			if((x!=objX || y!=objY)&&(!grille[x][y].est_vide && peut_manger[obj->nom][grille[x][y].nom]))
				trouver=1;
		}
		if(!trouver){
			x=objX;
			y=objY;
			while(nb_case>0 && y>0 && x<TAILLE-1 && grille[x+1][y-1].est_vide){
				nb_case--;
				y--;
				x++;
			}
			nb_case=nb_case_bis;
			if((x!=objX || y!=objY)&&(!grille[x][y].est_vide && peut_manger[obj->nom][grille[x][y].nom]))
				trouver=1;
		}
		if(!trouver){
			x=objX;
			y=objY;
			while(nb_case>0 && x>0 && y>0 && grille[x-1][y-1].est_vide){
				nb_case--;
				x--;
				y--;
			}
			nb_case=nb_case_bis;
			if((x!=objX || y!=objY)&&(!grille[x][y].est_vide && peut_manger[obj->nom][grille[x][y].nom]))
				trouver=1;
		}
		if(!trouver){
			x=objX;
			y=objY;
			while(nb_case>0 && x<TAILLE-1 && y<TAILLE-1 && grille[x+1][y+1].est_vide){
				nb_case--;
				x++;
				y++;
			}
			nb_case=nb_case_bis;
			if((x!=objX || y!=objY)&&(!grille[x][y].est_vide && peut_manger[obj->nom][grille[x][y].nom]))
				trouver=1;
		}
		if(!trouver){
			x=objX;
			y=objY;
			while(nb_case>0 && y<TAILLE-1 && x>0 && grille[x-1][y+1].est_vide){
				nb_case--;
				y++;
				x--;
			}
			nb_case=nb_case_bis;
			if((x!=objX || y!=objY)&&(!grille[x][y].est_vide && peut_manger[obj->nom][grille[x][y].nom]))
				trouver=1;
		}
		/*
		Si il a trouvé une proie
		*/
		if(trouver){
			nb_case=nb_case_bis-nb_case;
			obj->satiete-=nb_case;
			obj->a_bouger=1;
			transfert(obj,&grille[x][y]);
		}
		else{
			Deplacement_rand(rand()%8,obj,rand()%nb_case_bis,objX,objY);
		}
	}
}
/*
Si l'animal peut se reproduire,on regarde une par une si il existe une case adjacente qui
soit vide.Puis on choisi au hasard une case parmis celle qui sont vide.
*/
void Reproduction(objet *obj,int x,int y){
	int objX=x,objY=y;
	int tableau_try_reprod_x[8];
	int tableau_try_reprod_y[8];
	int k=0;
	/*
	Si l'animal peut se reproduire
	*/
	if(obj->est_vivant && espece_d[obj->nom-2].frequence_reproduction>0)
		if(obj->satiete>=espece_d[obj->nom-2].gestation*espece_d[obj->nom-2].metabolisme){
			if(obj->derniere_reproduction+espece_d[obj->nom-2].frequence_reproduction<Nb_tour){

				if(x>0 && grille[x-1][y].est_vide){
					tableau_try_reprod_x[k]=x-1;
					tableau_try_reprod_y[k++]=y;
				}
				else if(x<TAILLE-1 && grille[x+1][y].est_vide){
					tableau_try_reprod_x[k]=x+1;
					tableau_try_reprod_y[k++]=y;
				}
				else if(y<TAILLE-1 && grille[x][y+1].est_vide){
					tableau_try_reprod_x[k]=x;
					tableau_try_reprod_y[k++]=y+1;
				}
				else if(y>0 && grille[x][y-1].est_vide){
					tableau_try_reprod_x[k]=x;
					tableau_try_reprod_y[k++]=y-1;
				}
				else if(x<TAILLE-1 && y>0 && grille[x+1][y-1].est_vide){
					tableau_try_reprod_x[k]=x+1;
					tableau_try_reprod_y[k++]=y-1;
				}
				else if(x<TAILLE-1 && y<TAILLE-1 && grille[x+1][y+1].est_vide){
					tableau_try_reprod_x[k]=x+1;
					tableau_try_reprod_y[k++]=y+1;
				}
				else if(x>0 && y<TAILLE-1 && grille[x-1][y+1].est_vide){
					tableau_try_reprod_x[k]=x-1;
					tableau_try_reprod_y[k++]=y+1;
				}
				else if(x>0 && y>0 && grille[x-1][y-1].est_vide){
					tableau_try_reprod_x[k]=x-1;
					tableau_try_reprod_y[k++]=y-1;
				}
				/*
				Si il existe une case vide autour de l'animal
				*/
				if(k!=0){
					k=rand()%k;
					x=tableau_try_reprod_x[k];
					y=tableau_try_reprod_y[k];
				}
				if(x!=objX || y!=objY){
					obj->satiete=obj->satiete-espece_d[obj->nom-2].gestation*espece_d[obj->nom-2].metabolisme;
					obj->derniere_reproduction=Nb_tour;
					grille[x][y]=convertion_animal_to_objet(espece_d[obj->nom-2]);
				}
			}	
		}

}
