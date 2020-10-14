/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	client mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "helpers.h"
#include <unistd.h>
#include <arpa/inet.h>

void usage(char*file)
{
	fprintf(stderr,"Usage: %s ip_server port_server file\n",file);
	exit(0);
}

/*
*	Utilizare: ./client ip_server port_server nume_fisier_trimis
*/
int main(int argc,char**argv)
{
	if (argc!=4)
		usage(argv[0]);
	
	int fd;
	struct sockaddr_in to_station;
	char buf[BUFLEN];


	/*Deschidere socket*/
	int s = socket(PF_INET, SOCK_DGRAM, 0);
	DIE((s < 0) ,"Socket did not open\n");
	
	
	/* Deschidere fisier pentru citire */
	DIE((fd=open(argv[3],O_RDONLY))==-1,"open file");
	
	/*Setare struct sockaddr_in pentru a specifica unde trimit datele*/
	to_station.sin_family = AF_INET;	
	to_station.sin_port = htons(12345);
	inet_aton("127.0.0.1", &to_station.sin_addr);
	
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din fisier
	*		trimite pe socket
	*/	

	int r = 1;
	while (r > 0) {
		r = read(fd, buf, BUFLEN);
		DIE( (sendto(s, buf, r, 0, (struct sockaddr*) &to_station, sizeof(to_station)) < 0 ),
			"Did not send\n");
		usleep(100);
	}


	/*Inchidere socket*/
	close(s);
	
	/*Inchidere fisier*/
	close(fd);

	return 0;
}
