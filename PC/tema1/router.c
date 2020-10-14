#include "skel.h"
#include "queue.h"
#include "rtable_parser.h"
#include "arp.h"
#include "icmp.h"
#include <netinet/if_ether.h>

/* Router logic */
int main(int argc, char *argv[])
{
	packet m;
	int rc;

	init();

    /* Allocate the ARP table */
    int arp_table_size = 100;
    struct arp_entry *arp_table = (struct arp_entry*) malloc (
                   sizeof (struct arp_entry) * arp_table_size);

    /* Parse the ARP table */
    parse_arp_table(&arp_table, arp_table_size);

    /* The routing table starts with iniitial dimension 0 and it dinamicly
       allocates memory for new entries.
     */
    int route_table_size = INITIAL_ROUTE_TABLE_SIZE;
    struct route_table_entry* route_table = (struct route_table_entry*) malloc (
                  sizeof (struct route_table_entry) * INITIAL_ROUTE_TABLE_SIZE);

    DIE ((route_table == NULL), "Route table could not be allocated!");

    /* Parse the route table and populate the array. */
    route_table_size = read_rtable(&route_table, route_table_size);

    qsort(route_table, route_table_size, sizeof (struct route_table_entry), compare);
	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "get_message");
        
        /* Parse the packadge and store it in memory */
        struct ether_header *eth_hdr = (struct ether_header *) m.payload;
        struct iphdr *ip_hdr;
        struct icmphdr *icmp_hdr;

        /* Parse ARP header */
        if (eth_hdr->ether_type == ETHERTYPE_ARP) {
            continue;
        }
        /* Parse specific ICMP headers */
        else {
            ip_hdr = (struct iphdr *) (m.payload +
                                        sizeof (struct ether_header));

            icmp_hdr = (struct icmphdr *) (m.payload +
                                        sizeof (struct ether_header) + 
                                        sizeof (struct iphdr));
        }

        /* Check the checksum */
        if (ip_checksum (ip_hdr, sizeof (struct iphdr)))
            continue;

        /* Check TTL */
        if (ip_hdr->ttl <= 1) {
            /* send ICMP timeout message */
            send_icmp_packet(eth_hdr, ip_hdr, icmp_hdr, 
                    m.interface,  ICMP_TIME_EXCEEDED);
            continue;
        }


        /* Check if router interface */
        int isInterface = 0;
        for (int i = 0; i < ROUTER_NUM_INTERFACES; i++) {
            int packed_daddr;
            inet_pton (AF_INET, get_interface_ip(i), &packed_daddr);
            if (packed_daddr == ip_hdr->daddr)
                isInterface = 1;
        }

        if (isInterface) {
            /* Send reply */
            send_icmp_packet(eth_hdr, ip_hdr, icmp_hdr,
                    m.interface, ICMP_ECHOREPLY);
            continue;
        }

        /* Find best route */
        struct route_table_entry *best_route =
                  get_best_route (ip_hdr->daddr, route_table_size, route_table);

        if (best_route == NULL ) {
            /* Send ICMP host unreachable message */
            send_icmp_packet(eth_hdr, ip_hdr, icmp_hdr, 
                    m.interface, ICMP_DEST_UNREACH);
            continue;
        }
                
        /* Update TTL and Checksum */
        ip_hdr->ttl --;

        /* Old check sum in set to 0 so it dose not influence the new one */
        ip_hdr->check = 0;
        ip_hdr->check = ip_checksum (ip_hdr, sizeof (struct iphdr));

        /* Find matching ARP entry and update MAC */
        struct arp_entry *matching_arp = get_matching_arp_entry (ip_hdr->daddr,
                            arp_table_size, arp_table);

        memcpy (eth_hdr->ether_dhost, matching_arp->mac,
              sizeof (matching_arp->mac));

        /* Send packadge via best route */
        send_packet (best_route->interface, &m);
	}

    free (arp_table);
    free (route_table);
}
