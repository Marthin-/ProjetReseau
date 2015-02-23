CC=gcc
CFLAGS=-W -Wall -O3 -g -ansi -pedantic
LDFLAGS= -lSDL_mixer -lSDL_image -lSDL_ttf -lSDL

exec: main.o Include/init_fin.o Include/Action_pecheur.o Include/Regle_de_vie.o Include/affichage.o Include/Define.h
	$(CC) -o $@ $^ $(LDFLAGS)

debug: exec
	valgrind --tool=memcheck --leak-check=full -v  ./debug 2> erreur.txt

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o
	rm -f Include/*.o

cpt:
	wc -l *.c Include/*.c Include/*.h

zip:
	rm -f Projet_Da_costa_Pasquion.zip
	zip -r Projet_Da_costa_Pasquion.zip Musique/Accueil.mp3 Musique/Ma_musique.mp3 Images/mer_converted.jpg Images/icone.png Include/*.h Include/*.c Polices/police1.ttf Makefile Readme.txt configuration.txt main.c exec