#include "Include/Define.h"

pecheur_default joueur_d;
pont_default pont_d;
objet **grille;
pecheurs *joueurs;
espece_default *espece_d;
int **peut_manger;
int **graphe;
clr *tab_clr;
int TAILLE;
int Nb_objet=0;
int fini=0;
int Nb_vivant=0;
int Nb_tour=0;
int Nb_Joueur_tour=0;
int nb_affiche_graphe;
int Nb_joueur=-1;
int taille_cadre_X;
int taille_cadre_Y;
SDL_Surface *ecran=NULL;
int err_flag=0;
int cpt_peut_marcher;
char erreur[5][20];
int fils;
/*
   Alarme qui permet de kill le fils qui affiche l'ecran d'acceuil si
   l'utilisateur fait CTRL+C durant le choix du nombre de joueur
 */
void INTHandler(){
	kill(fils,SIGKILL);
}

/*
   Cette fonction va remplir les données utilisables par le graphique en mode information
   et il va détecter le cas de fin : Il n'existe plus de poisson pechable.
 */
void compte_grille(){
	int i,j;
	int nb_peut_manger=0;
	/*
	   Compte le nombre de poisson que peut pecher le joueur.
	 */
	for(i=0;i<Nb_vivant;i++){
		if(peut_manger[0][i+2])
			nb_peut_manger++;
	}
	/*
	   Décale tout le graphe de 1 case vers la gauche puisqu'un tour est passé.
	 */
	for(i=0;i<Nb_vivant+1;i++){
		for(j=0;j<nb_affiche_graphe;j++){
			if(j<nb_affiche_graphe-1)
				graphe[i][j]=graphe[i][j+1];
			else if (j==nb_affiche_graphe-1)
				graphe[i][j]=0;
		}
	}
	/*
	   Initialise chaque case de façon à ce que les animaux puissent de nouveau
	   manger et bouger.Compte le nombre de poisson présent dans la grille pour
	   chaque espece.
	 */
	for(i=0;i<TAILLE;i++){
		for(j=0;j<TAILLE;j++){
			grille[i][j].a_bouger=NON;
			grille[i][j].a_manger=NON;
			if(grille[i][j].est_vide!=1 && grille[i][j].nom>1){
				graphe[grille[i][j].nom-2][nb_affiche_graphe-1]++;
				graphe[Nb_vivant][nb_affiche_graphe-1]++;
			}
		}
	}
	/*
	   Si une espece pechable par le joueur a disparu,on décrement le nombre d'espece
	   pechable restant.
	 */
	for(i=0;i<Nb_vivant;i++)
		if(graphe[i][nb_affiche_graphe-1]==0 && peut_manger[0][i+2])
			nb_peut_manger--;
	/*
	   Si il ne reste plus rien a pecher, on arrete le jeu.
	 */
	if(nb_peut_manger==0)
		fini=2 ;
}
/*
   Execute chaques étapes du cycle
 */
