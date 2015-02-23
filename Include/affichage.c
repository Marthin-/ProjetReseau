#include "Define.h"

extern objet **grille;
extern pecheurs *joueurs;
extern espece_default *espece_d;
extern pecheur_default joueur_d;
extern pont_default pont_d;
extern int **peut_manger;
extern int TAILLE;
extern clr *tab_clr;
extern int Nb_objet;
extern int Nb_vivant;
extern int Nb_tour;
extern int Nb_Joueur_tour;
extern int **graphe;
extern int nb_affiche_graphe;
extern int Nb_joueur;
extern int taille_cadre_X;
extern int taille_cadre_Y;
extern SDL_Surface *ecran;
extern int err_flag;
extern char erreur[5][20];
/*
Convertie un message séparé par des espace en plusieur string
"Bonjour je programme"=> "Bonjour" "Je" "Programme"
*/
void converti_msg_erreur(char *msg_erreur){
	int cpt_mess=0,cpt_lettre=0,i=0;
	while(msg_erreur[i]!='\0' && cpt_mess<5){
		/*
		Coupe le mot
		*/
		if(msg_erreur[i]==' '){
			erreur[cpt_mess][cpt_lettre]='\0';
			cpt_mess++;
			cpt_lettre=0;
		}
		else{
			erreur[cpt_mess][cpt_lettre++]=msg_erreur[i];
			/*
			Met la première lettre en majuscule
			*/
			if(cpt_lettre==1 && erreur[cpt_mess][0]<='z' && erreur[cpt_mess][0]>='a')
				erreur[cpt_mess][0]=erreur[cpt_mess][0]-'a'+'A';
		}
		i++;
	}
	erreur[cpt_mess+1][0]='\0';
}
/*
copie le message a afficher dans le string erreur et positionne
le flag a 1
*/
void afficher_message_erreur(char * msg){
	converti_msg_erreur(msg);
	err_flag=1;
}
/*
Charge la SDL avec son icone
*/
void creation_SDL(){
	SDL_Surface *image=IMG_Load("Images/icone.png");
    SDL_Init(SDL_INIT_VIDEO);
   	SDL_WM_SetIcon(image, NULL);
    ecran = SDL_SetVideoMode(taille_cadre_X, taille_cadre_Y, 32, SDL_HWSURFACE | SDL_RESIZABLE);
    SDL_WM_SetCaption("Jeu du lac", NULL);
    SDL_FreeSurface(image);
}
/*
Cette fonction ressemblea un printf,on doit lui spécifier les coordonnées,la taille, la couleur et le message
a afficher.
*/
static void Create_text (short x, short y,short font_size, int r,int g,int b, const char * p_message)
{
	TTF_Font      *   p_font      = NULL;
	SDL_Surface   *   s_text      = NULL;
	SDL_Rect          rect_dest;
	SDL_Color         color;
	if (TTF_Init () < 0)
	{
		printf("ERROR TTF\n");
		exit(EXIT_FAILURE);
		return;
	}
	color.r = r;
	color.g = g;
	color.b = b;
	p_font = TTF_OpenFont ("Polices/police1.ttf", font_size);
	if (! p_font)
	{
		printf("%s\n",TTF_GetError());
		exit(EXIT_FAILURE);
	}
	s_text = TTF_RenderText_Solid (p_font, p_message, color);
	if (s_text == NULL)
		exit(EXIT_FAILURE);
	else if (s_text != NULL){
		rect_dest.x = x;
		rect_dest.y = y;
		rect_dest.w = s_text->w;
		rect_dest.h = s_text->h;
		SDL_BlitSurface (s_text, NULL, ecran, & rect_dest);
		SDL_FreeSurface (s_text);
	}
	TTF_CloseFont (p_font);
	return;
}
/*
En mode information, on affiche la partie graphique en bas.
*/
void afficher_graphique_SDL(){
	int i,j,largeur_trait,ecart,deb;
	char str[9];
	SDL_Surface *contour;
	SDL_Surface *rectangle_graphe,*fond_graphe;
	SDL_Rect position;
	float x=0.0;
	largeur_trait=3;
	/*
	On regarde combien de tour séparera chaque trait qui montre le nombre de tour
	passé
	*/
	if(nb_affiche_graphe<=10)
		ecart=1;
	else if(nb_affiche_graphe<=100)
		ecart=10;
	else if(nb_affiche_graphe<=1000)
		ecart=100;
	else
		ecart=500;
	position.x=0;
	position.y=taille_cadre_Y/2+taille_cadre_Y/4;
	/*
	Charge le fond
	*/
	fond_graphe=SDL_CreateRGBSurface(SDL_HWSURFACE, taille_cadre_X, taille_cadre_Y/4, 32, 0, 0, 0, 0);
	SDL_FillRect(fond_graphe, NULL, SDL_MapRGB(ecran->format, tab_clr[Nb_objet].r, tab_clr[Nb_objet].g, tab_clr[Nb_objet].b));
	SDL_BlitSurface(fond_graphe, NULL, ecran, &position);
	rectangle_graphe = SDL_CreateRGBSurface(SDL_HWSURFACE, (float)taille_cadre_X/(float)nb_affiche_graphe, 2, 32, 0, 0, 0, 0);
	/*
	Pour optimiser le code,on choisi de commencer le défilement du graphique à partir de la 1er valeur trouvé dans
	le tableau, et ainsi on ne traite pas les case contenant -1 au début du jeu;
	*/
	if(Nb_tour<nb_affiche_graphe)
		deb=nb_affiche_graphe-Nb_tour-1;
	else
		deb=0;
	for(i=0;i<Nb_vivant;i++){
		x=4.0;
		for(j=deb;j<nb_affiche_graphe;j++){
			position.x=x;
			SDL_FillRect(rectangle_graphe, NULL, SDL_MapRGB(ecran->format, tab_clr[i+2].r, tab_clr[i+2].g, tab_clr[i+2].b));
			/*
			Affiche la valeur du tour sur l'axe des abscisse,avec un trait et le nombre
			*/
			if(graphe[Nb_vivant+1][j]%ecart==0 && graphe[Nb_vivant+1][j]){
				position.y=taille_cadre_Y/2+taille_cadre_Y/4+taille_cadre_Y/8-5*largeur_trait;
				contour=SDL_CreateRGBSurface(SDL_HWSURFACE,largeur_trait,5*largeur_trait, 32, 0, 0, 0, 0);
				SDL_FillRect(contour, NULL, SDL_MapRGB(ecran->format, tab_clr[Nb_objet+8].r, tab_clr[Nb_objet+8].g, tab_clr[Nb_objet+8].b));
				SDL_BlitSurface(contour, NULL, ecran, &position);
				sprintf(str,"%d",graphe[Nb_vivant+1][j]);
				Create_text(position.x-10,position.y-20,10,tab_clr[Nb_objet+7].r,tab_clr[Nb_objet+7].g,tab_clr[Nb_objet+7].b,str);
			}
			/*
			Affiche le point sur le graphique relatif à l'espece
			*/
	        position.y=(float)taille_cadre_Y-(float)graphe[i][j]/(float)graphe[Nb_vivant][j]*((float)taille_cadre_Y/4.0);
		    SDL_BlitSurface(rectangle_graphe, NULL, ecran, &position);
		    x+=(float)taille_cadre_X/(float)nb_affiche_graphe;
		}
	}
	/*
	Affiche l'axe des abscisses
	*/
	position.x=0;
	position.y=taille_cadre_Y/2+taille_cadre_Y/4+taille_cadre_Y/8;
	contour=SDL_CreateRGBSurface(SDL_HWSURFACE, taille_cadre_X,largeur_trait, 32, 0, 0, 0, 0);
	SDL_FillRect(contour, NULL, SDL_MapRGB(ecran->format, tab_clr[Nb_objet+8].r, tab_clr[Nb_objet+8].g, tab_clr[Nb_objet+8].b));
	SDL_BlitSurface(contour, NULL, ecran, &position);
    Create_text(position.x+10,position.y+1,20,tab_clr[Nb_objet+7].r,tab_clr[Nb_objet+7].g,tab_clr[Nb_objet+7].b,"50%");
	/*
	Affiche l'axe des ordonnées
	*/
	position.y=taille_cadre_Y/4*3;
	SDL_FreeSurface(contour);
	contour=SDL_CreateRGBSurface(SDL_HWSURFACE, largeur_trait,taille_cadre_Y/4, 32, 0, 0, 0, 0);
	SDL_FillRect(contour, NULL, SDL_MapRGB(ecran->format, tab_clr[Nb_objet+8].r, tab_clr[Nb_objet+8].g, tab_clr[Nb_objet+8].b));
	SDL_BlitSurface(contour, NULL, ecran, &position);
	/*
	Libere les surfaces
	*/
	SDL_FreeSurface(contour);
	SDL_FreeSurface(rectangle_graphe);
	SDL_FreeSurface(fond_graphe);
}
void afficher_grille_SDL(int mode){
	char str[80];
	float taille_grille_X,taille_grille_Y;
	int X,Y,taille_police;
	SDL_Rect position;
	SDL_Surface *rectangle,*mer;
	/*
	Convertie la taille de la fentre SDL en taille pour la grille
	*/
	taille_grille_X = (float)(taille_cadre_X)/(float)TAILLE/4*3;
	taille_grille_Y = (float)(taille_cadre_Y)/(float)TAILLE/4*3;
	taille_police=taille_grille_Y-2;
	rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, taille_grille_X-1, taille_grille_Y-1, 32, 0, 0, 0, 0);
	position.x= 0;
	position.y = 0;
	/*
	Charge l'image de mer si on est pas en mode information puis la libere
	*/
	if(mode!=Information){
		mer = IMG_Load("Images/mer_converted.jpg");
    	SDL_BlitSurface(mer, NULL, ecran, &position);
    	SDL_FreeSurface(mer);
    }
    /*
	Affiche chaque case de la grille sur la fenetre SDL.
	La couleur de l'espece est défini dans le tableau tab_clr initialisé
	au début.
	Si on affiche une case joueur,on affiche son numéro
    */
	for(X=0; X<TAILLE; X++)
	{
	    for(Y=0; Y<TAILLE; Y++)
	    {
	    	position.x = (X*taille_grille_X);
		    position.y = (Y*taille_grille_Y);
			if(grille[X][Y].est_vivant && mode==Information)
				SDL_FillRect(rectangle, NULL,  SDL_MapRGB(ecran->format, tab_clr[grille[X][Y].nom].r, tab_clr[grille[X][Y].nom].g, tab_clr[grille[X][Y].nom].b));
			else if(grille[X][Y].est_vivant)
				SDL_FillRect(rectangle, NULL,  SDL_MapRGB(ecran->format, tab_clr[Nb_objet+10].r, tab_clr[Nb_objet+10].g, tab_clr[Nb_objet+10].b));
			else if(grille[X][Y].est_pont && !grille[X][Y].est_pecheur)
		    	SDL_FillRect(rectangle, NULL,  SDL_MapRGB(ecran->format, tab_clr[grille[X][Y].nom].r, tab_clr[grille[X][Y].nom].g, tab_clr[grille[X][Y].nom].b));
		    else if(grille[X][Y].est_pecheur && grille[X][Y].J_numero>=1 && !joueurs[grille[X][Y].J_numero-1].nage)
		    	SDL_FillRect(rectangle, NULL,  SDL_MapRGB(ecran->format, tab_clr[grille[X][Y].nom].r, tab_clr[grille[X][Y].nom].g, tab_clr[grille[X][Y].nom].b));
		    else if(grille[X][Y].est_pecheur && grille[X][Y].J_numero>=1)
		    	SDL_FillRect(rectangle, NULL,  SDL_MapRGB(ecran->format, tab_clr[Nb_objet+2].r, tab_clr[Nb_objet+2].g, tab_clr[Nb_objet+2].b));
		    if((!grille[X][Y].est_vide && mode!=Information && grille[X][Y].nom<2)||(!grille[X][Y].est_vide)){
		    	SDL_BlitSurface(rectangle, NULL, ecran, &position);
		    }
		    if(grille[X][Y].est_pecheur && grille[X][Y].J_numero>0){
		    	sprintf(str, "%d", grille[X][Y].J_numero);
		    	Create_text(position.x+taille_grille_X/5,position.y-2,taille_police,tab_clr[Nb_objet+7].r,tab_clr[Nb_objet+7].b,tab_clr[Nb_objet+7].b,str);
		    }
	    }
	}
	/*
	Libere la surface
	*/
	SDL_FreeSurface(rectangle);
}
/*
Affiche les contours de chaque partie de l'affichage puis libere la surface contour
*/
void afficher_contour_SDL(){
	int largeur_trait;
	SDL_Surface *contour;
	SDL_Rect position;
	largeur_trait=2;
	/*
	Affiche la barre horizontale
	*/
	position.x=0;
	position.y=taille_cadre_Y/4*3;
	contour=SDL_CreateRGBSurface(SDL_HWSURFACE, taille_cadre_X,largeur_trait, 32, 0, 0, 0, 0);
	SDL_FillRect(contour, NULL, SDL_MapRGB(ecran->format, tab_clr[Nb_objet+4].r, tab_clr[Nb_objet+4].g, tab_clr[Nb_objet+4].b));
	SDL_BlitSurface(contour, NULL, ecran, &position);
	SDL_FreeSurface(contour);
	/*
	Affiche la barre vertical
	*/
	position.x=taille_cadre_X/4*3-largeur_trait;
	position.y=0;
	contour=SDL_CreateRGBSurface(SDL_HWSURFACE, largeur_trait,taille_cadre_Y/4*3, 32, 0, 0, 0, 0);
	SDL_FillRect(contour, NULL, SDL_MapRGB(ecran->format, tab_clr[Nb_objet+4].r, tab_clr[Nb_objet+4].g, tab_clr[Nb_objet+4].b));
	SDL_BlitSurface(contour, NULL, ecran, &position);
	SDL_FreeSurface(contour);
}
/*
Affiche le numéro du tour, le joueur qui doit jouer,un message d'erreur si besoin et la legende des couleurs
de la grille.
*/
void afficher_legende_SDL(int mode,int Joueur_courant){
	int k,tour;
	int taille_police;
	SDL_Rect position;
	SDL_Surface *Info=NULL;
    SDL_Surface *rectangle_info;
	char str[80];
	if(mode!=Information)
		tour=Nb_Joueur_tour;
	else
		tour=Nb_tour;
	taille_police=32*taille_cadre_Y/1600;
	position.x=taille_cadre_X/4*3;
	position.y=0;
	/*
	Affiche l'entete de la partie legende
	*/
	Info=SDL_CreateRGBSurface(SDL_HWSURFACE, taille_cadre_X/4, taille_cadre_Y/4*3, 32, 0, 0, 0, 0);
	SDL_FillRect(Info, NULL, SDL_MapRGB(ecran->format, tab_clr[Nb_objet+1].r, tab_clr[Nb_objet+1].g, tab_clr[Nb_objet+1].b));
	SDL_BlitSurface(Info, NULL, ecran, &position);
	if(mode!=Information){
		sprintf(str, "Tour : %d", tour);
		Create_text(taille_cadre_X/4*3+10,0,taille_police,tab_clr[Nb_objet+6].r,tab_clr[Nb_objet+6].b,tab_clr[Nb_objet+6].b,str);
		sprintf(str, "(Joueur %d)",Joueur_courant);
		Create_text(taille_cadre_X/4*3+10,40,taille_police,tab_clr[Nb_objet+6].r,tab_clr[Nb_objet+6].b,tab_clr[Nb_objet+6].b,str);
		if(err_flag){
			for(k=0;erreur[k][0]!='\0';k++){
				sprintf(str, "/!\\%s/!\\",erreur[k]);
				bzero(erreur[k], 20);
				Create_text(taille_cadre_X/4*3+10,80+k*40,taille_police-2,tab_clr[Nb_objet+7].r,tab_clr[Nb_objet+7].b,tab_clr[Nb_objet+7].b,str);
			}
			err_flag=0;
		}
	}
	else{
		sprintf(str, "Tour : %d", tour);
		Create_text(taille_cadre_X/4*3+10,0,taille_police,tab_clr[Nb_objet+6].r,tab_clr[Nb_objet+6].b,tab_clr[Nb_objet+6].b,str);
	}
	/*
	Affiche la legende des couleurs
	*/
	rectangle_info = SDL_CreateRGBSurface(SDL_HWSURFACE, taille_police, taille_police, 32, 0, 0, 0, 0);
	for(k=0;k<Nb_objet;k++){
		if(Nb_joueur==0 && k==0)
	    	k+=2;
	    /*
		Affiche les carrés de couleur
	    */
	    if(k>1){
	    	position.x=taille_cadre_X/4*3+5;
		    position.y=taille_cadre_Y*3/4-30-k*taille_cadre_Y/25;
		    SDL_FillRect(rectangle_info, NULL, SDL_MapRGB(ecran->format, tab_clr[espece_d[k-2].numero].r, tab_clr[espece_d[k-2].numero].g, tab_clr[espece_d[k-2].numero].b));
	        sprintf(str, "%s %d",espece_d[k-2].nom,graphe[k-2][nb_affiche_graphe-1]);
	    }
	    else{
	    	position.x=taille_cadre_X/4*3+5;
		    position.y=taille_cadre_Y*3/4-30-(Nb_objet+k)*taille_cadre_Y/25;
		    if(k==PONT){
	    		SDL_FillRect(rectangle_info, NULL, SDL_MapRGB(ecran->format, tab_clr[1].r, tab_clr[1].g, tab_clr[1].b));
	    		strcpy(str,"Pont");
		    }
		    else if(k==PECHEUR){
		    	SDL_FillRect(rectangle_info, NULL, SDL_MapRGB(ecran->format, tab_clr[0].r, tab_clr[0].g, tab_clr[0].b));
		    	strcpy(str,"Joueur");
		    }
	    }
	    /*
		Affiche ce que la couleur représente
	    */
	   	if((mode!=Information && k<2 && Nb_joueur>0)||(mode==Information && Nb_joueur>=0)){
		    SDL_BlitSurface(rectangle_info, NULL, ecran, &position);
		    if((k>1 && graphe[k-2][nb_affiche_graphe-1]>0)||k<=1)
		    	Create_text(position.x+50,position.y,taille_police-5,tab_clr[Nb_objet+6].r,tab_clr[Nb_objet+6].b,tab_clr[Nb_objet+6].b,str);
		    else
		    	Create_text(position.x+50,position.y,taille_police-5,tab_clr[Nb_objet+7].r,tab_clr[Nb_objet+7].b,tab_clr[Nb_objet+7].b,str);

		}
		
	}
	/*
	Libere les surfaces
	*/
	SDL_FreeSurface(Info);
	SDL_FreeSurface(rectangle_info);
}
/*
Affiche les statistiques des joueurs
*/
void afficher_stat_SDL(int Joueur_courant){
	int i,taille_police;
	clr newclr;
	float espace;
	char str[80];
	SDL_Rect position;
	SDL_Surface *fond_stat;
	/*
	Défini la taille de la police
	*/
	taille_police=32*taille_cadre_Y/1600;
	position.x=0;
	position.y=taille_cadre_Y/2+taille_cadre_Y/4;
	/*
	Change la couleur du fond
	*/
	fond_stat=SDL_CreateRGBSurface(SDL_HWSURFACE, taille_cadre_X, taille_cadre_Y/4, 32, 0, 0, 0, 0);
	SDL_FillRect(fond_stat, NULL, SDL_MapRGB(ecran->format, tab_clr[Nb_objet+3].r, tab_clr[Nb_objet+3].g, tab_clr[Nb_objet+3].b));
	SDL_BlitSurface(fond_stat, NULL, ecran, &position);
	espace=(float)taille_cadre_X/(float)Nb_joueur;
	if(Nb_joueur>1)
		position.x=5;
	else
		position.x=(float)taille_cadre_X/2.0-50;
	/*
	Affiche les statistiques un à un
	*/
	for(i=0;i<Nb_joueur;i++){
		if(i+1==Joueur_courant)
			newclr=tab_clr[Nb_objet+7];
		else
			newclr=tab_clr[Nb_objet+6];
		sprintf(str,"%s(J%d)",joueurs[i].nom,i+1);
		Create_text(position.x,taille_cadre_Y/2+taille_cadre_Y/4+5,taille_police,newclr.r,newclr.g,newclr.b,str);
		if(joueurs[i].nage)
			strcpy(str,"_NAGE__");
		else
			strcpy(str,"_______");
		Create_text(position.x,taille_cadre_Y/2+taille_cadre_Y/4+taille_cadre_Y/25,taille_police-5,newclr.r,newclr.g,newclr.b,str);
		sprintf(str,"Sac = %d ",joueurs[i].sac);
		Create_text(position.x,taille_cadre_Y/2+taille_cadre_Y/4+taille_cadre_Y/25*2,taille_police-5,newclr.r,newclr.g,newclr.b,str);
		sprintf(str,"XP = %d",joueurs[i].XP);
		Create_text(position.x,taille_cadre_Y/2+taille_cadre_Y/4+taille_cadre_Y/25*3,taille_police-5,newclr.r,newclr.g,newclr.b,str);
		sprintf(str,"X = %d",joueurs[i].X);
		Create_text(position.x,taille_cadre_Y/2+taille_cadre_Y/4+taille_cadre_Y/25*4,taille_police-5,newclr.r,newclr.g,newclr.b,str);
		sprintf(str,"Y = %d",joueurs[i].Y);
		Create_text(position.x,taille_cadre_Y/2+taille_cadre_Y/4+taille_cadre_Y/25*5,taille_police-5,newclr.r,newclr.g,newclr.b,str);
		position.x+=espace;
	}
	SDL_FreeSurface(fond_stat);
}
/*
Permet de compter le nombre d'element dans le sac du joueur
*/
int somme(pile *p){
	if(p==NULL)
		return 0;
	else
		return 1+somme(p->suivant);
}
/*
Affiche le contenu du sac du joueur courant
*/
void afficher_sac_SDL(int Joueur_courant){
	int i,taille_police;
	clr newclr;
	char str[80];
	pile *tmp;
	SDL_Rect position;
	SDL_Surface *fond_sac;
	/*
	Choisi la couleur du texte
	*/
	newclr=tab_clr[Nb_objet+6];
	/*
	Choisi la taille de la police
	*/
	taille_police=32*taille_cadre_Y/1600-5;
	position.x=0;
	position.y=taille_cadre_Y/2+taille_cadre_Y/4;
	fond_sac=SDL_CreateRGBSurface(SDL_HWSURFACE, taille_cadre_X, taille_cadre_Y/4, 32, 0, 0, 0, 0);
	SDL_FillRect(fond_sac, NULL, SDL_MapRGB(ecran->format, tab_clr[Nb_objet+9].r, tab_clr[Nb_objet+9].g, tab_clr[Nb_objet+9].b));
	SDL_BlitSurface(fond_sac, NULL, ecran, &position);
	tmp=joueurs[Joueur_courant-1].sac_contient;
	sprintf(str,"%s(J%d)",joueurs[Joueur_courant-1].nom,Joueur_courant);
	Create_text(position.x,taille_cadre_Y/2+taille_cadre_Y/4+5,taille_police,newclr.r,newclr.g,newclr.b,str);
	i=joueurs[Joueur_courant-1].new_poisson;
	/*
	Affiche les poissons péchés
	*/
	if(i>1)
		sprintf(str,"Vous avez peche %d poissons : ",i);
	else if(i==1)
		sprintf(str,"Vous avez peche 1 poisson : ");
	else
		sprintf(str,"Vous n'avez rien peche");
	Create_text(position.x,taille_cadre_Y/2+taille_cadre_Y/4+5+2*taille_police,taille_police,newclr.r,newclr.g,newclr.b,str);
	i--;
	tmp=joueurs[Joueur_courant-1].sac_contient;
	while(i>=0){
		sprintf(str,"%s(%d %%)",espece_d[tmp->numero-2].nom,tmp->reste*100/espece_d[tmp->numero-2].taille);
		if(i<5)
			position.y=taille_cadre_Y/2+taille_cadre_Y/4+3*taille_police+15;
		else
			position.y=taille_cadre_Y/2+taille_cadre_Y/4+4*taille_police+30;
		Create_text(position.x+(i%5)*(taille_cadre_Y/5),position.y,taille_police,newclr.r,newclr.g,newclr.b,str);
		tmp=tmp->suivant;
		i--;
	}
	/*
	Affiche les 15 1er poissons que possède le joueur courant avec le pourcentage de ce qu'il en reste
	*/
	tmp=joueurs[Joueur_courant-1].sac_contient;
	i=0;
	if(tmp==NULL)
		sprintf(str,"Votre sac est vide");
	else if(tmp->suivant==NULL)
		sprintf(str,"Votre sac contient 1 poisson : ");
	else
		sprintf(str,"Votre sac contient %d poissons:",somme(tmp));
	Create_text(position.x,taille_cadre_Y/2+taille_cadre_Y/4+8*taille_police+15,taille_police,newclr.r,newclr.g,newclr.b,str);
	while(tmp!=NULL){
		sprintf(str,"%s(%d %%)",espece_d[tmp->numero-2].nom,tmp->reste*100/espece_d[tmp->numero-2].taille);
		if(i<5)
			position.y=taille_cadre_Y/2+taille_cadre_Y/4+9*taille_police+30;
		else if(i<10)
			position.y=taille_cadre_Y/2+taille_cadre_Y/4+10*taille_police+45;
		else if(i<15)
			position.y=taille_cadre_Y/2+taille_cadre_Y/4+11*taille_police+60;
		else break;
		if(i<15)
			Create_text(position.x+(i%5)*(taille_cadre_Y/5),position.y,taille_police,newclr.r,newclr.g,newclr.b,str);
		tmp=tmp->suivant;
		i++;
	}
	/*
	Libere la surface
	*/
	SDL_FreeSurface(fond_sac);
}
/*
Blit chaque partie de l'affichage SDL sur l'écran , un par un puis affiche l'ecran
*/
void afficher_SDL(int mode,int Joueur_courant){
	/*
	Rempli la mer en bleu si on est en mode information
	*/
	if(mode==Information)
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,tab_clr[Nb_objet+2].r, tab_clr[Nb_objet+2].g, tab_clr[Nb_objet+2].b));
	
	/*
	Met la grille sur l'écran
	*/
	afficher_grille_SDL(mode);
	/*
	Met la legende sur l'écran
	*/
	afficher_legende_SDL(mode,Joueur_courant);
	/*
	Met soit le graphique, soit les statistiques des joueurs, soit le sac des joueurs sur l'écran(Selon le mode)
	*/
	if(mode==Information)
		afficher_graphique_SDL();
	else if(mode==Jeux)
		afficher_stat_SDL(Joueur_courant);
	else
		afficher_sac_SDL(Joueur_courant);
	/*
	Met les contours sur l'écran
	*/
	afficher_contour_SDL();
	/*
	Affiche l'écran
	*/
	SDL_Flip(ecran);
    TTF_Quit();
}
/*
Affiche le gagnant avec un effet de couleur.La taille est défini a chaque itération afin
d'éviter que l'utilisateur puisse redimmensionner la fenetre
*/
void afficher_gagnant_SDL(int j_gagner){
	int i;
	char str[80];
	sprintf(str,"%s [J%d] a gagnee !",joueurs[j_gagner-1].nom,j_gagner);
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,255, 255, 255));
	for(i=0;i<=255;i++){
		taille_cadre_X=800;
   		taille_cadre_Y=640;
		ecran = SDL_SetVideoMode(taille_cadre_X, taille_cadre_Y,32,SDL_HWSURFACE | SDL_RESIZABLE);
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,i, i, i));
		Create_text(taille_cadre_X/6,taille_cadre_Y/2,32,255-i,255-i,255-i,str);
		SDL_Flip(ecran);
		SDL_Delay(20);
	}
}
/*
Affiche écran d'accueil
*/
void afficher_accueil_SDL(){
	int i=0,nb=255*5;
	int monter=OUI;
	char str1[20],str2[20],str3[30];
	/*
	Charge l'icone
	*/
	SDL_Surface *image=IMG_Load("Images/icone.png");
	taille_cadre_X=800;
    taille_cadre_Y=640;
    SDL_Init(SDL_INIT_VIDEO);
   	SDL_WM_SetIcon(image, NULL);
    ecran = SDL_SetVideoMode(taille_cadre_X, taille_cadre_Y, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("Jeu du lac", NULL);
    /*
	Copie le message d'accueil
    */
	sprintf(str1,"Bienvenue");
	sprintf(str2,"Sur");
	sprintf(str3,"! JEU DU LAC !");
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,255, 255, 255));
	while(nb>0){
		if(i==255)
			monter=NON;
		if(i==0)
			monter=OUI;
		if(monter)
			i++;
		else 
			i--;
		nb--;
		ecran = SDL_SetVideoMode(taille_cadre_X, taille_cadre_Y,32,SDL_HWSURFACE | SDL_RESIZABLE);
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,0, 0, i));
		Create_text(280,taille_cadre_Y/4,32,0,0,255-i,str1);
		Create_text(350,taille_cadre_Y/4+taille_cadre_Y/4,32,0,0,255-i,str2);
		Create_text(230,taille_cadre_Y/4+taille_cadre_Y/4+taille_cadre_Y/8,32,0,0,255-i,str3);
		SDL_Flip(ecran);
		SDL_Delay(30);
	}
	SDL_FreeSurface(image);
	SDL_FreeSurface(ecran);
	SDL_Quit();
}