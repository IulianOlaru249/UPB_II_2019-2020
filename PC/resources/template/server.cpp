#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "helpers.h"

void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_port\n", file);
	exit(0);
}

int cli_num = 0;
int fst_cli = -1;
int set_num;
bool is_set = false;

int main(int argc, char *argv[])
{
	int not_zero = 1;
	int udp_sockfd, tcp_sockfd, newsockfd, portno;
	/* Buffer to receive tcp messages */
	char buffer[BUFLEN];
	/* Buffer to receive udp messages */
	char udp_buffer[UDP_BUFLEN];
	struct sockaddr_in serv_addr, cli_addr;
	int n, i, ret;
	socklen_t clilen;

	/* set used for select() */
	fd_set read_fds;
	/* temporary set */
	fd_set tmp_fds;
	/* maximum value of read_fds set */
	int fdmax;

	if (argc < 2)
	{
		usage(argv[0]);
	}

	/* Put read_fds set in temporary temp_fds set*/
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	/* Set fd for keyboard input */
	FD_SET(0, &read_fds);
	tmp_fds = read_fds;

	/* Open tcp socket */
	tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(tcp_sockfd < 0, "TCP socket failed to open.");

	/* Open udp socket */
	udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	DIE(tcp_sockfd < 0, "UDP socket failed to open.");

	portno = atoi(argv[1]);
	DIE(portno == 0, "atoi");

	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	int enable = 1;

	/* Set up udp socket */
	DIE((bind(udp_sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0), "Could not bind udp.\n");

	/* Set up tcp socket */
	DIE((setsockopt(tcp_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1), "Setsocketopt failed fot tcp.\n");

	DIE((bind(tcp_sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0), "Could not bind tcp.");

	DIE((listen(tcp_sockfd, MAX_CLIENTS) < 0), "Failed to listen on tcp socket.");

	/* Add new file descriptor in read_fds set */
	FD_SET(tcp_sockfd, &read_fds);
	FD_SET(udp_sockfd, &read_fds);
	(tcp_sockfd > udp_sockfd) ? fdmax = tcp_sockfd : fdmax = udp_sockfd;

	/* Map sockets to client id and client ids to client entries for rease of lookup */
	map<int, string> sock_to_id;
	map<string, struct tcp_client> tcp_clients;
	/* Map topics to ids of clients that are subscribed and their subscription type */
	map<string, vector<pair<string, int>>> topic_to_cli;

	vector<pair<string, int>> *subs;

	bool terminate = false;
	while (1)
	{
		tmp_fds = read_fds;

		DIE((select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) < 0), "Failed to monitor sockets.");

		/* Check for user command */
		if (FD_ISSET(0, &tmp_fds))
		{
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);

			if (strncmp(buffer, "status", 6) == 0)
			{
				cout << "Clienti conectati: " << cli_num << endl;
			}

			if (strncmp(buffer, "quit", 4) == 0)
			{
				shutdown(tcp_sockfd, SHUT_RD);
				for (i = 0; i <= fdmax; i++)
					if (FD_ISSET(i, &tmp_fds))
						shutdown(i, SHUT_RD);
				break;
			}
		}

		for (i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i, &tmp_fds))
			{

				if (i == tcp_sockfd)
				{
					/* New query arrived on inactive socket (the one that listens).
					 * Server will accept it.
					 */
					clilen = sizeof(cli_addr);
					newsockfd = accept(tcp_sockfd, (struct sockaddr *)&cli_addr, &clilen);
					DIE(newsockfd < 0, "Failed to accept new socket.");
					/* Dissable Neagle's algorithm */
					int disable = setsockopt(newsockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&(not_zero), sizeof(int));

					/* Add new socket returned by accept function in reading set */
					FD_SET(newsockfd, &read_fds);
					if (newsockfd > fdmax)
					{
						fdmax = newsockfd;
					}

					printf("New client connected from %s:%d.\n",
						   inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

					cli_num++;
					if (cli_num == 1)
					{
						fst_cli = newsockfd;
					}
				}
				else
				{
					/* Data recceived on one of the client sockets */
					memset(buffer, 0, BUFLEN);
					n = recv(i, buffer, sizeof(buffer), 0);
					if (n < 0)
					{
						continue;
					}

					/* Connection closed */
					if (n == 0)
					{
						if (i == fst_cli) {	continue;}
						if (i == fst_cli && !is_set)
						{
							shutdown(tcp_sockfd, SHUT_RD);
							for (i = 0; i <= fdmax; i++)
								if (FD_ISSET(i, &tmp_fds))
									shutdown(i, SHUT_RD);
							cout << "First client disconnected witought setting number.\n";
							return -1;
						}
						cout << "Client on socket " << i << " disconnected." << endl;
						cli_num--;

						/* Close socket */
						/* Removed closed socket from reading set */
						close(i);
						FD_CLR(i, &read_fds);

					}
					/* Receive request from client */
					else
					{
						struct tcp_message m;
						memcpy(&m, buffer, sizeof(struct tcp_message));

						if (m.type == 0)
						{
							if (i == fst_cli && is_set == false)
							{
								set_num = atoi(m.buffer);
								is_set = true;

								struct tcp_message m;
								m.type = 0;
								memset(m.buffer, 0, sizeof(m.id));
								strncpy(m.buffer, "Set succesfully.", sizeof(m.buffer));

								memset(buffer, 0, BUFLEN);
								memcpy(buffer, &m, sizeof(struct tcp_message));
								n = send(i, buffer, BUFLEN, 0);
								DIE(n < 0, "Failed to send.");
							}
							else
							{
								struct tcp_message m;
								m.type = 1;
								memset(m.buffer, 0, sizeof(m.id));
								strncpy(m.buffer, "Not allowed to set.", sizeof(m.buffer));

								memset(buffer, 0, BUFLEN);
								memcpy(buffer, &m, sizeof(struct tcp_message));
								n = send(i, buffer, BUFLEN, 0);
								DIE(n < 0, "Failed to send.");
							}
						}
						else if (m.type == 1)
						{
							if (is_set == false)
							{
								struct tcp_message m;
								m.type = 0;
								memset(m.buffer, 0, sizeof(m.id));
								strncpy(m.buffer, "Number not set yet.", sizeof(m.buffer));

								memset(buffer, 0, BUFLEN);
								memcpy(buffer, &m, sizeof(struct tcp_message));
								n = send(i, buffer, BUFLEN, 0);
								DIE(n < 0, "Failed to send.");
								continue;
							}
							if (i == fst_cli)
							{
								struct tcp_message m;
								m.type = 0;
								memset(m.buffer, 0, sizeof(m.id));
								strncpy(m.buffer, "Not allowed to guess.", sizeof(m.buffer));

								memset(buffer, 0, BUFLEN);
								memcpy(buffer, &m, sizeof(struct tcp_message));
								n = send(i, buffer, BUFLEN, 0);
								DIE(n < 0, "Failed to send.");
								continue;
							}
							else
							{
								int num = atoi(m.buffer);
								if (num == set_num)
								{
									struct tcp_message m;
									m.type = -2;
									memset(m.buffer, 0, sizeof(m.id));
									strncpy(m.buffer, "Numar ghicit corect!.", sizeof(m.buffer));

									memset(buffer, 0, BUFLEN);
									memcpy(buffer, &m, sizeof(struct tcp_message));
									for (int j = 0; j <= fdmax; j++) {
										if (FD_ISSET(j, &read_fds)) {
											n = send(j, buffer, BUFLEN, 0);
											DIE(n < 0, "Failed to send.");
											//shutdown(j, SHUT_RD);
										}
									}
									continue;
								}
								if (num < set_num)
								{
									struct tcp_message m;
									m.type = 2;
									memset(m.buffer, 0, sizeof(m.id));
									strncpy(m.buffer, "Mai mic decat trebuie.", sizeof(m.buffer));

									memset(buffer, 0, BUFLEN);
									memcpy(buffer, &m, sizeof(struct tcp_message));
									n = send(i, buffer, BUFLEN, 0);
									DIE(n < 0, "Failed to send.");
									continue;
								}
								if (num > set_num)
								{
									struct tcp_message m;
									m.type = 2;
									memset(m.buffer, 0, sizeof(m.id));
									strncpy(m.buffer, "Mai mare decat trebuie.", sizeof(m.buffer));

									memset(buffer, 0, BUFLEN);
									memcpy(buffer, &m, sizeof(struct tcp_message));
									n = send(i, buffer, BUFLEN, 0);
									DIE(n < 0, "Failed to send.");
									continue;
								}
							}
						}
					}
				}
			}
		}
	}

	/* Close all sockets */
	close(tcp_sockfd);
	for (i = 0; i <= fdmax; i++)
		if (FD_ISSET(i, &tmp_fds))
			close(i);
	close(udp_sockfd);

	return 0;
}
