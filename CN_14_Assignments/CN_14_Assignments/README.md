# CN 14 Assignments - Computer Networks Lab

This repository contains 14 Computer Networks laboratory assignments covering socket programming, network analysis, packet crafting, and network topology design using C, Python, and Mininet.

## Table of Contents

| # | Assignment | Topic | Technologies |
|---|------------|-------|--------------|
| 1 | [Assignment 1](#assignment-1-basic-tcp-client-server) | Basic TCP Client-Server | C, TCP Sockets |
| 2 | [Assignment 2](#assignment-2-concurrent-tcp-fruit-store) | Concurrent TCP Fruit Store | C, TCP, Shared Memory, Semaphores |
| 3 | [Assignment 3](#assignment-3-udp-fruit-store) | UDP Fruit Store | C, UDP Sockets, Mininet |
| 4 | [Assignment 4](#assignment-4-packet-capture-with-wireshark) | Packet Capture with Wireshark | Wireshark, PCAP |
| 5 | [Assignment 5](#assignment-5-network-traffic-tools) | Network Traffic Tools | iperf3, hping3, D-ITG |
| 6 | [Assignment 6](#assignment-6-tcp-traffic-analyzer) | TCP Traffic Analyzer | C, Raw Sockets |
| 7 | [Assignment 7](#assignment-7-udp-calculator-with-timeout) | UDP Calculator with Timeout | C, UDP Sockets, Mininet |
| 8 | [Assignment 8](#assignment-8-multi-client-chat-server) | Multi-Client Chat Server | C, TCP, Pthreads |
| 9 | [Assignment 9](#assignment-9-tcp-file-transfer) | TCP File Transfer | C, TCP Sockets, Mininet |
| 10 | [Assignment 10](#assignment-10-raw-socket-packet-crafting) | Raw Socket Packet Crafting | C, Raw Sockets, TCP/ICMP |
| 11 | [Assignment 11](#assignment-11-tcp-syn-attack-simulation) | TCP SYN Attack Simulation | C, Raw Sockets, IP Spoofing |
| 12 | [Assignment 12](#assignment-12-icmp-packet-crafting) | ICMP Packet Crafting | C, Raw Sockets, ICMP |
| 13 | [Assignment 13](#assignment-13-packet-capture-and-analysis) | Packet Capture & Analysis | C, libpcap |
| 14 | [Assignment 14](#assignment-14-leaf-spine-topology) | Leaf-Spine Topology | Python, Mininet |

---

## Assignment 1: Basic TCP Client-Server

**Objective:** Implement a basic TCP client-server communication.

**Files:**
- `server.c` - TCP server that listens on port 8080, accepts a connection, receives a message, and sends a reply
- `client.c` - TCP client that connects to the server, sends "Hi", and receives "Hello"

**Compilation:**
```bash
gcc server.c -o server
gcc client.c -o client
```

**Execution:**
```bash
# Terminal 1 (Server)
./server

# Terminal 2 (Client)
./client
```

---

## Assignment 2: Concurrent TCP Fruit Store

**Objective:** Build a concurrent TCP server for a fruit store with shared memory and semaphores for process synchronization.

**Features:**
- Shared memory for inventory management across multiple child processes
- Semaphore-based locking for process-safe operations
- Multi-client support using `fork()`
- Tracks unique customers and purchase transactions

**Files:**
- `server.c` - Concurrent TCP server with shared memory inventory
- `client.c` - Interactive client for purchasing fruits

**Compilation:**
```bash
gcc server.c -o server
gcc client.c -o client
```

**Usage:**
```bash
# Server
./server

# Client (specify server IP)
./client <server_ip>
# Commands: BUY <fruit> <quantity>, EXIT
```

---

## Assignment 3: UDP Fruit Store

**Objective:** Implement a UDP-based fruit store with connectionless communication.

**Features:**
- UDP connectionless communication
- Fruit inventory with timestamps
- Track unique customer IDs (IP:Port)
- Stock viewing and purchasing functionality

**Files:**
- `server.c` - UDP fruit store server
- `client.c` - UDP client for interacting with the store
- `Readme.md` - Detailed instructions

**Compilation:**
```bash
gcc server.c -o server
gcc client.c -o client
```

**Mininet Execution:**
```bash
sudo mn --topo single,3
# h1: ./server &
# h2: ./client 10.0.0.1
# h3: ./client 10.0.0.1
```

---

## Assignment 4: Packet Capture with Wireshark

**Objective:** Capture and analyze network packets using Wireshark.

**Files:**
- `packet captures (use ping).pcapng` - Captured packet file
- `Assignment4.pdf` - Detailed assignment documentation
- `output_ass4.png` - Output screenshot

---

## Assignment 5: Network Traffic Tools

**Objective:** Explore network traffic generation and performance testing tools.

**Tools Used:**
- **iperf3** - Network speed and throughput testing
- **hping3** - Custom packet flooding
- **D-ITG** - Network traffic generation and performance testing

**Files:**
- `Assignment5.pdf` - Documentation
- Multiple output screenshots demonstrating each tool

---

## Assignment 6: TCP Traffic Analyzer

**Objective:** Build a TCP packet analyzer using raw sockets to capture and analyze network traffic at different OSI layers.

**Features:**
- Captures packets at Ethernet level (Layer 2)
- Extracts Ethernet, IP (Layer 3), and TCP (Layer 4) headers
- Displays TCP flags (SYN, ACK, FIN, RST, PSH, URG)
- Advanced version with CSV logging and connection tracking

**Files:**
- `tcp_packet_analyzer.c` - Basic TCP analyzer
- `tcp_analyzer_advanced.c` - Advanced version with logging
- `tcp_test_server.c` / `tcp_test_client.c` - Test traffic generators
- `readme.md` - Comprehensive documentation

**Compilation:**
```bash
gcc tcp_packet_analyzer.c -o tcp_analyzer
gcc tcp_analyzer_advanced.c -o tcp_analyzer_adv
gcc tcp_test_server.c -o tcp_server
gcc tcp_test_client.c -o tcp_client
```

**Note:** Requires root/sudo privileges for raw socket access.

---

## Assignment 7: UDP Calculator with Timeout

**Objective:** Implement a UDP-based scientific calculator with timeout handling for packet loss detection.

**Features:**
- Basic operations: add, sub, mul, div
- Scientific operations: sin, cos, tan, sqrt, pow
- Timeout mechanism to detect packet loss

**Files:**
- `server.c` - UDP calculator server
- `client.c` - UDP client with timeout handling
- `Readme.md` - Execution instructions

**Compilation:**
```bash
gcc server.c -o server -lm
gcc client.c -o client
```

**Testing with Packet Loss:**
```bash
sudo mn --topo single,2 --link tc,loss=20
```

---

## Assignment 8: Multi-Client Chat Server

**Objective:** Build a multi-threaded chat server supporting multiple concurrent clients.

**Features:**
- Pthread-based multi-client handling
- Message broadcasting to all connected clients
- Chat logging to `log.txt`
- Thread-safe client management

**Files:**
- `server.c` - Multi-threaded chat server
- `client.c` - Chat client
- `Readme.md` - Instructions

**Compilation:**
```bash
gcc server.c -o server -pthread
gcc client.c -o client -pthread
```

**Mininet Execution:**
```bash
sudo mn --topo single,3 --mac
# h1: ./server
# h2: ./client 10.0.0.1  (Enter name: Alice)
# h3: ./client 10.0.0.1  (Enter name: Bob)
```

---

## Assignment 9: TCP File Transfer

**Objective:** Implement bidirectional file transfer over TCP with transfer time measurement.

**Features:**
- File upload from client to server
- File download from server to client
- Transfer time measurement

**Files:**
- `server_folder/` - Server code and files
- `client_folder/` - Client code and files
- `Readme_of_Assignment9.md` - Detailed guide

**Mininet Execution:**
```bash
sudo mn --topo single,2
xterm h1 h2
# h1: Setup server
# h2: Setup client
```

---

## Assignment 10: Raw Socket Packet Crafting

**Objective:** Craft custom TCP and ICMP packets using raw sockets.

**Features:**
- Custom TCP packet with embedded data (Roll Number)
- ICMP Timestamp Request generation
- Manual checksum calculation

**Files:**
- `tcp_raw.c` - TCP packet crafter
- `icmp_raw.c` - ICMP packet crafter
- `checksum.h` - Checksum utility header
- `Readme_of_Assignment10.md` - Instructions

**Compilation:**
```bash
gcc tcp_raw.c -o tcp_gen
gcc icmp_raw.c -o icmp_gen
```

---

## Assignment 11: TCP SYN Attack Simulation

**Objective:** Simulate a TCP SYN flood attack to understand DDoS mechanics (educational purposes only).

**Features:**
- Crafts TCP SYN packets with spoofed source IPs
- Demonstrates IP spoofing techniques
- Analyzes attack patterns with TCPDump/Wireshark

**Files:**
- `syn_crafter.c` - SYN packet crafter
- `Readme_of_Assignment11.md` - Lab guide

**Compilation:**
```bash
gcc syn_crafter.c -o syn_crafter
```

**Mininet Execution:**
```bash
sudo mn --topo single,6
# h1 (Attacker): ./syn_crafter <spoofed_src_ip> <victim_ip>
# h2 (Victim): tcpdump -i h2-eth0 -n "tcp[tcpflags] & (tcp-syn) != 0"
```

---

## Assignment 12: ICMP Packet Crafting

**Objective:** Craft custom ICMP Echo Request packets with spoofed source IPs.

**Features:**
- ICMP Echo Request generation
- IP spoofing for ICMP packets
- Traffic analysis with TCPDump

**Files:**
- `icmp_crafter.c` - ICMP packet crafter
- `Readme_for_Assignment12.md` - Lab guide

**Compilation:**
```bash
gcc icmp_crafter.c -o icmp_crafter
```

**Usage:**
```bash
./icmp_crafter <source_ip> <destination_ip>
```

---

## Assignment 13: Packet Capture and Analysis

**Objective:** Capture and analyze network packets using libpcap to create a time diagram of protocol flows.

**Features:**
- Binary tree topology (7 switches) in Mininet
- Packet capture at root switch
- Protocol analysis (Ethernet, ARP, IPv4, ICMP)
- Time diagram visualization

**Files:**
- `packet_analyzer.c` - Packet analysis program
- `traffic.pcap` / `traffic2.pcap` - Captured traffic files
- `Readme_for_Assignment13.md` - Detailed guide

**Prerequisites:**
```bash
sudo apt-get install libpcap-dev
```

**Compilation:**
```bash
gcc packet_analyzer.c -o analyzer -lpcap
```

**Usage:**
```bash
./analyzer traffic.pcap
```

---

## Assignment 14: Leaf-Spine Topology

**Objective:** Design a scalable Leaf-Spine network topology using Python and Mininet.

**Features:**
- Scalable architecture (k parameter controls radix)
- Full mesh connectivity between spine and leaf switches
- Non-blocking network design

**Topology Structure:**
- Spine Switches = k
- Leaf Switches = k × 2
- Hosts = k × 2 (one host per leaf switch)

**Files:**
- `leaf_spine_topology.py` - Python topology definition
- `Readme_for_Assignment14.md` - Lab guide

**Execution:**
```bash
# Default (k=2): 2 Spine Switches, 4 Leaf Switches
sudo mn --custom leaf_spine_topology.py --topo ls --controller=none

# Scaled (k=4): 4 Spine Switches, 8 Leaf Switches
sudo mn --custom leaf_spine_topology.py --topo ls,k=4 --controller=none

# Verify
mininet> net
mininet> pingall
```

---

## Prerequisites

### System Requirements
- Linux-based OS (Ubuntu recommended)
- Mininet for network simulation
- GCC compiler
- Wireshark for packet analysis

### Libraries
```bash
# Essential packages
sudo apt-get update
sudo apt-get install build-essential mininet wireshark

# For Assignment 13 (libpcap)
sudo apt-get install libpcap-dev

# For network tools (Assignment 5)
sudo apt-get install iperf3 hping3
```

---

## Learning Outcomes

1. **Socket Programming** - TCP and UDP socket creation, binding, listening, and data transfer
2. **Raw Sockets** - Low-level packet crafting and network protocol implementation
3. **Concurrency** - Multi-process (fork) and multi-threaded (pthread) programming
4. **Network Analysis** - Packet capture, protocol dissection, and traffic analysis
5. **Network Simulation** - Mininet topology creation and network testing
6. **Security Concepts** - Understanding of DDoS mechanics and IP spoofing (educational)

---

## References

- RFC 793 - Transmission Control Protocol (TCP)
- RFC 791 - Internet Protocol (IP)
- RFC 792 - Internet Control Message Protocol (ICMP)
- Linux Socket Programming Manual
- Mininet Documentation
