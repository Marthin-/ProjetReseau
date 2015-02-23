/*
D'après M. Pallard. 
*/
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <strings.h>
#include <sys/ioctl.h>

typedef void (*sighandler_t)(int);

struct sockaddr_in  nom_sdemande, dst;
struct sockaddr from;

struct hostent *hp;
struct ifconf ifc;
struct ifreq *ifr;
char liste[1024];

catch(){
	exit(2);
}



main(argc,argv)
int argc;
char ** argv;{

int sdemande; 

char buf[256], *p;
u_char *q;
int i, n, *pi, nc, fromlen, on;

signal(SIGINT,(sighandler_t)catch);

if ( (sdemande = socket(AF_INET, SOCK_DGRAM, 0) ) <0){
	perror("socket de demande");
	exit(1);
}

on = 1;
if(setsockopt(sdemande, SOL_SOCKET, SO_BROADCAST, &on, sizeof on)<0) {
        perror("socket opt");
        exit(1);
      }

ifc.ifc_len = sizeof (liste);
ifc.ifc_buf = liste;
if (ioctl(sdemande, SIOCGIFCONF, (char *) &ifc) <0){
      perror("ioctl IFCONF");
      exit(1);
    }

n=ifc.ifc_len/sizeof (struct ifreq); ifr = ifc.ifc_req;
for (n; --n>= 0; ifr++){
    	printf("interface %s:\n",ifr->ifr_name);
    	if (ioctl(sdemande, SIOCGIFFLAGS, (char *) ifr)<0){
      	perror("ioctl IFFLAG");
      	exit(1);
    	}
    	if ( ((ifr->ifr_flags & IFF_UP)== 0) ||
         (ifr->ifr_flags & IFF_LOOPBACK) ||
         (ifr->ifr_flags &  IFF_POINTOPOINT)||
	 ((ifr->ifr_flags & IFF_BROADCAST)==0) ){
         continue;
    	}
  
	if (ioctl(sdemande, SIOCGIFBRDADDR, (char *) ifr)){
		perror("ioctl BROADADDR");
		exit(1);
	   }
	bcopy(&(ifr->ifr_broadaddr), &dst, sizeof(ifr->ifr_broadaddr));
	dst.sin_port=htons(5011);
	dst.sin_family=AF_INET;
	socklen_t addrlen=sizeof(dst.sin_addr);
	sendto(sdemande, "bonjour", 7, 0, (struct sockaddr *) &dst, sizeof(dst));
	}

}

