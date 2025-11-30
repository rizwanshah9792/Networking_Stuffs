#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/tcp.h> // TCP Header definitions
#include <netinet/ip.h>  // IP Header definitions
#include <arpa/inet.h>
#include <unistd.h>
#include "checksum.h"    // Include our helper

// Configuration
#define SRC_IP "10.0.0.1"
#define DST_IP "10.0.0.2"
#define MY_ROLL_NUM "RollNo: CSM24004" 

// A struct to help calculate TCP checksum (Pseudo-header)
struct pseudo_header {
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
};

int main() {
    // 1. Create the Raw Socket
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock == -1) { perror("Failed to create socket"); exit(1); }

    // Buffer to hold our packet
    char packet[4096];
    memset(packet, 0, 4096);

    // Pointers to where headers sit in the packet
    struct iphdr *iph = (struct iphdr *) packet;
    struct tcphdr *tcph = (struct tcphdr *) (packet + sizeof(struct iphdr));
    char *data = packet + sizeof(struct iphdr) + sizeof(struct tcphdr);

    // 2. Add the Payload (Roll Number)
    strcpy(data, MY_ROLL_NUM);
    int data_len = strlen(data);

    // 3. Fill in the IP Header (The Envelope)
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + data_len;
    iph->id = htonl(54321);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->saddr = inet_addr(SRC_IP);
    iph->daddr = inet_addr(DST_IP);
    iph->check = 0; // Kernel fills this usually, but good practice to init 

    // 4. Fill in the TCP Header (The Letter)
    tcph->source = htons(12345); // Random source port
    tcph->dest = htons(80);      // Destination port (like HTTP)
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5;  // Header size
    tcph->fin = 0;
    tcph->syn = 1;   // Let's make it a SYN packet (start connection)
    tcph->rst = 0;
    tcph->psh = 0;
    tcph->ack = 0;
    tcph->urg = 0;
    tcph->window = htons(5840);
    tcph->check = 0; // Leave 0 for now, filling later
    tcph->urg_ptr = 0;

    // 5. Calculate TCP Checksum (Optional for basic viewing, but required for validity)
    // We need a pseudo header for TCP checksums
    struct pseudo_header psh;
    psh.source_address = inet_addr(SRC_IP);
    psh.dest_address = inet_addr(DST_IP);
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr) + data_len);
    
    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + data_len;
    char *pseudogram = malloc(psize);
    memcpy(pseudogram , (char*) &psh , sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr) + data_len);
    
    tcph->check = checksum((unsigned short*) pseudogram , psize);

    // 6. Send the packet
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(DST_IP);

    // Tell the socket WE provided the IP header (IP_HDRINCL)
    int one = 1;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("Error setting IP_HDRINCL");
        exit(0);
    }

    if (sendto(sock, packet, iph->tot_len, 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        perror("Send failed");
    } else {
        printf("TCP Packet with Roll No sent to %s\n", DST_IP);
    }

    close(sock);
    return 0;
}
