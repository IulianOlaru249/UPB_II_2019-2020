#include "icmp.h"

void init_packet(packet *pkt)
{ 
    memset(pkt->payload, 0, sizeof(pkt->payload));
    pkt->len = 0;
}

/* Function to compute the checksum ~from 4th lab~*/
uint16_t ip_checksum(void* vdata,size_t length) {
	// Cast the data pointer to one that can be indexed.
	char* data=(char*)vdata;

	// Initialise the accumulator.
	uint64_t acc=0xffff;

	// Handle any partial block at the start of the data.
	unsigned int offset=((uintptr_t)data)&3;
	if (offset) {
		size_t count=4-offset;
		if (count>length) count=length;
		uint32_t word=0;
		memcpy(offset+(char*)&word,data,count);
		acc+=ntohl(word);
		data+=count;
		length-=count;
	}

	// Handle any complete 32-bit blocks.
	char* data_end=data+(length&~3);
	while (data!=data_end) {
		uint32_t word;
		memcpy(&word,data,4);
		acc+=ntohl(word);
		data+=4;
	}
	length&=3;

	// Handle any partial block at the end of the data.
	if (length) {
		uint32_t word=0;
		memcpy(&word,data,length);
		acc+=ntohl(word);
	}

	// Handle deferred carries.
	acc=(acc&0xffffffff)+(acc>>32);
	while (acc>>16) {
		acc=(acc&0xffff)+(acc>>16);
	}

	// If the data began at an odd byte address
	// then reverse the byte order to compensate.
	if (offset&1) {
		acc=((acc&0xff00)>>8)|((acc&0x00ff)<<8);
	}

	// Return the checksum in network byte order.
	return htons(~acc);
} 

void send_icmp_packet(struct ether_header *old_ether_hdr,
                        struct iphdr *old_ip_hdr,
                        struct icmphdr *old_icmp_hdr,
                        int interface,
                        int response_type)
{
    packet pkt;
    init_packet (&pkt);

    pkt.len = sizeof(struct ether_header) + sizeof(struct iphdr) +
                                        sizeof(struct icmphdr);

    struct ether_header *new_ether_hdr = (struct ether_header*) pkt.payload;

    struct iphdr *new_ip_hdr = (struct iphdr*) (pkt.payload +
                                        sizeof (struct ether_header));

    struct icmphdr *new_icmp_hdr = (struct icmphdr *) (pkt.payload +
                                        sizeof (struct ether_header) +
                                        sizeof (struct iphdr));


    new_ether_hdr->ether_type = htons (ETHERTYPE_IP);
    memcpy (new_ether_hdr->ether_dhost, old_ether_hdr->ether_shost,
                    sizeof (new_ether_hdr->ether_dhost));
    memcpy (new_ether_hdr->ether_shost, old_ether_hdr->ether_dhost,
                    sizeof (new_ether_hdr->ether_dhost));

    new_ip_hdr->version = 4;
    new_ip_hdr->ihl = 5;
    new_ip_hdr->tos = 0;
    new_ip_hdr->tot_len = htons (pkt.len -
                                        sizeof (struct ether_header));

    new_ip_hdr->saddr = old_ip_hdr->daddr;
    new_ip_hdr->daddr = old_ip_hdr->saddr;

    new_ip_hdr->id = old_ip_hdr->id;
    new_ip_hdr->ttl = 64;
    new_ip_hdr->protocol = IPPROTO_ICMP;
    new_ip_hdr->check = 0;
    new_ip_hdr->check = ip_checksum (new_ip_hdr,
                                        sizeof (struct iphdr));

    new_icmp_hdr->code = 0;
    new_icmp_hdr->type = response_type;
    new_icmp_hdr->un.echo.id = old_icmp_hdr->un.echo.id;
    new_icmp_hdr->checksum = 0;
    new_icmp_hdr->checksum = ip_checksum (new_icmp_hdr,
                                sizeof (struct icmphdr));

    pkt.interface = interface;
    send_packet(pkt.interface, &pkt);
    return;
}
