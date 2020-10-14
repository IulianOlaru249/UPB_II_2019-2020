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
	multimap<string, string> dns;
	unordered_map<string, string> ptr;
	multimap<string, string> mail;
	int sockfd, newsockfd, portno;
	char buffer[BUFLEN];
	struct sockaddr_in serv_addr, cli_addr;
	int n, i, ret, connected_clients = 0;
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

					connected_clients++;
					printf("S-a conectat un nou client pe socketul %d\n", newsockfd);
				} //SE FACE O CERERE DE CONECTARE DE LA CLIENT TCP

				else if (i == 0) {
					char command[30];
					scanf("%s", command);
					if(strcmp(command, "status") == 0) {
						printf ("Connected clients: %d\n", connected_clients); 
						continue;
					}
					else if(strcmp(command, "quit") == 0) {
						goto END;
					}
					else if(strcmp(command, "info") == 0) {
						cout << "DNS:" << endl;
						for(auto it = dns.cbegin(); it != dns.cend(); ++it) {
							cout << it->first << " " << it->second << endl;
						}
						cout << "PTR:" << endl;
						for(auto it = ptr.cbegin(); it != ptr.cend(); ++it) {
							cout << it->first << " " << it->second << endl;
						}
						cout << "MAIL:" << endl;
						for(auto it = mail.cbegin(); it != mail.cend(); ++it) {
							cout << it->first << " " << it->second << endl;
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
						if(strncmp(buffer,"add_address", 11) == 0) {
							char command[20],hostname[100], ip[100];
							sscanf(buffer, "%s %s %s", command, hostname, ip);
							string host_str(hostname);
							string ip_str(ip);
							dns.insert(pair <string, string>(host_str, ip_str));
						}
						else if(strncmp(buffer,"add_name", 8) == 0) {
							char command[20],hostname[100], ip[100];
							sscanf(buffer, "%s%s%s", command, ip, hostname);
							string host_str(hostname);
							string ip_str(ip);
							ptr[ip_str] = host_str;
						}
						else if(strncmp(buffer,"add_mail", 8) == 0) {
							char command[20],hostname[100], ip[100];
							sscanf(buffer, "%s%s%s", command, hostname, ip);
							string host_str(hostname);
							string ip_str(ip);
							mail.insert(pair <string, string>(host_str, ip_str));
						}
						else if(strncmp(buffer,"get_address", 11) == 0) {
							char command[20], hostname[100];
							sscanf(buffer, "%s%s", command, hostname);
							string host_str(hostname);
							string reply;
							for(auto it = dns.cbegin(); it != dns.cend(); ++it) {
								if(it->first.compare(host_str) == 0) {
									reply =reply + it->second + "\n";
								}
							}
							n = send(i, reply.c_str(), reply.length(), 0);
							DIE(n < 0, "ERROR SENDING DNS REPLY");
						}
						else if(strncmp(buffer,"get_name", 8) == 0) {
							char command[20], ip[100];
							sscanf(buffer, "%s%s", command, ip);
							string ip_str(ip);
							string reply;
							reply = ptr[ip_str] + "\n";
							n = send(i, reply.c_str(), reply.length(), 0);
							DIE(n < 0, "ERROR SENDING DNS REPLY");
						}
						else if(strncmp(buffer,"get_mail", 8) == 0) {
							char command[20], hostname[100];
							sscanf(buffer, "%s%s", command, hostname);
							string host_str(hostname);
							string reply;
							for(auto it = mail.cbegin(); it != mail.cend(); ++it) {
								if(it->first.compare(host_str) == 0) {
									reply =reply + it->second + "\n";
								}
							}
							n = send(i, reply.c_str(), reply.length(), 0);
							DIE(n < 0, "ERROR SENDING DNS REPLY");
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