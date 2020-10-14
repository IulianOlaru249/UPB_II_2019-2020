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

	if (argc < 2) {
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
			int KEY_BUFLEN = 30;
			char key_buff[KEY_BUFLEN];
			memset(key_buff, 0, KEY_BUFLEN);
			fgets(key_buff, KEY_BUFLEN - 1, stdin);
			/* Close UDP and TCP sockets */
			char *token = strtok(key_buff, " ");
			if (strcmp(token, "exit\n") == 0 || strcmp(token, "exit") == 0)
			{
				/* Close all sockets */
				/* For tch sockets close only receiving so they can finish transmitting data in the buffer. */
				shutdown(tcp_sockfd, SHUT_RD);
				for (i = 0; i <= fdmax; i++)
					if (FD_ISSET(i, &tmp_fds))
						 shutdown(i, SHUT_RD);
				/* No more transmition or reveiving */
				shutdown(udp_sockfd, SHUT_RDWR);
				return 0;
			}
		}

		for (i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i, &tmp_fds))
			{
				if (i == udp_sockfd)
				{

					memset(udp_buffer, 0, UDP_BUFLEN);
					clilen = sizeof(cli_addr);
					int bytes = recvfrom(udp_sockfd, udp_buffer, UDP_BUFLEN, 0, (struct sockaddr *)&cli_addr, &clilen);
					DIE(bytes < 0, "Failed to receive UDP message.");

					/* Get IP and port of the UDP client */
					char *udp_cli_ip = inet_ntoa(cli_addr.sin_addr);
					int udp_cli_port = ntohs(cli_addr.sin_port);

					/* Parce message form udp client */
					struct udp_message *recv_msg = (struct udp_message *)udp_buffer;
					string topic = recv_msg->topic;

					/* If it is a topic that nobody subscribed to, it can be ignored */
					auto topic_query = topic_to_cli.find(topic);
					if (topic_query != topic_to_cli.end())
					{
						/* Compose message */
						struct cli_message new_message;
						memset(new_message.udp_cli_ip, 0, sizeof(new_message.udp_cli_ip));
						strncpy(new_message.udp_cli_ip, udp_cli_ip, strlen(udp_cli_ip));
						new_message.udp_cli_port = udp_cli_port;
						memset(new_message.topic, 0, sizeof(new_message.topic));
						strncpy(new_message.topic, recv_msg->topic, strlen(recv_msg->topic));
						new_message.type = recv_msg->type;
						memset(new_message.content, 0, 1500);
						memcpy(new_message.content, recv_msg->content, 1500);

						/* For every client subscribed to this topic */
						for (pair<string, int> subber : topic_query->second)
						{
							/* Lookup client with that id */
							auto cli_query = tcp_clients.find(subber.first);
							if (cli_query != tcp_clients.end())
							{
								struct tcp_client *subber_cli = &(cli_query->second);
								/* If they are online, send messgae */
								if (subber_cli->is_online)
								{
									/*The server should not hault when a message is not sent properly, but instead should
									 * resend the message. It is important for client to receieve notifications.
									 */
									int bytes = 0;
									while (bytes == 0) {
										bytes = send(subber_cli->socket, (char *)&new_message, sizeof(struct cli_message), 0);
									}
								}
								else
								{
									/* If they are ofline and have SF option set, save message in backlog */
									if (subber.second == 1)
									{
										subber_cli->backlog.push_back(new_message);
									}
								}
							}
						}
					}
				}
				else if (i == tcp_sockfd)
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

					/* Wait for message with client id */
					struct tcp_message m;
					n = recv(newsockfd, buffer, sizeof(buffer), 0);
					DIE(n < 0, "Failed to recv client id.");
					memcpy(&m, buffer, sizeof(struct tcp_message));

					/*See if client already exists */
					string id_cast = m.id;
					auto cli_query = tcp_clients.find(id_cast);
					if (cli_query != tcp_clients.end())
					{
						/* If the client already exists but was disconnected */
						if (!cli_query->second.is_online)
						{
							/* Mark client as back online */
							cli_query->second.is_online = true;

							/* Associate id with this socket */
							sock_to_id.insert(make_pair(newsockfd, id_cast));

							/* Set new socket */
							cli_query->second.socket = newsockfd;

							printf("New client %s connected from %s:%d.\n",
								   m.id, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

							/* Send messages from backlog */
							for (struct cli_message msg : cli_query->second.backlog) {
								int bytes = 0;
								/* It is important for users to receive their notifications so the message will be resend
								 * if not send succesfully.
								 */
								while (bytes == 0) {
									bytes = send(newsockfd, (char *)&msg, sizeof(struct cli_message), 0);
									/*Wait for message to be send so it is not concatenated with other.
									 *This delay will not impact performance.
									 */
									sleep(0.0001);
								}
							}
							cli_query->second.backlog.clear();
							continue;
						}
						else
						{
							/* Close socket */
							close(newsockfd);
							/* Removed closed socket from reading set */
							FD_CLR(newsockfd, &read_fds);
							continue;
						}
					}
					else
					{
						/* Create new client entry */
						struct tcp_client new_tcp_client;
						new_tcp_client.is_online = true;
						/* Set socket */
						new_tcp_client.socket = newsockfd;
						/* Add new client in tcp client list */
						tcp_clients.insert(make_pair(id_cast, new_tcp_client));
						/* Associate id with this socket */
						sock_to_id.insert(make_pair(newsockfd, id_cast));

						printf("New client %s connected from %s:%d.\n",
							   m.id, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
						continue;
					}
				}
				else
				{
					/* Data recceived on one of the client sockets */
					memset(buffer, 0, BUFLEN);
					n = recv(i, buffer, sizeof(buffer), 0);
					/* Failed to receive from tcp cli. It is ok if some tcm messages are lost. This way the server will
					 * not hault when a client tries to reconnect after receiving many packages.
					 */
					if(n < 0) { 
						continue;
					}
					struct tcp_message m;
					memcpy(&m, buffer, sizeof(struct tcp_message));

					/* Connection closed */
					if (n == 0)
					{
						/* Lookup the client id on that socket */
						auto targret_cli_id = sock_to_id.find(i);
						cout << "Client " << targret_cli_id->second << " disconnected." << endl;

						/* Lookup client by id and mark client as offline */
						auto targret_cli = tcp_clients.find(targret_cli_id->second);
						targret_cli->second.is_online = false;

						/* Close socket */
						close(i);
						/* Removed closed socket from reading set */
						FD_CLR(i, &read_fds);
						/* Removed closed socket-id pair from map since socket is no longer asociated with that id */
						sock_to_id.erase(i);
						/* Set client socket -1 */
						targret_cli->second.socket = -1;
					}
					/* Receive request from client */
					else
					{
						/* Subscription request */
						if (m.type == 1)
						{
							/* Get the topic that client wants to subscribe to */
							string topic;
							char *token = strtok(m.buffer, " ");
							topic = token;

							/* Get subscripiton type */
							token = strtok(NULL, " ");
							int type = 0;
							//if (token != NULL)
								type = atoi(token);
							/* If not mentioned, default subscription type will be set to 0 */

							/* Get subscriber id */
							string id_cast = m.id;

							/* If it is a new topic add it to the server topics list */
							auto topic_query = topic_to_cli.find(topic);
							if (topic_query == topic_to_cli.end())
							{
								/* Create new sub list for the topic and add client id and their subscription type to it */
								subs = new vector<pair<string, int>>;
								(*subs).push_back(make_pair(id_cast, type));

								/* Store the new topic and it's sublist */
								topic_to_cli.insert(make_pair(topic, *subs));
							}
							/* Add subscriber and their subscription type to sublist of topic */
							else
							{
								/* Make sure the same client does not subscribe in the same way to same topic twice */
								auto topic_query = topic_to_cli.find(topic);

								bool is_subbed = false;
								bool dif_sub_type = false;
								int index = 0;
								for (pair<string, int> subber : topic_query->second)
								{
									/* If it is the same client but wants a diffrent subscription type*/
									if (id_cast.compare(subber.first) == 0)
									{
										is_subbed = true;
										if (type != subber.second)
										{
											dif_sub_type = true;
											break;
										}
									}
									index++;
								}

								/* If client wants new subscription type */
								if (dif_sub_type)
									(topic_query->second)[index].second = type;

								/* If it is a new client */
								if (!is_subbed)
									topic_query->second.push_back(make_pair(id_cast, type));
							}
						}
						/* Unsubscription request */
						if (m.type == 2)
						{
							/* Get the topic that client wants to unsubscribe from */
							string topic;
							char *token = strtok(m.buffer, " ");
							topic = token;

							/* Get subscriber id */
							string id_cast = m.id;

							/* If cliend wants to unsubscribe from topic that does not exist, ignore request */
							bool is_subbed = false;
							int index = 0;
							auto topic_query = topic_to_cli.find(topic);
							if (topic_query != topic_to_cli.end())
							{
								/* If cliend wants to unsubscribe from topic they are not subscribed to, ignore request */
								for (pair<string, int> subber : topic_query->second)
								{
									if (id_cast.compare(subber.first) == 0)
									{
										is_subbed = true;
										break;
									}
								}
								index++;
							}

							/* Remove client from sub list of topic */
							if (is_subbed)
							{
								(topic_query->second).erase((topic_query->second).begin() + index - 1);
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
