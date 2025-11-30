/*
 * TCP Traffic Analyzer - Network Simulator
 * Captures and analyzes TCP packets using raw sockets
 * Author: Network Programming Lab
 * Date: 2025-11-19
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <time.h>

#define BUFFER_SIZE 65536

// Structure to hold TCP packet statistics
struct tcp_stats {
    unsigned long total_packets;
    unsigned long syn_packets;
    unsigned long ack_packets;
    unsigned long fin_packets;
    unsigned long rst_packets;
    unsigned long psh_packets;
    unsigned long urg_packets;
    unsigned long total_bytes;
};

struct tcp_stats stats = {0};

// Function to get current timestamp
void get_timestamp(char *buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(buffer, "%02d-%02d-%04d %02d:%02d:%02d", 
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min, t->tm_sec);
}

// Function to print Ethernet header
void print_ethernet_header(unsigned char *buffer, int size) {
    struct ethhdr *eth = (struct ethhdr *)buffer;
    
    printf("\n");
    printf("================================================================\n");
    printf("                  ETHERNET HEADER (LAYER 2)                    \n");
    printf("================================================================\n");
    printf("  |-Destination MAC Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
           eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
           eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf("  |-Source MAC Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
           eth->h_source[0], eth->h_source[1], eth->h_source[2],
           eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    printf("  |-Protocol Type           : 0x%.4x ", ntohs(eth->h_proto));
    
    switch(ntohs(eth->h_proto)) {
        case ETH_P_IP:
            printf("(IPv4)\n");
            break;
        case ETH_P_IPV6:
            printf("(IPv6)\n");
            break;
        case ETH_P_ARP:
            printf("(ARP)\n");
            break;
        default:
            printf("(Other)\n");
            break;
    }
    printf("================================================================\n");
}

// Function to print IP header
void print_ip_header(unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
    
    printf("\n");
    printf("================================================================\n");
    printf("                    IP HEADER (LAYER 3)                        \n");
    printf("================================================================\n");
    printf("  |-IP Version              : %d\n", (unsigned int)iph->version);
    printf("  |-IP Header Length        : %d bytes (IHL: %d)\n", 
           ((unsigned int)(iph->ihl)) * 4, (unsigned int)iph->ihl);
    printf("  |-Type Of Service         : 0x%.2x\n", (unsigned int)iph->tos);
    printf("  |-IP Total Length         : %d bytes\n", ntohs(iph->tot_len));
    printf("  |-Identification          : 0x%.4x (%d)\n", ntohs(iph->id), ntohs(iph->id));
    
    // Flags
    printf("  |-Flags                   : ");
    if (ntohs(iph->frag_off) & 0x4000) printf("DF ");
    if (ntohs(iph->frag_off) & 0x2000) printf("MF ");
    printf("\n");
    
    printf("  |-Fragment Offset         : %d\n", ntohs(iph->frag_off) & 0x1FFF);
    printf("  |-Time To Live (TTL)      : %d hops\n", (unsigned int)iph->ttl);
    printf("  |-Protocol                : %d ", (unsigned int)iph->protocol);
    
    switch(iph->protocol) {
        case IPPROTO_TCP:
            printf("(TCP)\n");
            break;
        case IPPROTO_UDP:
            printf("(UDP)\n");
            break;
        case IPPROTO_ICMP:
            printf("(ICMP)\n");
            break;
        default:
            printf("(Other)\n");
            break;
    }
    
    printf("  |-Header Checksum         : 0x%.4x\n", ntohs(iph->check));
    printf("  |-Source IP Address       : %s\n", inet_ntoa(source.sin_addr));
    printf("  |-Destination IP Address  : %s\n", inet_ntoa(dest.sin_addr));
    printf("================================================================\n");
}

// Function to print TCP header
void print_tcp_header(unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    unsigned short iphdrlen = iph->ihl * 4;
    
    struct tcphdr *tcph = (struct tcphdr *)(buffer + iphdrlen + sizeof(struct ethhdr));
    
    printf("\n");
    printf("================================================================\n");
    printf("                    TCP HEADER (LAYER 4)                       \n");
    printf("================================================================\n");
    printf("  |-Source Port             : %u\n", ntohs(tcph->source));
    printf("  |-Destination Port        : %u\n", ntohs(tcph->dest));
    printf("  |-Sequence Number         : %u\n", ntohl(tcph->seq));
    printf("  |-Acknowledge Number      : %u\n", ntohl(tcph->ack_seq));
    printf("  |-Header Length           : %d bytes (DOFF: %d)\n", 
           (unsigned int)tcph->doff * 4, (unsigned int)tcph->doff);
    
    // TCP Flags
    printf("  |-TCP Flags               : ");
    if (tcph->urg) printf("URG ");
    if (tcph->ack) printf("ACK ");
    if (tcph->psh) printf("PSH ");
    if (tcph->rst) printf("RST ");
    if (tcph->syn) printf("SYN ");
    if (tcph->fin) printf("FIN ");
    printf("\n");
    
    printf("  |-Window Size             : %u bytes\n", ntohs(tcph->window));
    printf("  |-Checksum                : 0x%.4x\n", ntohs(tcph->check));
    printf("  |-Urgent Pointer          : %u\n", tcph->urg_ptr);
    printf("================================================================\n");
    
    // Update statistics
    stats.total_packets++;
    stats.total_bytes += size;
    
    if (tcph->syn) stats.syn_packets++;
    if (tcph->ack) stats.ack_packets++;
    if (tcph->fin) stats.fin_packets++;
    if (tcph->rst) stats.rst_packets++;
    if (tcph->psh) stats.psh_packets++;
    if (tcph->urg) stats.urg_packets++;
}

// Function to print TCP data payload
void print_tcp_data(unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    unsigned short iphdrlen = iph->ihl * 4;
    
    struct tcphdr *tcph = (struct tcphdr *)(buffer + iphdrlen + sizeof(struct ethhdr));
    unsigned short tcphdrlen = tcph->doff * 4;
    
    unsigned char *data = buffer + sizeof(struct ethhdr) + iphdrlen + tcphdrlen;
    int data_size = size - (sizeof(struct ethhdr) + iphdrlen + tcphdrlen);
    
    if (data_size > 0) {
        printf("\n");
        printf("================================================================\n");
        printf("                    TCP DATA PAYLOAD                           \n");
        printf("================================================================\n");
        printf("  |-Data Size               : %d bytes\n", data_size);
        printf("  |-Data (First 64 bytes)   :\n");
        
        int display_size = (data_size > 64) ? 64 : data_size;
        
        // Print in hexadecimal format
        printf("\n  Hex Format:\n  ");
        for (int i = 0; i < display_size; i++) {
            printf("%.2X ", data[i]);
            if ((i + 1) % 16 == 0) printf("\n  ");
        }
        printf("\n");
        
        // Print in ASCII format
        printf("\n  ASCII Format:\n  ");
        for (int i = 0; i < display_size; i++) {
            if (data[i] >= 32 && data[i] <= 126) {
                printf("%c", data[i]);
            } else {
                printf(".");
            }
            if ((i + 1) % 64 == 0) printf("\n  ");
        }
        printf("\n");
        printf("================================================================\n");
    } else {
        printf("\n  [No TCP Data Payload]\n");
    }
}

// Function to print statistics
void print_statistics() {
    printf("\n\n");
    printf("################################################################\n");
    printf("                  TCP TRAFFIC STATISTICS                        \n");
    printf("################################################################\n");
    printf("  Total TCP Packets Captured  : %lu\n", stats.total_packets);
    printf("  Total Bytes Captured        : %lu bytes (%.2f KB)\n", 
           stats.total_bytes, stats.total_bytes / 1024.0);
    printf("----------------------------------------------------------------\n");
    printf("  SYN Packets (Connection)    : %lu\n", stats.syn_packets);
    printf("  ACK Packets (Acknowledge)   : %lu\n", stats.ack_packets);
    printf("  PSH Packets (Push Data)     : %lu\n", stats.psh_packets);
    printf("  FIN Packets (Termination)   : %lu\n", stats.fin_packets);
    printf("  RST Packets (Reset)         : %lu\n", stats.rst_packets);
    printf("  URG Packets (Urgent)        : %lu\n", stats.urg_packets);
    printf("################################################################\n\n");
}

// Function to process TCP packet
void process_tcp_packet(unsigned char *buffer, int size) {
    char timestamp[30];
    get_timestamp(timestamp);
    
    printf("\n\n");
    printf("################################################################\n");
    printf("          TCP PACKET CAPTURED - %s          \n", timestamp);
    printf("################################################################\n");
    printf("Packet Size: %d bytes\n", size);
    
    // Print all headers
    print_ethernet_header(buffer, size);
    print_ip_header(buffer, size);
    print_tcp_header(buffer, size);
    print_tcp_data(buffer, size);
    
    printf("\n################################################################\n");
    printf("                    END OF PACKET                              \n");
    printf("################################################################\n");
}

// Main packet processing function
void packet_handler(unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    
    // Process only TCP packets
    if (iph->protocol == IPPROTO_TCP) {
        process_tcp_packet(buffer, size);
    }
}

int main() {
    int sock_raw;
    int saddr_size, data_size;
    struct sockaddr saddr;
    unsigned char *buffer = (unsigned char *)malloc(BUFFER_SIZE);
    
    printf("################################################################\n");
    printf("          TCP TRAFFIC ANALYZER - NETWORK SIMULATOR             \n");
    printf("################################################################\n");
    printf("Starting TCP packet capture...\n");
    printf("Press Ctrl+C to stop and view statistics\n");
    printf("################################################################\n\n");
    
    // Create raw socket to capture packets at Ethernet level
    // AF_PACKET: Low level packet interface
    // SOCK_RAW: Raw protocol access
    // ETH_P_ALL: All protocols
    sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    
    if (sock_raw < 0) {
        perror("Socket Error: Failed to create raw socket");
        printf("\nNote: This program requires root/sudo privileges!\n");
        printf("Run with: sudo ./tcp_packet_analyzer\n\n");
        return 1;
    }
    
    printf("[SUCCESS] Raw socket created successfully!\n");
    printf("[INFO] Listening on all network interfaces...\n");
    printf("[INFO] Capturing TCP packets...\n\n");
    
    // Continuous packet capture loop
    while (1) {
        saddr_size = sizeof(saddr);
        
        // Receive a packet
        data_size = recvfrom(sock_raw, buffer, BUFFER_SIZE, 0, &saddr, (socklen_t *)&saddr_size);
        
        if (data_size < 0) {
            perror("Recvfrom error");
            break;
        }
        
        // Process the packet
        packet_handler(buffer, data_size);
        
        // Print statistics every 10 packets
        if (stats.total_packets % 10 == 0) {
            print_statistics();
        }
    }
    
    // Cleanup
    close(sock_raw);
    free(buffer);
    
    // Print final statistics
    print_statistics();
    
    return 0;
}
