void Survie(objet *obj);
void Tour(objet *obj);
void Predation(objet *obj1,int i,int j);
void Deplacement(objet *obj,int x,int y);
void Deplacement_rand(int direction,objet *obj,int nb_case,int x,int y);
void Reproduction(objet *obj,int x,int y);
void vide(objet *obj);
objet convertion_animal_to_objet(espece_default e);
