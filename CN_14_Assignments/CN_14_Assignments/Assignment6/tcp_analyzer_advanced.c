/*
 * Advanced TCP Traffic Analyzer with Filtering
 * Author: Network Programming Lab
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
#include <signal.h>
#include <time.h>

#define BUFFER_SIZE 65536
#define LOG_FILE "tcp_traffic.log"

// Global variables
int sock_raw;
unsigned char *buffer;
FILE *log_file;

// TCP Connection structure
struct tcp_connection {
    char src_ip[16];
    char dst_ip[16];
    unsigned short src_port;
    unsigned short dst_port;
    unsigned long packets;
    unsigned long bytes;
    time_t first_seen;
    time_t last_seen;
};

struct tcp_connection connections[1000];
int connection_count = 0;

// Statistics structure
struct detailed_stats {
    unsigned long total_packets;
    unsigned long total_bytes;
    unsigned long syn_packets;
    unsigned long syn_ack_packets;
    unsigned long ack_packets;
    unsigned long fin_packets;
    unsigned long rst_packets;
    unsigned long http_packets;
    unsigned long https_packets;
    unsigned long ssh_packets;
} stats = {0};

// Signal handler for graceful exit
void signal_handler(int signum) {
    printf("\n\n[SIGNAL] Received interrupt signal. Closing...\n");
    
    // Print final statistics
    printf("\n################################################################\n");
    printf("              FINAL TCP TRAFFIC STATISTICS                     \n");
    printf("################################################################\n");
    printf("Total Packets: %lu\n", stats.total_packets);
    printf("Total Bytes: %lu (%.2f MB)\n", stats.total_bytes, 
           stats.total_bytes / (1024.0 * 1024.0));
    printf("----------------------------------------------------------------\n");
    printf("SYN Packets: %lu\n", stats.syn_packets);
    printf("SYN-ACK Packets: %lu\n", stats.syn_ack_packets);
    printf("ACK Packets: %lu\n", stats.ack_packets);
    printf("FIN Packets: %lu\n", stats.fin_packets);
    printf("RST Packets: %lu\n", stats.rst_packets);
    printf("----------------------------------------------------------------\n");
    printf("HTTP Packets (Port 80): %lu\n", stats.http_packets);
    printf("HTTPS Packets (Port 443): %lu\n", stats.https_packets);
    printf("SSH Packets (Port 22): %lu\n", stats.ssh_packets);
    printf("################################################################\n\n");
    
    // Print connection table
    printf("################################################################\n");
    printf("              TCP CONNECTIONS SUMMARY                          \n");
    printf("################################################################\n");
    printf("Total Unique Connections: %d\n\n", connection_count);
    
    for (int i = 0; i < connection_count && i < 20; i++) {
        printf("Connection %d:\n", i + 1);
        printf("  %s:%u -> %s:%u\n", 
               connections[i].src_ip, connections[i].src_port,
               connections[i].dst_ip, connections[i].dst_port);
        printf("  Packets: %lu, Bytes: %lu\n", 
               connections[i].packets, connections[i].bytes);
        printf("  Duration: %ld seconds\n\n", 
               connections[i].last_seen - connections[i].first_seen);
    }
    printf("################################################################\n\n");
    
    // Cleanup
    if (log_file) fclose(log_file);
    if (sock_raw) close(sock_raw);
    if (buffer) free(buffer);
    
    exit(0);
}

// Function to find or create connection entry
int find_connection(char *src_ip, char *dst_ip, unsigned short src_port, unsigned short dst_port) {
    for (int i = 0; i < connection_count; i++) {
        if ((strcmp(connections[i].src_ip, src_ip) == 0 &&
             strcmp(connections[i].dst_ip, dst_ip) == 0 &&
             connections[i].src_port == src_port &&
             connections[i].dst_port == dst_port) ||
            (strcmp(connections[i].src_ip, dst_ip) == 0 &&
             strcmp(connections[i].dst_ip, src_ip) == 0 &&
             connections[i].src_port == dst_port &&
             connections[i].dst_port == src_port)) {
            return i;
        }
    }
    
    // Create new connection
    if (connection_count < 1000) {
        strcpy(connections[connection_count].src_ip, src_ip);
        strcpy(connections[connection_count].dst_ip, dst_ip);
        connections[connection_count].src_port = src_port;
        connections[connection_count].dst_port = dst_port;
        connections[connection_count].packets = 0;
        connections[connection_count].bytes = 0;
        connections[connection_count].first_seen = time(NULL);
        return connection_count++;
    }
    
    return -1;
}

// Function to analyze TCP packet
void analyze_tcp_packet(unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    unsigned short iphdrlen = iph->ihl * 4;
    
    struct tcphdr *tcph = (struct tcphdr *)(buffer + iphdrlen + sizeof(struct ethhdr));
    
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
    
    unsigned short src_port = ntohs(tcph->source);
    unsigned short dst_port = ntohs(tcph->dest);
    
    // Update statistics
    stats.total_packets++;
    stats.total_bytes += size;
    
    if (tcph->syn && !tcph->ack) stats.syn_packets++;
    if (tcph->syn && tcph->ack) stats.syn_ack_packets++;
    if (tcph->ack) stats.ack_packets++;
    if (tcph->fin) stats.fin_packets++;
    if (tcph->rst) stats.rst_packets++;
    
    // Check for common protocols
    if (src_port == 80 || dst_port == 80) stats.http_packets++;
    if (src_port == 443 || dst_port == 443) stats.https_packets++;
    if (src_port == 22 || dst_port == 22) stats.ssh_packets++;
    
    // Update connection tracking
    int conn_idx = find_connection(inet_ntoa(source.sin_addr), 
                                   inet_ntoa(dest.sin_addr),
                                   src_port, dst_port);
    if (conn_idx >= 0) {
        connections[conn_idx].packets++;
        connections[conn_idx].bytes += size;
        connections[conn_idx].last_seen = time(NULL);
    }
    
    // Print brief packet info
    printf("[PKT %lu] %s:%u -> %s:%u | Flags: ", 
           stats.total_packets,
           inet_ntoa(source.sin_addr), src_port,
           inet_ntoa(dest.sin_addr), dst_port);
    
    if (tcph->syn) printf("SYN ");
    if (tcph->ack) printf("ACK ");
    if (tcph->fin) printf("FIN ");
    if (tcph->rst) printf("RST ");
    if (tcph->psh) printf("PSH ");
    
    printf("| Seq: %u | Size: %d bytes\n", ntohl(tcph->seq), size);
    
    // Log to file
    if (log_file) {
        fprintf(log_file, "%ld,%s,%u,%s,%u,%d,%u,%u,%d,%d,%d,%d,%d\n",
                time(NULL),
                inet_ntoa(source.sin_addr), src_port,
                inet_ntoa(dest.sin_addr), dst_port,
                size, ntohl(tcph->seq), ntohl(tcph->ack_seq),
                tcph->syn, tcph->ack, tcph->fin, tcph->rst, tcph->psh);
        fflush(log_file);
    }
}

int main(int argc, char *argv[]) {
    struct sockaddr saddr;
    int saddr_size, data_size;
    
    // Setup signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    printf("################################################################\n");
    printf("      ADVANCED TCP TRAFFIC ANALYZER - NETWORK SIMULATOR        \n");
    printf("################################################################\n\n");
    
    // Open log file
    log_file = fopen(LOG_FILE, "w");
    if (log_file) {
        fprintf(log_file, "Timestamp,SrcIP,SrcPort,DstIP,DstPort,Size,Seq,Ack,SYN,ACK,FIN,RST,PSH\n");
        printf("[INFO] Logging to: %s\n", LOG_FILE);
    }
    
    // Create raw socket
    sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_raw < 0) {
        perror("[ERROR] Socket creation failed");
        printf("\n[NOTE] This program requires root privileges!\n");
        printf("[CMD] Run with: sudo ./tcp_analyzer_advanced\n\n");
        return 1;
    }
    
    printf("[SUCCESS] Raw socket created\n");
    printf("[INFO] Capturing TCP packets... (Press Ctrl+C to stop)\n");
    printf("################################################################\n\n");
    
    // Allocate buffer
    buffer = (unsigned char *)malloc(BUFFER_SIZE);
    
    // Packet capture loop
    while (1) {
        saddr_size = sizeof(saddr);
        data_size = recvfrom(sock_raw, buffer, BUFFER_SIZE, 0, 
                           &saddr, (socklen_t *)&saddr_size);
        
        if (data_size < 0) {
            perror("[ERROR] Packet receive failed");
            break;
        }
        
        // Check if it's an IP packet
        struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
        if (iph->protocol == IPPROTO_TCP) {
            analyze_tcp_packet(buffer, data_size);
        }
        
        // Print statistics every 50 packets
        if (stats.total_packets % 50 == 0 && stats.total_packets > 0) {
            printf("\n[STATS] Packets: %lu | Bytes: %lu | Connections: %d\n\n",
                   stats.total_packets, stats.total_bytes, connection_count);
        }
    }
    
    signal_handler(SIGINT);
    return 0;
}
