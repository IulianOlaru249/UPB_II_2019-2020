#ifndef __ICMP_H__
#define __ICMP_H__

#include "skel.h"

uint16_t ip_checksum(void* vdata,size_t length);
void init_packet(packet *pkt);
void send_icmp_packet(struct ether_header *old_ether_hdr,
                        struct iphdr *old_ip_hdr,
                        struct icmphdr *old_icmp_hdr,
                        int interface,
                        int response_type);

#endif
