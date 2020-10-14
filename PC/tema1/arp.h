#ifndef __ARP_H__
#define __ARP_H__

#include <stdint.h>
#include <unistd.h>

#include "skel.h"
struct arp_entry {
    uint32_t ip;
    uint8_t mac[6];
} __attribute__((packed));

void parse_arp_table(struct arp_entry **my_arp_table, int arp_table_len);
struct arp_entry *get_matching_arp_entry (uint32_t ip,
                                          int arp_table_size,
                                          struct arp_entry *arp_table);

#endif
