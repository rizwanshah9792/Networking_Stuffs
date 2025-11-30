#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h> 
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <time.h>

// Global flags to track what we found
int found_ip = 0, found_arp = 0;
int found_icmp = 0, found_tcp = 0, found_udp = 0;

void packet_handler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    // 1. Handle Layer 2 (Ethernet)
    struct ether_header *eth_header;
    eth_header = (struct ether_header *) packet;
    
    printf("[%ld.%06ld] ", pkthdr->ts.tv_sec, pkthdr->ts.tv_usec);

    // Check EtherType (L2 Protocol)
    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        found_ip = 1;
        
        // 2. Handle Layer 3 (IP)
        // Skip Ethernet header size (14 bytes) to get to IP header
        struct ip *ip_header = (struct ip*)(packet + sizeof(struct ether_header));
        
        printf("ETH -> IP  -> ");

        // 3. Handle Layer 4 (Transport)
        switch(ip_header->ip_p) {
            case IPPROTO_ICMP:
                found_icmp = 1;
                printf("ICMP (Ping) [Packet Size: %d]\n", pkthdr->len);
                break;
            case IPPROTO_TCP:
                found_tcp = 1;
                printf("TCP         [Packet Size: %d]\n", pkthdr->len);
                break;
            case IPPROTO_UDP:
                found_udp = 1;
                printf("UDP         [Packet Size: %d]\n", pkthdr->len);
                break;
            default:
                printf("Unknown L4  [Protocol ID: %d]\n", ip_header->ip_p);
                break;
        }
    } 
    else if (ntohs(eth_header->ether_type) == ETHERTYPE_ARP) {
        found_arp = 1;
        printf("ETH -> ARP  -> Request/Reply\n");
    } 
    else {
        printf("ETH -> Unknown L2 Protocol\n");
    }
}

int main(int argc, char *argv[]) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    if (argc != 2) {
        printf("Usage: %s <pcap_file>\n", argv[0]);
        return 1;
    }

    // Open the saved pcap file
    handle = pcap_open_offline(argv[1], errbuf);
    if (handle == NULL) {
        printf("Error opening file: %s\n", errbuf);
        return 1;
    }

    printf("---------------- TIME DIAGRAM ----------------\n");
    printf("Timestamp        | Protocol Flow\n");
    printf("----------------------------------------------\n");

    // Loop through packets
    pcap_loop(handle, 0, packet_handler, NULL);

    printf("----------------------------------------------\n\n");
    printf("SUMMARY OF DETECTED PROTOCOLS:\n");
    printf("Layer 2 (Data Link): Ethernet\n");
    
    printf("Layer 3 (Network)  : ");
    if (found_ip) printf("IPv4 ");
    if (found_arp) printf("ARP ");
    printf("\n");

    printf("Layer 4 (Transport): ");
    if (found_icmp) printf("ICMP ");
    if (found_tcp) printf("TCP ");
    if (found_udp) printf("UDP ");
    printf("\n");

    pcap_close(handle);
    return 0;
}
