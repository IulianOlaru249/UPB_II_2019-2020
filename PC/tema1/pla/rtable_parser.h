#ifndef __RTABLE_PARSER_H__
#define __RTABLE_PARSER_H__

#include <stdint.h>
#include <unistd.h>

#define INITIAL_ROUTE_TABLE_SIZE 0
#define IPV4_LENGTH 32

struct route_table_entry {
    uint32_t prefix;
    uint32_t next_hop;
    uint32_t mask;
    int interface;
} __attribute__((packed));

int read_rtable (struct route_table_entry **rtable, int route_table_size);

int compare (const void *a, const void *b);
struct route_table_entry *get_best_route (uint32_t destination_ip,
                                          int route_table_size,
                                          struct route_table_entry *rtable);
void print_entry (struct route_table_entry entry);
void print_route_table (struct route_table_entry * rtable, int route_table_size);

#endif
