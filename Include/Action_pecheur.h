int Construire(int Joueur_courant,int Direction);
int Bouger_joueur(int Joueur_courant,int Direction);
int Pecher(int Joueur_courant,int x,int y,int mode);
int lancer(int Joueur_courant,int x,int y);
void spawn_rand_pecheur(int J_numero);
void respawn(int J_numero);
int depile(int qui);
objet convertion_pecheur_to_objet(int J_numero);
objet convertion_pont_to_objet();