# TCP Traffic Analyzer - Network Simulator

## Compilation Instructions

```bash
# Compile the TCP analyzer (main program)
gcc tcp_packet_analyzer.c -o tcp_analyzer

# Compile the advanced version with filtering
gcc tcp_analyzer_advanced.c -o tcp_analyzer_adv

# Compile test server
gcc tcp_test_server.c -o tcp_server

# Compile test client
gcc tcp_test_client.c -o tcp_client
```

## Running the Programs

### Step 1: Start the TCP Packet Analyzer

```bash
# Basic version
sudo ./tcp_analyzer

# Advanced version with logging
sudo ./tcp_analyzer_adv
```

**Note:** Root/sudo privileges are required to create raw sockets!

### Step 2: Generate TCP Traffic for Testing

**Terminal 1 - Start Server:**
```bash
./tcp_server
```

**Terminal 2 - Start Client:**
```bash
./tcp_client
# Or connect to specific IP
./tcp_client 10.0.0.1
```

**Terminal 3 - Run Analyzer:**
```bash
sudo ./tcp_analyzer_adv
```

### Step 3: Generate Traffic

In the client terminal, type messages and press Enter. The analyzer will capture and display all TCP packets.

## Using with Mininet

```bash
# Start Mininet
sudo mn --topo single,3

# On host h1 (analyzer)
mininet> h1 ./tcp_analyzer_adv &

# On host h2 (server)
mininet> h2 ./tcp_server &

# On host h3 (client)
mininet> h3 ./tcp_client 10.0.0.2
```

## Features Implemented

### Basic Analyzer (tcp_packet_analyzer.c)
✓ Captures packets at Ethernet level using raw sockets
✓ Extracts and displays Ethernet header (Layer 2)
✓ Extracts and displays IP header (Layer 3)
✓ Extracts and displays TCP header (Layer 4)
✓ Displays TCP data payload
✓ Real-time packet statistics
✓ Flag analysis (SYN, ACK, FIN, RST, PSH, URG)

### Advanced Analyzer (tcp_analyzer_advanced.c)
✓ All basic features
✓ Connection tracking and analysis
✓ Protocol identification (HTTP, HTTPS, SSH)
✓ CSV logging to file
✓ Detailed statistics with graceful exit
✓ Connection duration tracking
✓ Bandwidth analysis

## Output Examples

### TCP Packet Header Display
```
################################################################
          TCP PACKET CAPTURED - 19-11-2025 05:17:54
################################################################
Packet Size: 74 bytes

================================================================
                  ETHERNET HEADER (LAYER 2)
================================================================
  |-Destination MAC Address : 00-00-00-00-00-02
  |-Source MAC Address      : 00-00-00-00-00-01
  |-Protocol Type           : 0x0800 (IPv4)
================================================================

================================================================
                    IP HEADER (LAYER 3)
================================================================
  |-IP Version              : 4
  |-IP Header Length        : 20 bytes (IHL: 5)
  |-Type Of Service         : 0x00
  |-IP Total Length         : 60 bytes
  |-Identification          : 0x1234 (4660)
  |-Flags                   : DF
  |-Fragment Offset         : 0
  |-Time To Live (TTL)      : 64 hops
  |-Protocol                : 6 (TCP)
  |-Header Checksum         : 0xb5e3
  |-Source IP Address       : 10.0.0.1
  |-Destination IP Address  : 10.0.0.2
================================================================

================================================================
                    TCP HEADER (LAYER 4)
================================================================
  |-Source Port             : 45678
  |-Destination Port        : 8888
  |-Sequence Number         : 1234567890
  |-Acknowledge Number      : 987654321
  |-Header Length           : 20 bytes (DOFF: 5)
  |-TCP Flags               : SYN
  |-Window Size             : 65535 bytes
  |-Checksum                : 0x1e2f
  |-Urgent Pointer          : 0
================================================================
```

### Statistics Output
```
################################################################
                  TCP TRAFFIC STATISTICS
################################################################
  Total TCP Packets Captured  : 150
  Total Bytes Captured        : 89450 bytes (87.35 KB)
----------------------------------------------------------------
  SYN Packets (Connection)    : 10
  ACK Packets (Acknowledge)   : 140
  PSH Packets (Push Data)     : 75
  FIN Packets (Termination)   : 10
  RST Packets (Reset)         : 0
  URG Packets (Urgent)        : 0
################################################################
```

## Log File Format (tcp_traffic.log)

```csv
Timestamp,SrcIP,SrcPort,DstIP,DstPort,Size,Seq,Ack,SYN,ACK,FIN,RST,PSH
1700374674,10.0.0.1,45678,10.0.0.2,8888,74,1234567890,0,1,0,0,0,0
1700374675,10.0.0.2,8888,10.0.0.1,45678,74,987654321,1234567891,1,1,0,0,0
```

## TCP/IP Libraries Used

1. **socket.h** - Socket creation and management
2. **netinet/ip.h** - IP protocol structures
3. **netinet/tcp.h** - TCP protocol structures
4. **netinet/if_ether.h** - Ethernet frame structures
5. **arpa/inet.h** - Internet address manipulation
6. **net/ethernet.h** - Ethernet definitions
7. **sys/socket.h** - Socket system calls

## Key Structures Used

### IP Header (struct iphdr)
- Version, IHL, TOS
- Total length, ID, Flags
- TTL, Protocol, Checksum
- Source and Destination IP

### TCP Header (struct tcphdr)
- Source and Destination Ports
- Sequence and Acknowledgment Numbers
- Data Offset, Flags
- Window Size, Checksum
- Urgent Pointer

### Ethernet Header (struct ethhdr)
- Destination MAC
- Source MAC
- Protocol Type

## Troubleshooting

### Error: "Operation not permitted"
**Solution:** Run with sudo
```bash
sudo ./tcp_analyzer
```

### Error: "No packets captured"
**Solution:** Check network interface
```bash
# List interfaces
ifconfig -a

# or
ip link show
```

### Error: "Socket creation failed"
**Solution:** Ensure you have CAP_NET_RAW capability
```bash
sudo setcap cap_net_raw+ep ./tcp_analyzer
```

## Learning Outcomes Achieved

✓ Understanding of TCP/IP protocol stack
✓ Knowledge of raw socket programming in C
✓ Experience with packet capture at Ethernet level
✓ Header extraction and analysis skills
✓ TCP connection tracking implementation
✓ Network protocol analysis techniques
✓ Use of C libraries for network programming

## Advanced Usage

### Capture specific interface only
Modify the socket creation to bind to specific interface:
```c
struct ifreq ifr;
memset(&ifr, 0, sizeof(ifr));
snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "eth0");
setsockopt(sock_raw, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr));
```

### Filter by port
Add filtering in the packet handler:
```c
if (ntohs(tcph->source) == 80 || ntohs(tcph->dest) == 80) {
    // Process only HTTP traffic
    process_tcp_packet(buffer, size);
}
```

## References

1. RFC 793 - Transmission Control Protocol (TCP)
2. RFC 791 - Internet Protocol (IP)
3. Linux Socket Programming Manual
4. TCP/IP Illustrated, Volume 1

---
