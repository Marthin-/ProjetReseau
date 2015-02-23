#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


int cree_socket_stream (const char * nom_hote, const char * nom_service, const char * nom_proto)

{
	int sock;
	struct sockaddr_in adresse;
	struct hostent * hostent;
	struct servent * servent;
	struct protoent * protoent;

	if ((hostent = gethostbyname(nom_hote)) == NULL)
	{
		perror ("gethostbyname");
		return -1;
	}
	if ((protoent = getprotobyname(nom_proto)) == NULL)
	{
		perror("getprotobyname");
		return -1;
	}
	if ((servent = getservbyname(nom_service, protoent->p_name)) == NULL)
	{
		perror("getprotobyname");
		return -1;
	}
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		return -1;
	}
	memset(& adresse, 0, sizeof (struct sockaddr_in));
	adresse.sin_family = AF_INET;
	adresse.sin_port = servent->s_port;
	adresse.sin_addr.s_addr = ((struct in_addr *) (hostent->h_addr))->s_addr;
	if (bind(sock, (struct sockaddr *) & adresse,sizeof(struct sockaddr_in)) < 0)
	{
		close(sock);
		perror("bind");
		return -1;
	}
	return sock;
}

int main(int argc, char const *argv[])
{

	return 0;
}