#include "arp.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>


void parse_arp_table(struct arp_entry **my_arp_table, int arp_table_len)
{
    struct arp_entry *arp_table = *my_arp_table;
	FILE *f = fopen("arp_table.txt", "r");
	DIE(f == NULL, "Failed to open arp_table.txt");

	char line[100];
	int i = 0;
	for(i = 0; fgets(line, sizeof(line), f); i++) {
		char ip_str[50], mac_str[50];
		sscanf(line, "%s %s", ip_str, mac_str);

		arp_table[i].ip = inet_addr(ip_str);
		int rc = hwaddr_aton(mac_str, arp_table[i].mac);
		DIE(rc < 0, "invalid MAC");
	}
	arp_table_len = i;
	fclose(f);
}

/* Looks for an IP in the ARP table and return the matching MAC address. */
struct arp_entry *get_matching_arp_entry (uint32_t ip,
                                          int arp_table_size,
                                          struct arp_entry *arp_table)
{
    for (int i = 0; i < arp_table_size; i++)
        if (arp_table[i].ip == ip)
            return &arp_table[i];
    return NULL;
}