void cycle(int i,int j){
	Survie(&grille[i][j]);
	Reproduction(&grille[i][j],i,j);
	Predation(&grille[i][j],i,j);
	Deplacement(&grille[i][j],i,j);
	Tour(&grille[i][j]);
}
int main(int argc,char *argv[])
{
	int i,j_gagner=0,go=0;
	int Joueur_courant=1,mode=Jeux,done=0,x=0,y=0;
	SDL_Event event;
	/*
	   Vérifie le nombre d'argument
	 */
	if(argc!=1){
		printf("Usage : %s\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	/*
	   Affiche message bienvenue
	 */
	/*
	   On crée un processus qui va afficher l'écran d'accueil
	 */
	fils=fork();
	/*
	   Pour kill le fils en cas de CTRL+C,on arme une alarme
	 */
	signal(SIGINT,INTHandler);

	switch(fils){
		case -1:
			exit(EXIT_FAILURE);
			break;
		case 0 :
			/*
			   Le fils lance la musique d'acceuil puis
			   affiche l'ecran d'accueil
			 */
			charger_musique_accueil();
			afficher_accueil_SDL();
			break;
		default :
			/*
			   Initialise le jeu
			 */
			srand(time(NULL));
			mode=nombre_de_joueur();
			if(mode==-1){
				kill(fils,SIGKILL);
				exit(EXIT_FAILURE);
			}
			charger_config();
			charger_ecosystem();
			creation_SDL();
			compte_grille();
			kill(fils,SIGKILL);
			break;
	}
	/*
	   Termine la musique d'accueil et commence celle de fond si
	   l'utilisateur le veux(configuration.txt)
	 */
	terminer_musique_accueil();
	charger_musique_fond();
	afficher_SDL(mode,Joueur_courant);
	cpt_peut_marcher=joueur_d.nb_depl_par_tour;
	while (!fini)
	{
		/*
		   Si un message d'erreur doit etre afficher
		 */
		if(err_flag)
			afficher_SDL(mode,Joueur_courant);
		/*
		   Si une action a été faite hors déplacement,et que ça n'a pas encore été 
		   affiché.
		 */
		if(done){
			afficher_SDL(mode,Joueur_courant);
			go=1;
		}
		if(go){
			/*
			   Reinitialise le nombre de tour
			 */
			cpt_peut_marcher=joueur_d.nb_depl_par_tour;
			/*
			   Si le joueur a pecher de nouveaux poissons,on affiche le sac pendant 2sec
			 */
			if(Nb_joueur>0 && joueurs[Joueur_courant-1].new_poisson){
				afficher_SDL(Sac,Joueur_courant);
				sleep(2);
				joueurs[Joueur_courant-1].new_poisson=0;
			}
			done=0;
			go=0;
			/*
			   Fait tourner l'écosysteme sur 5 tours.
			 */
			for(i=0;i<5;i++){
				for(x=0;x<TAILLE;x++){
					for(y=0;y<TAILLE;y++){
						cycle(x,y);
					}
				}
				compte_grille();
				Nb_tour++;
			}
			/*
			   Si il existe des joueurs on augmente leur tour
			 */
			if(mode==Jeux && Nb_joueur>0)
				Nb_Joueur_tour++;
			/*
			   On passe au joueur suivant et si le joueur nage, on augmente la valeur de peut_nager
			 */
			if(Nb_joueur>=1 && mode!=Information){
				Joueur_courant++;
				if(Joueur_courant>Nb_joueur)
					Joueur_courant=1;
				if(joueurs[Joueur_courant-1].nage && joueurs[Joueur_courant-1].peut_nager<2)
					joueurs[Joueur_courant-1].peut_nager++;
			}
			/*
			   Puis on affiche la nouvelle map
			 */
			afficher_SDL(mode,Joueur_courant);

		}
		/*
		   Si il n'y pas de joueur ,tout ce que l'on attend c'est d'appuyer sur la touche
		   entrée pour faire passé un tour ,donc on attend pas que la touche soit relaché
		   =>SDL_POLL_EVENT, sinon on attend.On fait cela afin de rendre le jeu plus
		   fluide lors de l'utilisation sans joueurs.
		 */
		if(Nb_joueur>0)
			SDL_WaitEvent (&event);
		else
			SDL_PollEvent (&event);
		switch(event.type)
		{
			/*
			   Si on change la taille de l'écran,on modifie l'affichage
			 */
			case SDL_VIDEORESIZE:
				taille_cadre_X=event.resize.w;
				taille_cadre_Y=event.resize.h;
				ecran = SDL_SetVideoMode(event.resize.w, event.resize.h, 32,SDL_HWSURFACE | SDL_RESIZABLE);
				afficher_SDL(mode,Joueur_courant);
				break;
				/*
				   Si on quitte le jeu,on signale que le jeu est fini
				 */
			case SDL_QUIT:
				fini = 1;
				break;
			case SDL_MOUSEMOTION:
				x=event.motion.x;
				y=event.motion.y;
				break;
				/*
				   Si on utilise la souri
				 */
			case SDL_MOUSEBUTTONDOWN:
				x=event.motion.x;
				y=event.motion.y;
				switch(event.button.button){
					/*
					   CLICK GAUCHE=>lancer un poisson
					 */
					case SDL_BUTTON_LEFT:
						if(!done && Nb_joueur>0 && !joueurs[Joueur_courant-1].nage)
							done=lancer(Joueur_courant,x,y);
						break;
				}
				break;
				/*
				   Si on appuie sur une touche
				 */
			case SDL_KEYDOWN:
				/*
				   Si on appuie en meme temps sur Maj,SHIFT Gauche
				 */
				if ((event.key.keysym.mod & KMOD_LCTRL) == KMOD_LCTRL)
					switch (event.key.keysym.sym)
					{
						/*
						   Si on appuie sur i,on affiche le graphique et les animaux sous l'eau
						 */
						case SDLK_i:
							if(mode!=Information)
								mode=Information;
							else if (Nb_joueur>0)
								mode=Jeux;
							afficher_SDL(mode,Joueur_courant);
							break;
						default:
							break;
					}
				else
					switch (event.key.keysym.sym)
					{
						/*
						   touche R => redémarre la carte
						 */
						case SDLK_r:
							err_flag=0;
							Nb_tour=0;
							Nb_Joueur_tour=0;
							fini=0;
							reinitialise();
							charger_ecosystem();
							compte_grille();
							afficher_SDL(mode,Joueur_courant);
							break;
							/*
							   touche S => affiche le sac
							 */
						case SDLK_s:
							if(mode!=Sac && Nb_joueur>0)
								mode=Sac;
							else if (Nb_joueur>0)
								mode=Jeux;
							afficher_SDL(mode,Joueur_courant);
							break;
							/*
							   touche C => peche canne
							 */
						case SDLK_c:
							if(!done && Nb_joueur>0 && !joueurs[Joueur_courant-1].nage)
								done=Pecher(Joueur_courant,x,y,CANNE);
							break;
							/*
							   touche F => peche filet
							 */
						case SDLK_f:
							if(!done && Nb_joueur>0 && !joueurs[Joueur_courant-1].nage)
								done=Pecher(Joueur_courant,x,y,FILET);
							break;
							/*
							   touche ESC => fini le jeu
							 */
						case SDLK_ESCAPE:
							fini = 1;
							break;
							/*
							   touche Entree => passe au tour suivant
							 */	                
						case SDLK_RETURN:
							if(mode!=Information)
								mode=Jeux;
							go=1;
							break;

						default:
							break;
					}
				break;
			default:
				break;
		}
	}

	if(fini==1){
		/*
		   Cherche si un joueur a gagné
		 */
		for(i=0;i<Nb_joueur;i++){
			if(joueurs[i].arrivee==BAS && joueurs[i].Y==TAILLE-2)
				j_gagner=i+1;
			if(joueurs[i].arrivee==HAUT && joueurs[i].Y==1)
				j_gagner=i+1;
			if(joueurs[i].arrivee==DROITE && joueurs[i].X==TAILLE-2)
				j_gagner=i+1;
			if(joueurs[i].arrivee==GAUCHE && joueurs[i].X==1)
				j_gagner=i+1;
		}
		/*
		   Affiche le gagnant si il existe
		 */
		if(Nb_joueur>0 && j_gagner!=0)
			afficher_gagnant_SDL(j_gagner);
	}
	/*
	   Libere la mémoire,l'écran,termine la musique de fond
	   si elle existe et quitte SDL
	 */
	fin();
	terminer_musique_fond();
	SDL_FreeSurface(ecran);
	SDL_Quit();
	return EXIT_SUCCESS;
}
