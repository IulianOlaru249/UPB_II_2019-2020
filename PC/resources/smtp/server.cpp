#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include "helpers.h"
#include <unordered_map>
#include <iostream>
#include <map>

using namespace std;

void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_port\n", file);
	exit(0);
}

int check_prime(int n) {
	for(int d = 2; d < n/2; d++) {
		if(n % d == 0)
			return 0;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
	string current_mail;
	char buffer[BUFLEN];
	struct sockaddr_in serv_addr, cli_addr;
	int n, i, ret, connected_clients = 0;
	unordered_map<int, string> users;
	multimap<string, string> sent_mails;
	socklen_t clilen;

	fd_set read_fds;	// multimea de citire folosita in select()
	fd_set tmp_fds;		// multime folosita temporar
	int fdmax;			// valoare maxima fd din multimea read_fds

	if (argc < 2) {
		usage(argv[0]);
	}

	// se goleste multimea de descriptori de citire (read_fds) si multimea temporara (tmp_fds)
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	portno = atoi(argv[1]);
	DIE(portno == 0, "atoi");

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "bind");

	ret = listen(sockfd, MAX_CLIENTS);
	DIE(ret < 0, "listen");

	// se adauga noul file descriptor (socketul pe care se asculta conexiuni) in multimea read_fds
	FD_SET(sockfd, &read_fds);
	FD_SET(0, & read_fds);
	fdmax = sockfd;

	while (1) {
		tmp_fds = read_fds; 
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");
		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {

				if (i == sockfd) {

					clilen = sizeof(cli_addr);
					newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
					DIE(newsockfd < 0, "accept");
					
					FD_SET(newsockfd, &read_fds);
					if (newsockfd > fdmax) { 
						fdmax = newsockfd;
					}

					memset(buffer, 0, strlen(buffer));
					n = recv(newsockfd, buffer, sizeof(buffer), 0);

					users[newsockfd] = buffer;

					connected_clients++;
					printf("S-a conectat un nou client pe socketul %d cu id-ul %s\n", newsockfd, buffer);
				} //SE FACE O CERERE DE CONECTARE DE LA CLIENT TCP

				else if (i == 0) {
					char command[100];
					fgets(command, 100, stdin);
					if(strcmp(command, "status") == 0) {
						printf ("Connected clients: %d\n", connected_clients); 
						continue;
					}
					else if(strcmp(command, "quit") == 0) {
						goto END;
					}
					else if(strncmp(command, "info", 4) == 0) {
						char tmp[20], id[100];
						cout << command << endl;
						sscanf(command,"%s %s",tmp,id);
						cout << "Emails of user " << id << " are:" << endl;
						for(auto it = sent_mails.cbegin(); it != sent_mails.cend(); ++it) {
							if(it->first.compare(id) == 0) {
								cout << it->second << endl;
							}
						}
					}
				}

				else { //SE PRIMESC DATE DE LA UN CLIENT TCP
					// s-au primit date pe unul din socketii de client,
					// asa ca serverul trebuie sa le receptioneze

					memset(buffer, 0, BUFLEN);
					n = recv(i, buffer, sizeof(buffer), 0);
					DIE(n < 0, "recv");

					if (n == 0) {
						// conexiunea s-a inchis
						connected_clients--;
						close(i);
						// se scoate din multimea de citire socketul inchis 
						FD_CLR(i, &read_fds);
					} 
					else {                     

						if(strncmp(buffer, "HELO", 4) == 0 || strncmp(buffer, "MAIL", 4) == 0 || strncmp(buffer, "RCPT", 4) == 0) {
							n = send(i, "250", 3, 0);
							if(strncmp(buffer, "MAIL", 4) == 0) {
								char tmp1[50], tmp2[50], source[100];
								sscanf(buffer, "%s%s%s",tmp1,tmp2,source);
								current_mail = "From:" + string(source);
							}
							if(strncmp(buffer, "RCPT", 4) == 0) {
								char tmp1[50], tmp2[50], dest[100];
								sscanf(buffer, "%s%s%s",tmp1,tmp2,dest);
								current_mail = current_mail+ " To:" + string(dest);
							}
						}
						else if(strncmp(buffer, "DATA",4) == 0) {
							n = send(i, "254", 3, 0);	
						}
						else if(strncmp(buffer, "QUIT",4) == 0) {
							n = send(i, "221", 3, 0);
							sent_mails.insert(pair<string, string>(users[i],current_mail));	
							current_mail = "";
						}
						else {
							n = send(i, "250", 3, 0);
						}
					} //S-a primit o cerere
				}
			}
		}
	}
END:
	close(sockfd);
	return 0;
}