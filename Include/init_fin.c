#include "Define.h"

extern objet **grille;
extern espece_default *espece_d;
extern pecheur_default joueur_d;
extern pont_default pont_d;
extern int **peut_manger;
extern clr *tab_clr;
extern int TAILLE;
extern int Nb_objet;
extern int Nb_vivant;
extern int **graphe;
extern int nb_affiche_graphe;
extern int Nb_joueur;
extern int taille_cadre_X;
extern int taille_cadre_Y;
extern pecheurs *joueurs;
extern int fils;
int Avec_musique;
Mix_Music *musique;
/*
Si une erreur se situe dans le fichier de config,on affiche la ligne
*/
void afficher_erreur_ligne(int i){
	printf("ERREUR : ligne %d de configuration.txt\n",i);
	kill(fils,SIGKILL);
	exit(EXIT_FAILURE);
}
/*
Chargement de l'écosysteme
*/
void charger_ecosystem(){
	int i,j,randX,randY,nombre_case=(TAILLE-1)*(TAILLE-1);
	srand (time (NULL));
	
/*
Calcule le nombre d'animal par espece qui seront présent dans la grille
	for(i=0;i<Nb_vivant;i++){
		espece_d[i].nombre_espece=espece_d[i].ratio*(nombre_case)/100;
	}
	Puis les place aléatoirement
	for(i=0;i<Nb_vivant;i++){
		j=espece_d[i].nombre_espece;
		while(j>0){
			randX=rand()%(TAILLE-2)+1;
			randY=rand()%(TAILLE-2)+1;
			if(grille[randX][randY].est_vivant!=1){
				grille[randX][randY]=convertion_animal_to_objet(espece_d[i]);
				j--;
			}
		}
	}
*/	
	/*
	On crée les bords de la carte
	*/
	for(i=0;i<TAILLE;i++){
		grille[i][TAILLE-1]=convertion_pont_to_objet();
		grille[i][TAILLE-1].soliditee=0;
		grille[i][0]=convertion_pont_to_objet();
		grille[i][0].soliditee=0;
		grille[0][i]=convertion_pont_to_objet();
		grille[0][i].soliditee=0;
		grille[TAILLE-1][i]=convertion_pont_to_objet();
		grille[TAILLE-1][i].soliditee=0;
	}
	/*
	Puis on fait spawner les joueurs
	*/
	for(i=1;i<=Nb_joueur;i++)
		spawn_rand_pecheur(i);
}

