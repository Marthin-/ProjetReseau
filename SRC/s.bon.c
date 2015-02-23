/*
D'après M. Pallard
*/


#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <arpa/inet.h>


typedef void (*sighandler_t)(int);

struct sockaddr_in  nom_sdemande;
struct sockaddr_in from;


catch(){
	exit(2);
}



main(argc,argv)
int argc;
char ** argv;{

int sdemande;

char buf[256];
u_char *p;
int i,  nc, fromlen;

struct hostent * hp;


signal(SIGINT,(sighandler_t)catch);

if ( (sdemande = socket(AF_INET, SOCK_DGRAM, 0) ) <0){
	perror("socket de demande");
	exit(1);
}

nom_sdemande.sin_family = AF_INET;
nom_sdemande.sin_addr.s_addr= htonl(INADDR_ANY);
nom_sdemande.sin_port=htons(5011);

if (bind ( sdemande, (const struct sockaddr *)&nom_sdemande, sizeof(nom_sdemande))<0){
	perror("bind ");
	exit(1);
}

fromlen= sizeof (from);
nc=recvfrom(sdemande, buf, sizeof ( buf), 0,(struct sockaddr *)&from, &fromlen);
if (nc <= 0){
  perror("recvfrom");
  exit(1);
}

printf("serveur recoit:");
buf[nc]='\0';printf("%s\n",buf);
hp=gethostbyaddr( &from.sin_addr,sizeof(from.sin_addr) , AF_INET);
printf("...de la part de %s(%d)\n",inet_ntoa(from.sin_addr),htons(from.sin_port));

}

