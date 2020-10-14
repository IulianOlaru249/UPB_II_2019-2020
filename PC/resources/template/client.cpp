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

int vieti = 5;
int numar;
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
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
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

	if (argc < 3) {
		usage(argv[0]);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "TCP socket failed to open.");

    // /* Set client ID */
    // strncpy (client_id, argv[1], sizeof(client_id));
	serv_addr.sin_family = AF_INET;
    /* Set port */
	serv_addr.sin_port = htons(atoi(argv[3]));
    /* Set IP address */
	ret = inet_aton(argv[2], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");
	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "Failed connect");

	// /* After connection send message with the client id */
	// m.type = 0;
	// memset(m.id, 0, sizeof (m.id));
	// strncpy(m.id, client_id, sizeof(m.id));

	// /* Place message in buffer and send it */
	// memset(m.buffer, 0, BUFLEN);
	// memcpy (buffer, &m, sizeof(struct tcp_message));
	// n = send(sockfd, buffer, BUFLEN, 0);
	// DIE(n < 0, "Failed to send client id.");

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
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);
        	//char *user_input = strtok(buffer, " ");
        	//user_input = strtok(user_input, "\n ");


			if (strncmp(buffer, "set", 3) == 0) {
				char * num_char = strtok (buffer, " ");
				num_char = strtok (NULL, "\n");

				m.type = 0;
				memset(m.buffer, 0, sizeof (m.id));
				strncpy(m.buffer, num_char, sizeof(m.id));
				
				memset(buffer, 0, BUFLEN);
				memcpy (buffer, &m, sizeof(struct tcp_message));
				n = send(sockfd, buffer, BUFLEN, 0);
				DIE(n < 0, "Failed to send.");
			}

			if (strncmp(buffer, "guess", 5) == 0) {
				char * num_char = strtok (buffer, " ");
				num_char = strtok (NULL, "\n");
				//int num = atoi(num_char);

				m.type = 1;
				memset(m.buffer, 0, sizeof (m.id));
				strncpy(m.buffer, num_char, sizeof(m.id));
				
				memset(buffer, 0, BUFLEN);
				memcpy (buffer, &m, sizeof(struct tcp_message));
				n = send(sockfd, buffer, BUFLEN, 0);
				DIE(n < 0, "Failed to send.");
			}

			if (strncmp(buffer, "quit", 4) == 0) {
				break;
			}

			if (strncmp(buffer, "status", 5) == 0) {
				cout << "Vieti ramase: " << vieti << endl;
			}
			
		}

		else if (FD_ISSET(sockfd, &tmp_fds)){
			memset(recv_buffer, 0, RECV_BUFLEN);
			socklen_t srvr = sizeof(serv_addr);
			int rec_msg_size = recvfrom(sockfd, recv_buffer, RECV_BUFLEN, 0, (struct sockaddr *)&serv_addr, &srvr);

			/* Empty message means server closed socket */
			if (rec_msg_size == 0) {
				cout << "Server closed connection.\n";
				break;
			}

			memcpy(&m, recv_buffer, sizeof(struct tcp_message));

			cout << m.buffer << endl;

			if (m.type == 2) {
				vieti --;
				if (vieti == 0) {
					cout << "Nu mai sunt vieti\n";
					break;
				}
			}
			if (m.type == -2) {
				cout << "Numar ghicit corect!\n";
				break;
			}
		}

	}
	
	/* Allows client to receive messages that remain in buffer */
	shutdown(sockfd, SHUT_WR);

	return 0;
}