void charger_config(){
	
	int i=-1,j,nombre,ratiototal=0,nb_ligne=0;
	FILE* fichier = NULL;
	clr new;
	espece_default e;
    char chaine[100] = "";
    char chaine2[100] = "";
   
    fichier = fopen("configuration.txt", "r");
    /*
    Le fichier existe-t-il ?
    */
    if(fichier==NULL){
    	/*
		Si il n'existe pas, on le crée puis on le rempli
    	*/
    	fichier=fopen("configuration.txt", "w");
    	if(fichier==NULL){
    		printf("Impossible ouvrir fichier configuration.txt\n");
    		kill(fils,SIGKILL);
    		exit(EXIT_FAILURE);
    	}
    	else
    		printf("Création du fichier configuration.txt\n");
    	fprintf(fichier,"Configuration\n\n");
		fprintf(fichier,"Avec_musique= 1\n");
		fprintf(fichier,"Resolution= 900 * 900\n");
		fprintf(fichier,"Nombre_tour_affiche_graphe= 500\n");
		fprintf(fichier,"TAILLE= 50\n\n");
		fprintf(fichier,"Objets\n\n");
		fprintf(fichier,"Autre: Pecheur\n");
		fprintf(fichier,"Numero= 0\n");
		fprintf(fichier,"taille_canne_a_peche= 12\n");
		fprintf(fichier,"distance_peche_filet= 10\n");
		fprintf(fichier,"debloque_filet= 40\n");
		fprintf(fichier,"nb_depl_par_tour= 3\n");
		fprintf(fichier,"taille= 2\n");
		fprintf(fichier,"--------------------------\n");
		fprintf(fichier,"Autre: Pont\n");
		fprintf(fichier,"Numero= 1\n");
		fprintf(fichier,"soliditee= 20\n");
		fprintf(fichier,"Taille= 1\n");
		fprintf(fichier,"--------------------------\n");
		fprintf(fichier,"Vie: Plancton\n");
		fprintf(fichier,"Numero= 2\n");
		fprintf(fichier,"duree_survie= 0\n");
		fprintf(fichier,"taille_du_bide= 4\n");
		fprintf(fichier,"taille= 2\n");
		fprintf(fichier,"saut_max= 0\n");
		fprintf(fichier,"metabolisme= 0\n");
		fprintf(fichier,"gestation= 0\n");
		fprintf(fichier,"ratio= 40\n");
		fprintf(fichier,"frequence_reproduction= 1\n");
		fprintf(fichier,"--------------------------\n");
		fprintf(fichier,"Vie: Corail\n");
		fprintf(fichier,"Numero= 3\n");
		fprintf(fichier,"duree_survie= 14\n");
		fprintf(fichier,"taille_du_bide= 2\n");
		fprintf(fichier,"taille= 2\n");
		fprintf(fichier,"saut_max= 0\n");
		fprintf(fichier,"metabolisme= 1\n");
		fprintf(fichier,"gestation= 1\n");
		fprintf(fichier,"ratio= 4\n");
		fprintf(fichier,"frequence_reproduction= 35\n");
		fprintf(fichier,"--------------------------\n");
		fprintf(fichier,"Vie: Bar\n");
		fprintf(fichier,"Numero= 4\n");
		fprintf(fichier,"duree_survie= 23\n");
		fprintf(fichier,"taille_du_bide= 2\n");
		fprintf(fichier,"taille= 3\n");
		fprintf(fichier,"saut_max= 3\n");
		fprintf(fichier,"metabolisme= 1\n");
		fprintf(fichier,"gestation= 1\n");
		fprintf(fichier,"ratio= 10\n");
		fprintf(fichier,"frequence_reproduction= 2\n");
		fprintf(fichier,"--------------------------\n");
		fprintf(fichier,"Vie: Thon\n");
		fprintf(fichier,"Numero= 5\n");
		fprintf(fichier,"duree_survie= 26\n");
		fprintf(fichier,"taille_du_bide= 2\n");
		fprintf(fichier,"taille= 4\n");
		fprintf(fichier,"saut_max= 3\n");
		fprintf(fichier,"metabolisme= 1\n");
		fprintf(fichier,"gestation= 1\n");
		fprintf(fichier,"ratio= 10\n");
		fprintf(fichier,"frequence_reproduction= 11\n");
		fprintf(fichier,"--------------------------\n");
		fprintf(fichier,"Vie: Pollution\n");
		fprintf(fichier,"Numero= 6\n");
		fprintf(fichier,"duree_survie= 0\n");
		fprintf(fichier,"taille_du_bide= 3\n");
		fprintf(fichier,"taille= 6\n");
		fprintf(fichier,"saut_max= 3\n");
		fprintf(fichier,"metabolisme= 1\n");
		fprintf(fichier,"gestation= 0\n");
		fprintf(fichier,"ratio= 1\n");
		fprintf(fichier,"frequence_reproduction= 0\n");
		fprintf(fichier,"--------------------------\n");
		fprintf(fichier,"Vie: Pyranha\n");
		fprintf(fichier,"Numero= 7\n");
		fprintf(fichier,"duree_survie= 37\n");
		fprintf(fichier,"taille_du_bide= 4\n");
		fprintf(fichier,"taille= 5\n");
		fprintf(fichier,"saut_max= 3\n");
		fprintf(fichier,"metabolisme= 1\n");
		fprintf(fichier,"gestation= 1\n");
		fprintf(fichier,"ratio= 2\n");
		fprintf(fichier,"frequence_reproduction= 40\n");
		fprintf(fichier,"--------------------------\n");
		fprintf(fichier,"Vie: Requin\n");
		fprintf(fichier,"Numero= 8\n");
		fprintf(fichier,"duree_survie= 90\n");
		fprintf(fichier,"taille_du_bide= 3\n");
		fprintf(fichier,"taille= 6\n");
		fprintf(fichier,"saut_max= 3\n");
		fprintf(fichier,"metabolisme= 1\n");
		fprintf(fichier,"gestation= 1\n");
		fprintf(fichier,"ratio= 2\n");
		fprintf(fichier,"frequence_reproduction= 16\n");
		fprintf(fichier,"--------------------------\n");
		fprintf(fichier,"Vie: Orque\n");
		fprintf(fichier,"Numero= 9\n");
		fprintf(fichier,"duree_survie= 190\n");
		fprintf(fichier,"taille_du_bide= 6\n");
		fprintf(fichier,"taille= 7\n");
		fprintf(fichier,"saut_max= 3\n");
		fprintf(fichier,"metabolisme= 1\n");
		fprintf(fichier,"gestation= 1\n");
		fprintf(fichier,"ratio= 1\n");
		fprintf(fichier,"frequence_reproduction= 21\n");
		fprintf(fichier,"--------------------------\n");
		fprintf(fichier,"Vie: Baleine\n");
		fprintf(fichier,"Numero= 10\n");
		fprintf(fichier,"duree_survie= 2\n");
		fprintf(fichier,"taille_du_bide= 2\n");
		fprintf(fichier,"taille= 16\n");
		fprintf(fichier,"saut_max= 3\n");
		fprintf(fichier,"metabolisme= 2\n");
		fprintf(fichier,"gestation= 1\n");
		fprintf(fichier,"ratio= 3\n");
		fprintf(fichier,"frequence_reproduction= 160\n");
		fprintf(fichier,"--------------------------\n\n");
		fprintf(fichier,"FIN FIN\n\n");
		fprintf(fichier,"0 = 4 & 5 & 7 & 8 & 9 & 10 /\n");
		fprintf(fichier,"1 = -1 /\n");
		fprintf(fichier,"2 = -1 /\n");
		fprintf(fichier,"3 = 2 /\n");
		fprintf(fichier,"4 = 2 & 1 /\n");
		fprintf(fichier,"5 = 2 /\n");
		fprintf(fichier,"6 = 3 /\n");
		fprintf(fichier,"7 = 4 & 5 & 0 /\n");
		fprintf(fichier,"8 = 4 & 0 /\n");
		fprintf(fichier,"9 = 8 & 0 /\n");
		fprintf(fichier,"10 = 2 & 1 /\n\n");
		fprintf(fichier,"0_couleur 0 = 0 0 0\n");
		fprintf(fichier,"1_couleur 1 = 102 51 0\n");
		fprintf(fichier,"2_couleur 2 = 121 231 102\n");
		fprintf(fichier,"3_couleur 3 = 219 90 90\n");
		fprintf(fichier,"4_couleur 4 = 138 125 117\n");
		fprintf(fichier,"5_couleur 5 = 205 17 17\n");
		fprintf(fichier,"6_couleur 6 = 255 255 255\n");
		fprintf(fichier,"7_couleur 7 = 255 182 0\n");
		fprintf(fichier,"8_couleur 8 = 2 136 252\n");
		fprintf(fichier,"9_couleur 9 = 124 90 164\n");
		fprintf(fichier,"10_couleur 10 = 98 3 252\n");
		fprintf(fichier,"11_couleur fond_graphe = 0 126 169\n");
		fprintf(fichier,"12_couleur fond_legende = 0 126 169\n");
		fprintf(fichier,"13_couleur mer = 0 0 255\n");
		fprintf(fichier,"14_couleur fond_stat = 0 126 169\n");
		fprintf(fichier,"15_couleur contour = 0 0 0\n");
		fprintf(fichier,"16_couleur num_joueur = 255 255 255\n");
		fprintf(fichier,"17_couleur texte = 0 0 0\n");
		fprintf(fichier,"18_couleur texte_choisi = 255 255 255\n");
		fprintf(fichier,"19_couleur axe_graphe = 255 255 255\n");
		fprintf(fichier,"20_couleur fond_sac = 0 126 169\n");
		fprintf(fichier,"21_couleur ombres = 105 105 105\n");
		fclose(fichier);
		fichier=fopen("configuration.txt", "r");
    }
    /*
	Puis on lit les informations qui y sont présentes
    */
 	espece_d=malloc(sizeof(espece_default));
 	assert(espece_d!=NULL);
    if (fichier != NULL)
    {
    	if(fscanf(fichier,"Configuration\n\n"))
    		afficher_erreur_ligne(nb_ligne);
    	nb_ligne+=3;
    	if(!fscanf(fichier,"Avec_musique= %d\n",&Avec_musique))
    		afficher_erreur_ligne(nb_ligne);
    	nb_ligne++;
    	if(!fscanf(fichier,"Resolution= %d * %d\n",&taille_cadre_X,&taille_cadre_Y))
    		afficher_erreur_ligne(nb_ligne);
    	nb_ligne++;
    	if(!fscanf(fichier,"Nombre_tour_affiche_graphe= %d\n",&nb_affiche_graphe))
    		afficher_erreur_ligne(nb_ligne);
    	nb_ligne++;
    	if(Nb_joueur==0){
			if(!fscanf(fichier,"TAILLE= %d\n",&nombre))
    			afficher_erreur_ligne(nb_ligne);
    	}
		else{
			if(!fscanf(fichier,"TAILLE= %d\n",&TAILLE))
    			afficher_erreur_ligne(nb_ligne);
		}
		if(fscanf(fichier,"\nObjets\n"))
    			afficher_erreur_ligne(nb_ligne);
		nb_ligne+=4;
    	if(!fscanf(fichier,"%s %s\n",chaine,chaine2))
    			afficher_erreur_ligne(nb_ligne);
    	nb_ligne++;
        while(strcmp(chaine,"FIN")){
        	if(!strcmp(chaine,"Vie:")){
        		i++;
        		Nb_vivant++;
        		Nb_objet++;
        		espece_d=realloc(espece_d,sizeof(espece_default)*Nb_vivant);
        		assert(espece_d!=NULL);
	        	strcpy(e.nom,chaine2);
				if(!fscanf(fichier,"Numero= %d\n",&nombre))
    				afficher_erreur_ligne(nb_ligne);
    			nb_ligne++;
				e.numero=nombre;
				if(!fscanf(fichier,"duree_survie= %d\n",&nombre))
    				afficher_erreur_ligne(nb_ligne);
    			nb_ligne++;
				e.duree_survie=nombre;
				if(!fscanf(fichier,"taille_du_bide= %d\n",&nombre))
    				afficher_erreur_ligne(nb_ligne);
    			nb_ligne++;
				e.taille_du_bide=nombre;
				if(!fscanf(fichier,"taille= %d\n",&nombre))
    				afficher_erreur_ligne(nb_ligne);
    			nb_ligne++;
				e.taille=nombre;
				if(!fscanf(fichier,"saut_max= %d\n",&nombre))
    				afficher_erreur_ligne(nb_ligne);
    			nb_ligne++;
				e.saut_max=nombre;
				if(!fscanf(fichier,"metabolisme= %d\n",&nombre))
    				afficher_erreur_ligne(nb_ligne);
    			nb_ligne++;
				e.metabolisme=nombre;
				if(!fscanf(fichier,"gestation= %d\n",&nombre))
    				afficher_erreur_ligne(nb_ligne);
    			nb_ligne++;
				e.gestation=nombre;
				if(!fscanf(fichier,"ratio= %d\n",&nombre))
    				afficher_erreur_ligne(nb_ligne);
    			nb_ligne++;
				e.ratio=nombre;
				ratiototal+=nombre;
				if(!fscanf(fichier,"frequence_reproduction= %d\n",&nombre))
    				afficher_erreur_ligne(nb_ligne);
    			nb_ligne++;
				e.frequence_reproduction=nombre;
				espece_d[i]=e;
			}
			else{
				if(!strcmp(chaine2,"Pecheur")){
					Nb_objet++;
					if(!fscanf(fichier,"Numero= %d\n",&nombre))
    					afficher_erreur_ligne(nb_ligne);
    				nb_ligne++;
					joueur_d.numero=nombre;
					if(!fscanf(fichier,"taille_canne_a_peche= %d\n",&nombre))
    					afficher_erreur_ligne(nb_ligne);
    				nb_ligne++;
					joueur_d.taille_canne_a_peche=nombre;
					if(!fscanf(fichier,"distance_peche_filet= %d\n",&nombre))
    					afficher_erreur_ligne(nb_ligne);
    				nb_ligne++;
					joueur_d.distance_peche_filet=nombre;
					if(!fscanf(fichier,"debloque_filet= %d\n",&nombre))
    					afficher_erreur_ligne(nb_ligne);
    				nb_ligne++;
					joueur_d.debloque_filet=nombre;
					if(!fscanf(fichier,"nb_depl_par_tour= %d\n",&nombre))
    					afficher_erreur_ligne(nb_ligne);
    				nb_ligne++;
					joueur_d.nb_depl_par_tour=nombre;
					if(!fscanf(fichier,"taille= %d\n",&nombre))
    					afficher_erreur_ligne(nb_ligne);
    				nb_ligne++;
					joueur_d.taille=nombre;
				}
				else
					if(!strcmp(chaine2,"Pont")){
						Nb_objet++;
						if(!fscanf(fichier,"Numero= %d\n",&nombre))
    						afficher_erreur_ligne(nb_ligne);
    					nb_ligne++;
						pont_d.numero=nombre;
						if(!fscanf(fichier,"soliditee= %d\n",&nombre))
    						afficher_erreur_ligne(nb_ligne);
    					nb_ligne++;
						pont_d.soliditee=nombre;
						if(!fscanf(fichier,"Taille= %d\n",&nombre))
    						afficher_erreur_ligne(nb_ligne);
    					nb_ligne++;
						pont_d.taille=nombre;
					}
			}
			if(!fscanf(fichier,"%s",chaine))
    			afficher_erreur_ligne(nb_ligne);
    		nb_ligne++;
			if(!fscanf(fichier,"%s %s\n",chaine,chaine2))
    			afficher_erreur_ligne(nb_ligne);
    		nb_ligne++;
   		}
   		nb_ligne+=2;
   		peut_manger=calloc(Nb_objet,sizeof(int *));
   		assert(peut_manger!=NULL);
	    for(i=0;i<Nb_objet;i++){
			peut_manger[i]=calloc(Nb_objet,sizeof(int));
			assert(peut_manger[i]!=NULL);		
			if(!fscanf(fichier,"%d %c",&j,&chaine[0]))
    			afficher_erreur_ligne(nb_ligne);
			while(chaine[0]!='/'){
				if(!fscanf(fichier," %d %c",&nombre,&chaine[0]))
    				afficher_erreur_ligne(nb_ligne);
				if(nombre!=-1){
					peut_manger[j][nombre]=1;
				}
			}
			if(fscanf(fichier,"\n\n"))
				afficher_erreur_ligne(nb_ligne);
			nb_ligne++;
		}
		tab_clr=malloc(sizeof(clr)*(Nb_objet+11));
		assert(tab_clr!=NULL);
		for(i=0;i<Nb_objet;i++){
			if(!fscanf(fichier,"%d_couleur %d = %d %d %d\n",&j,&nombre,&new.r,&new.g,&new.b))
				afficher_erreur_ligne(nb_ligne);
			nb_ligne++;
			tab_clr[i]=new;
		}
		for(i=Nb_objet;i<Nb_objet+11;i++){
			if(!fscanf(fichier,"%d_couleur %s = %d %d %d\n",&j,chaine,&new.r,&new.g,&new.b))
				afficher_erreur_ligne(nb_ligne);
			nb_ligne++;
			tab_clr[i]=new;
		}
        fclose(fichier);
    }
    graphe=malloc(sizeof(int*)*(Nb_vivant+2));
    assert(graphe!=NULL);
	for(i=0;i<Nb_vivant+2;i++){
		graphe[i]=malloc(sizeof(int)*nb_affiche_graphe);
		assert(graphe[i]!=NULL);
		for(j=0;j<nb_affiche_graphe;j++)
			graphe[i][j]=-1;
	}
	grille=malloc(sizeof(objet*)*TAILLE);
	assert(grille!=NULL);
	for(i=0;i<TAILLE;i++){
		grille[i]=malloc(sizeof(objet)*TAILLE);
		assert(grille[i]!=NULL);
		for (j=0; j< TAILLE; j++)
			vide(&grille[i][j]);
	}
	/*
	Gestion de quelques erreur
	*/
	if(ratiototal>100){
		printf("Le ratio total est supérieur à 100 ! (%d)\n",ratiototal );
		exit(-1);
	}
	if(TAILLE>200){
		printf("TAILLE est trop élevé !(max 100)\n");
		exit(-1);
	}
	
}
/*
Reinitialise le jeu
*/
void reinitialise(){
	int i,j;
	for(i=0;i<Nb_vivant+2;i++)
		for(j=0;j<nb_affiche_graphe;j++)
			graphe[i][j]=-1;
	for(i=0;i<TAILLE;i++)
		for (j=0; j< TAILLE; j++)
			vide(&grille[i][j]);
}
/*
Libere la mémoire en fin de jeu
*/
void fin(){
	int i,max;
	max=TAILLE;
	if(Nb_objet>max)
		max=Nb_objet;
	for (i = max; i >=0; i--)
	{
		if(i<TAILLE)
			free(grille[i]);
		if(i<Nb_objet)
			free(peut_manger[i]);
		if(i<=Nb_vivant)
			free(graphe[i]);
		if(i<Nb_joueur)
			while(depile(i)!=1);
	}
	free(peut_manger);
	free(graphe);
	free(grille);
	free(espece_d);
	if(Nb_joueur>0)
		free(joueurs);
	free(tab_clr);
}
/*
Demande le nombre de joueurs qui il y aura dans la partie puis
initialise le tableau des joueurs avec leurs nom
*/
int nombre_de_joueur(){
	int mode,i;
	do{
		printf("Veuillez choisir le nombre de joueur [0-%d] : ",Limite_nb_joueur);
		if(!scanf("%d",&Nb_joueur))
			return -1;
	}while(Nb_joueur>Limite_nb_joueur || Nb_joueur<0);
	if(Nb_joueur==0){
		mode=Information;
		do{
			printf("Veuillez choisir la taille de la grille [10-200] : ");
			if(!scanf("%d",&TAILLE))
				return -1;
		}while(TAILLE>200 || TAILLE<10);
	}
	else
		mode=Jeux;
	if(Nb_joueur>0){
		joueurs=malloc(sizeof(pecheurs)*Nb_joueur);
		assert(joueurs!=NULL);
	}
	for(i=0;i<Nb_joueur;i++){
		printf("Joueur %d, Veuillez choisir votre pseudo :",i+1 );
		if(!scanf("%s",joueurs[i].nom))
			return -1;
		joueurs[i].nom[7]='\0';
	}
	return mode;
}
/*
Charge la musique
*/
void charger_musique_accueil(){
	if(Mix_OpenAudio(8000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		printf("%s", Mix_GetError());
	musique = Mix_LoadMUS("Musique/Accueil.mp3");
	Mix_PlayMusic(musique, -1);
}
void charger_musique_fond(){
	if(!Avec_musique)
		return;
	if(Mix_OpenAudio(8000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		printf("%s", Mix_GetError());
	musique = Mix_LoadMUS("Musique/Ma_musique.mp3");
	Mix_PlayMusic(musique, -1);
}
/*
Termine la musique
*/
void terminer_musique_accueil(){
		Mix_FreeMusic(musique);
		Mix_CloseAudio();
}
void terminer_musique_fond(){
	if(Avec_musique){
		Mix_FreeMusic(musique);
		Mix_CloseAudio();
	}
}
