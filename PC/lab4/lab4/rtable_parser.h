#ifndef ARP_H
#define ARP_H

#include <stdint.h>
#include <unistd.h>

#define INITIAL_TABLE_SIZE 0
#define IPV4_LENGTH 32

struct route_table_entry {
    uint32_t prefix;
    uint32_t next_hop;
    uint32_t mask;
    int interface;
} __attribute__((packed));

int read_rtable (struct route_table_entry **rtable, int route_table_size);
void print_entry (struct route_table_entry entry);
void print_route_table (struct route_table_entry * rtable, int route_table_size);

#endif
