#ifndef _HELPERS_H
#define _HELPERS_H 1

#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h> 
#include <vector>
#include <netinet/tcp.h>

using namespace std;

#define UDP_BUFLEN 1551
#define RECV_BUFLEN 1585
#define BUFLEN		256	// dimensiunea maxima a calupului de date
#define MAX_CLIENTS	5	// numarul maxim de clienti in asteptare

/* Template for tcp message */
struct tcp_message {
	int type;
	char id [10];
	char buffer [BUFLEN -sizeof (int) - 10 * sizeof (char)];
}__attribute__((packed));

/* Template for message from server to subscriber */
struct cli_message {
	char udp_cli_ip[30];
	int udp_cli_port;
	char topic[50];
	uint8_t type;
	char content[1500];
}__attribute__((packed));

/* Template for udp template */
struct udp_message {
	char topic[50];
	uint8_t type;
	char content[1500];
}__attribute__((packed));

/* Types of content the message can have based on the data type */
struct int_content {
	/* 0 or 1 */
	uint8_t sign;
	/* in network byte order */
	uint32_t value; 
}__attribute__((packed));

struct short_real_content {
	uint16_t value;
}__attribute__((packed));

struct float_content {
	/* 0 or 1 */
	uint8_t sign;
	/* in network order*/
	uint32_t value;
	uint8_t exp;
}__attribute__((packed));

/* Template fotr tcp clients */
struct tcp_client {
	int socket;
	bool is_online;
	vector<struct cli_message> backlog;
};

/*
 * Macro de verificare a erorilor
 * Exemplu:
 *     int fd = open(file_name, O_RDONLY);
 *     DIE(fd == -1, "open failed");
 */

#define DIE(assertion, call_description)	\
	do {									\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",	\
					__FILE__, __LINE__);	\
			perror(call_description);		\
			exit(EXIT_FAILURE);				\
		}									\
	} while(0)

#endif
