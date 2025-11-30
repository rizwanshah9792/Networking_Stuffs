#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h> // ICMP definitions
#include <arpa/inet.h>
#include <unistd.h>
#include "checksum.h"

#define SRC_IP "10.0.0.1"
#define DST_IP "10.0.0.2"

int main() {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sock == -1) { perror("Failed to create socket"); exit(1); }

    char packet[4096];
    memset(packet, 0, 4096);

    struct iphdr *iph = (struct iphdr *) packet;
    struct icmp *icmph = (struct icmp *) (packet + sizeof(struct iphdr));

    // 1. Fill IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct icmp);
    iph->id = htons(12345);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_ICMP;
    iph->saddr = inet_addr(SRC_IP);
    iph->daddr = inet_addr(DST_IP);
    iph->check = 0;

    // 2. Fill ICMP Header (Timestamp Request)
    icmph->icmp_type = 13; // Type 13 is Timestamp Request
    icmph->icmp_code = 0;
    icmph->icmp_id = htons(1001);
    icmph->icmp_seq = htons(1);
    
    // Timestamp fields (Originate, Receive, Transmit)
    // We just fill them with dummy numbers for the assignment
    icmph->icmp_otime = htonl(1000); 
    icmph->icmp_rtime = htonl(0);
    icmph->icmp_ttime = htonl(0);

    // Calculate Checksum for ICMP
    icmph->icmp_cksum = checksum((unsigned short *)icmph, sizeof(struct icmp));

    // 3. Send
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(DST_IP);

    int one = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));

    if (sendto(sock, packet, iph->tot_len, 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        perror("Send failed");
    } else {
        printf("ICMP Timestamp Request sent to %s\n", DST_IP);
    }

    close(sock);
    return 0;
}
