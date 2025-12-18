# Packet Capture & Analysis Lab

## Overview

This lab demonstrates network packet capture and analysis using Mininet, tcpdump, and a custom C-based packet analyzer. You will create a virtual network topology, capture live traffic, and analyze protocol layers to understand network communication patterns.

## Learning Objectives

- Understand network topology creation using Mininet
- Capture network packets using tcpdump
- Analyze packet headers across OSI layers (Data Link, Network, Transport)
- Parse PCAP files programmatically using libpcap library

---

## Prerequisites

### System Requirements
- Linux-based operating system (Ubuntu recommended)
- Root/sudo privileges
- Mininet installed
- GCC compiler

### Install Required Dependencies

Update package repositories and install the libpcap development library:

```bash
sudo apt-get update
sudo apt-get install libpcap-dev
```

---

## Lab Instructions

### Step 1: Create Network Topology

Create a binary tree topology with 7 switches using Mininet. 

**Topology Details:**
- **Type:** Binary tree
- **Depth:** 3
- **Fanout:** 2
- **Total Switches:** 7 (2⁰ + 2¹ + 2² = 1 + 2 + 4)

**Command:**
```bash
sudo mn --topo tree,depth=3,fanout=2
```

This creates a hierarchical network with: 
- 1 root switch (s1)
- 2 intermediate switches (s2, s3)
- 4 leaf switches (s4, s5, s6, s7)
- 8 hosts (h1-h8)

---

### Step 2: Capture Network Traffic

#### 2.1 Open Terminal Windows

From the Mininet console (`mininet>`), open terminals for the root switch and a host:

```bash
xterm s1 h1
```

Two separate terminal windows will appear:
- **s1 terminal:** For packet capture
- **h1 terminal:** For traffic generation

#### 2.2 Start Packet Capture

In the **s1 terminal** (root switch), run tcpdump to capture packets: 

```bash
tcpdump -i s1-eth1 -w traffic.pcap
```

**Parameters explained:**
- `-i s1-eth1`: Capture on interface s1-eth1 (root switch to branch connection)
- `-w traffic.pcap`: Write captured packets to file

#### 2.3 Generate Network Traffic

In the **h1 terminal**, generate ICMP traffic by pinging host h8:

```bash
ping 10.0.0.8 -c 4
```

**Parameters explained:**
- `10.0.0.8`: IP address of host h8 (leaf node)
- `-c 4`: Send 4 ping packets

#### 2.4 Stop Capture

Return to the **s1 terminal** and stop tcpdump:

```
Press Ctrl+C
```

The file `traffic.pcap` is now saved and ready for analysis.

#### 2.5 Exit Mininet

Close terminal windows and exit Mininet: 

```bash
exit  # In xterm windows
exit  # In Mininet console
```

---

### Step 3: Analyze Captured Packets

#### 3.1 Prepare the Analyzer

Create the packet analyzer source file `packet_analyzer.c` using the code provided in the assignment materials. 

#### 3.2 Compile the Program

Compile with libpcap library linking:

```bash
gcc packet_analyzer.c -o analyzer -lpcap
```

**Compilation flags:**
- `-o analyzer`: Output executable named "analyzer"
- `-lpcap`: Link with libpcap library

#### 3.3 Run the Analyzer

Execute the analyzer on the captured traffic:

```bash
./analyzer traffic.pcap
```

---

## Expected Output

### Time Diagram

The analyzer displays a chronological protocol flow: 

```
---------------- TIME DIAGRAM ----------------
Timestamp        | Protocol Flow
----------------------------------------------
[170000.123456] ETH -> ARP  -> Request/Reply
[170000.124000] ETH -> ARP  -> Request/Reply
[170000.125000] ETH -> IP   -> ICMP (Ping) [Packet Size: 98]
[170000.126000] ETH -> IP   -> ICMP (Ping) [Packet Size: 98]
[170000.127000] ETH -> IP   -> ICMP (Ping) [Packet Size: 98]
[170000.128000] ETH -> IP   -> ICMP (Ping) [Packet Size: 98]
... 
----------------------------------------------
```

### Protocol Summary

```
SUMMARY OF DETECTED PROTOCOLS: 
Layer 2 (Data Link): Ethernet
Layer 3 (Network)  : IPv4, ARP
Layer 4 (Transport): ICMP
```

---

## Results Interpretation

### Protocol Analysis

| Layer | Protocol | Purpose |
|-------|----------|---------|
| **Layer 2** | Ethernet | Frame encapsulation for local network transmission |
| **Layer 3** | ARP | MAC address resolution for IP-to-hardware mapping |
| **Layer 3** | IPv4 | Logical addressing and routing |
| **Layer 4** | ICMP | Network diagnostics (ping/echo request-reply) |

### Traffic Flow Explanation

1. **ARP Packets:** Initial packets are ARP requests/replies to resolve MAC addresses for IP 10.0.0.8
2. **ICMP Packets:** Subsequent packets are ICMP Echo Requests (from h1) and Echo Replies (from h8)
3. **Packet Size:** Typical ICMP packets are 98 bytes (including Ethernet, IP, and ICMP headers)

---

## Troubleshooting

### Common Issues

**Issue:** `tcpdump:  command not found`
```bash
sudo apt-get install tcpdump
```

**Issue:** `permission denied` when running analyzer
```bash
chmod +x analyzer
```

**Issue:** No packets captured
- Verify the correct interface (use `ifconfig` in s1 terminal)
- Ensure traffic generation completed before stopping tcpdump

**Issue:** Compilation errors
- Verify libpcap-dev is installed:  `dpkg -l | grep libpcap`
- Check GCC installation: `gcc --version`

---

## Additional Resources

- [Mininet Documentation](http://mininet.org/)
- [tcpdump Manual](https://www.tcpdump.org/manpages/tcpdump.1.html)
- [libpcap Programming Guide](https://www.tcpdump.org/pcap.html)
- [Wireshark PCAP Analysis](https://www.wireshark.org/)

---

## Lab Deliverables

- [ ] `traffic.pcap` - Captured network traffic file
- [ ] `packet_analyzer.c` - Source code for packet analyzer
- [ ] `analyzer` - Compiled executable
- [ ] Analysis report with protocol summary and observations

---

## Author

**Course:** Computer Networks (CN-14)  
**Assignment:** Assignment 13  
**Repository:** [rizwanshah9792/Networking_Stuffs](https://github.com/rizwanshah9792/Networking_Stuffs)

---

## License

This lab material is provided for educational purposes. 
