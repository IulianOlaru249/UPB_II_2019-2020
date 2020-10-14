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

const int MOD = 1000000007;
/* Compute a^b in O(log n) time. */
int fast_pow(int base, int exponent) {
	if (!exponent) {
		return 1;
	}

	int aux = 1;
	while (exponent != 1) {
		if (exponent % 2 == 0) {
			base = (1LL * base * base) % MOD;
			exponent /= 2;
		} else {
			aux = (1LL * aux  * base) % MOD;
			exponent--;
		}
	}

	return aux = (1LL * aux  * base) % MOD;
}

void usage(char *file)
{
	fprintf(stderr, "Usage: %s client_id server_address server_port\n", file);
	exit(0);
}

int main(int argc, char *argv[])
{
	bool bad_unsub_usage = false;
	bool bad_sub_usage = false;

    char client_id[1024];
	int sockfd, n, ret;
	struct sockaddr_in serv_addr;
	char buffer[BUFLEN];
	struct tcp_message m;
	char recv_buffer [RECV_BUFLEN];
	memset(recv_buffer, 0, RECV_BUFLEN);

	if (argc < 4) {
		usage(argv[0]);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "TCP socket failed to open.");

    /* Set client ID */
    strncpy (client_id, argv[1], sizeof(client_id));
	serv_addr.sin_family = AF_INET;
    /* Set port */
	serv_addr.sin_port = htons(atoi(argv[3]));
    /* Set IP address */
	ret = inet_aton(argv[2], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");
	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "Failed connect");

	/* After connection send message with the client id */
	m.type = 0;
	memset(m.id, 0, sizeof (m.id));
	strncpy(m.id, client_id, sizeof(m.id));

	/* Place message in buffer and send it */
	memset(m.buffer, 0, BUFLEN);
	memcpy (buffer, &m, sizeof(struct tcp_message));
	n = send(sockfd, buffer, BUFLEN, 0);
	DIE(n < 0, "Failed to send client id.");

	fd_set read_fds;
	fd_set tmp_fds;
	int fdmax;
	fdmax = sockfd;

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	FD_SET(sockfd, &read_fds);
	FD_SET(0, &read_fds);
	tmp_fds = read_fds;

	while (1) {
		tmp_fds = read_fds;
		
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "Failed to select");

		if (FD_ISSET(0, &tmp_fds)){

			// se citeste de la tastatura
			memset(m.buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);

			char *token = strtok(buffer, " ");
			bad_unsub_usage = false;
			bad_sub_usage = false;

			/* Handle subscribe commands (type 1) */
			if (strcmp(token, "subscribe") == 0) {

				/*Set message type */
				m.type = 1;

				/* set message id to client id */
				memset(m.id, 0, sizeof (m.id));
				strncpy(m.id, client_id, sizeof(m.id));

				/* put subscription topic and SF in message buffer*/
				memset(m.buffer, 0, sizeof (m.buffer));
				memcpy(m.buffer, buffer + 10, BUFLEN - 1);

				/* Place message in buffer and send it */
				memset(buffer, 0, BUFLEN);
				memcpy (buffer, &m, sizeof(struct tcp_message));
				n = send(sockfd, buffer, BUFLEN, 0);
				/* Client should not hault if message did not send, instead notify user */
				if (n < 0) {
					fprintf(stderr, "Failed to send subscription request ");
					continue;
				}

				/* Give feedback */
				char *token2 = strtok(m.buffer, " ");
				fprintf (stderr, "Subscribed %s\n", token2);
				continue;
			}

			/* Handle unsubscribe commands (type 2) */
			//token = strtok(buffer, " ");
			else if (strcmp(token, "unsubscribe") == 0) {
				/*Set message type */
				m.type = 2;

				/* set message id to client id */
				memset(m.id, 0, sizeof (m.id));
				strncpy(m.id, client_id, sizeof(m.id));

				/* put unsubscription topic in message buffer */
				token = strtok(NULL, " ");
				if (token != NULL) {
					if (token[strlen(token) * sizeof(char) - 1] == '\n')
						token[strlen(token) * sizeof(char) - 1] = '\0';
					memset(m.buffer, 0, sizeof (m.buffer));
					memcpy(m.buffer, token, strlen(token) * sizeof(char));
				}
				else {
					bad_unsub_usage = true;
					fprintf(stderr, "Usage: unsubscribe <topic>\n");
				}

				if (!bad_unsub_usage) {
					/* Place message in buffer and send it */
					memset(buffer, 0, BUFLEN);
					memcpy (buffer, &m, sizeof(struct tcp_message));
					n = send(sockfd, buffer, BUFLEN, 0);
					/* Client should not hault if message did not send, instead notify user */
					if (n < 0) {
						fprintf(stderr, "Failed to send unsubscription request.\n");
						continue;
					}

					/* Give feedback */
					char *token2 = strtok(m.buffer, " ");
					fprintf (stderr, "Unsubscribed %s\n", token2);
					continue;
				}
			}

			/* Handle exit commands */
			//token = strtok(buffer, " ");
			else if (strcmp(token, "exit\n") == 0 || strcmp(token, "exit") == 0)
			{
				break;
			}
			continue;
		}

		else if (FD_ISSET(sockfd, &tmp_fds)){
			memset(recv_buffer, 0, RECV_BUFLEN);
			socklen_t srvr = sizeof(serv_addr);
			int rec_msg_size = recvfrom(sockfd, recv_buffer, RECV_BUFLEN, 0, (struct sockaddr *)&serv_addr, &srvr);

			/* Empty message means server closed socket */
			if (rec_msg_size == 0) {
				cout << "Server closed connection.\nPossible reasons:\n\t*ID already in use.\n\t*Server sesion closed by keyboard input.\n";
				break;
			}

			struct cli_message *recv_msg = (struct cli_message *) recv_buffer;
			fprintf( stderr, "%s:%d - %s", recv_msg->udp_cli_ip, recv_msg->udp_cli_port, recv_msg->topic);
			switch (recv_msg->type)
			{
				/* INT data type */
				case 0: {
					struct int_content *int_cont = (struct int_content *)recv_msg->content;
					fprintf( stderr, " - INT - ");
					if (int_cont->sign == 1) fprintf( stderr, "-");
					fprintf( stderr, "%d", ntohl(int_cont->value));
					break;
				}
		
				/* SHORT REAL data type */
				case 1:{
					struct short_real_content *short_real_cont = (struct short_real_content *)recv_msg->content;
					fprintf( stderr, " - SHORT_REAL - %.2f",  ntohs(short_real_cont->value)* 0.01);
					break;
				}

				/* FLOAT data type */
				case 2: {
					struct float_content *float_cont = (struct float_content *)recv_msg->content;
					fprintf( stderr, " - FLOAT - ");
					if (float_cont->sign == 1) fprintf( stderr, "-");
					if (float_cont->exp == 0)
						fprintf(stderr, "%d", ntohl(float_cont->value));
					else
						fprintf( stderr, "%.*f", float_cont->exp, (ntohl(float_cont->value) * (0.1 / fast_pow(10, float_cont->exp - 1))));
					break;
				}
				
				/*STRING data type */
				case 3: {
					recv_msg->content[1500] = '\0';
					fprintf( stderr, " - STRING - %s", recv_msg->content);
					break;
				}
			}
			fprintf( stderr, "\n");
			memset(recv_buffer, 0, RECV_BUFLEN);
		}

	}
	
	/* Allows client to receive messages that remain in buffer */
	shutdown(sockfd, SHUT_WR);

	return 0;
}
