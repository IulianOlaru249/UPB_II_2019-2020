#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helpers.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
#include <time.h>

using namespace std;

template <class Container>
void split1(const std::string& str, Container& cont)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter(cont));
}

void usage(char *file)
{
	fprintf(stderr, "Usage: %s id server_address server_port\n", file);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, n, ret;
	struct sockaddr_in serv_addr;
	char buffer[BUFLEN];
	char receive[BUFLEN];

	if (argc < 4) {
		usage(argv[0]);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	ret = inet_aton(argv[2], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	strcpy(buffer, argv[1]);
	argv[1][strlen(argv[1])] = '\0';
	n = send(sockfd, buffer, strlen(buffer), 0);
	DIE(n < 0, "ERROR SENDING ID");

	fd_set read_fds;
	fd_set tmp_fds;
	int fdmax, i;

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	FD_SET(sockfd, &read_fds);
	FD_SET(0, & read_fds);
	fdmax = sockfd;

	while (1) {

		tmp_fds = read_fds;

		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");
		
		if(FD_ISSET(0, &tmp_fds)) { 
  				// se citeste de la tastatura
				memset(buffer, 0, BUFLEN);
				fgets(buffer, BUFLEN - 1, stdin);

				if (strncmp(buffer, "quit", 4) == 0) {
					break;
				}

				string buffer_str(buffer);
								
				vector<string> tokens;
				split1(buffer_str, tokens);

				int arg = 0;

				string sursa;
				string dest;
				string corp;

				for(string s : tokens) {
					if(arg == 1)
						sursa = s;
					else if(arg == 2)
						dest = s;
					else if(arg >= 3)
						corp = corp + " " + s;
					arg++;
				}

				sprintf(buffer, "HELO %s", argv[1]);
				//Se trimite mesaj la server
				n = send(sockfd, buffer, strlen(buffer), 0);
				DIE(n < 0, "helo");
				cout << buffer << endl;

				memset(buffer, 0, BUFLEN);
				n = recv(sockfd, buffer, sizeof(buffer), 0);
				cout << buffer << endl;

				sprintf(buffer, "MAIL FROM: %s", sursa.c_str());
				n = send(sockfd, buffer, strlen(buffer), 0);
				DIE(n < 0, "MAIL FROM");
				cout << buffer << endl;

				memset(buffer, 0, BUFLEN);
				n = recv(sockfd, buffer, sizeof(buffer), 0);
				cout << buffer << endl;

				sprintf(buffer, "RCPT TO: %s", dest.c_str());
				n = send(sockfd, buffer, strlen(buffer), 0);
				DIE(n < 0, "RCPT");
				cout << buffer << endl;

				memset(buffer, 0, BUFLEN);
				n = recv(sockfd, buffer, sizeof(buffer), 0);
				cout << buffer << endl;

				time_t t = time(NULL);
 				struct tm tm = *localtime(&t);
 				sprintf(buffer,"DATA: %d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				n = send(sockfd, buffer, strlen(buffer), 0);
				DIE(n < 0, "DATA");
				cout << buffer << endl;

				memset(buffer, 0, BUFLEN);
				n = recv(sockfd, buffer, sizeof(buffer), 0);
				cout << buffer << endl;

				sprintf(buffer, "%s", corp.c_str());
				n = send(sockfd, buffer, strlen(buffer), 0);
				DIE(n < 0, "corp");
				cout << buffer << endl;

				memset(buffer, 0, BUFLEN);
				n = recv(sockfd, buffer, sizeof(buffer), 0);
				cout << buffer << endl;

				sprintf(buffer, "QUIT");
				n = send(sockfd, buffer, strlen(buffer), 0);
				DIE(n < 0, "quit");
				cout << buffer << endl;	

				memset(buffer, 0, BUFLEN);
				n = recv(sockfd, buffer, sizeof(buffer), 0);
				cout << buffer << endl;
			

			}
		else if(FD_ISSET(sockfd, &tmp_fds)) {	
				memset(buffer, 0,BUFLEN);
				n = recv(sockfd, buffer, sizeof(buffer), 0);
				if(n == 0) {
					goto END;
				}
				printf("%s\n", buffer);
			}
		}
END:
	close(sockfd);
	return 0;
}