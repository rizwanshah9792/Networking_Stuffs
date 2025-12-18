# 🌐 Wireshark Packet Analysis Lab

## 📋 Overview

This lab focuses on analyzing network packets using Wireshark to understand the structure and behavior of network protocols. The assignment involves capturing ICMP ping packets and analyzing their layer-by-layer structure.

## 🎯 Objectives

- Set up and install Wireshark for network packet analysis
- Capture ping (ICMP) packets between local machine and a remote server
- Create time diagrams to visualize packet flow
- Perform detailed header analysis of captured packets
- Understand the OSI model layers in practical network communication

---

## 🛠️ Setup & Installation

### Requirements
- Wireshark (latest version)
- Administrator/root access for packet capturing
- Active internet connection
- Terminal/Command Prompt access

### Installation Steps
1. Download Wireshark from the official website
2. Install with administrator privileges
3. Configure network interface permissions
4. Verify installation by launching Wireshark

---

## 🧪 The Experiment (Capturing the Ping)

### Procedure

1. **Select Network Interface**
   - Open Wireshark
   - Choose the active network interface (e.g., Wi-Fi, Ethernet)
   - Start packet capture

2. **Generate ICMP Traffic**
   - Open terminal/command prompt
   - Execute ping command: `ping [target_host]`
   - Let it run for several packets
   - Stop the ping command

3. **Filter Captured Packets**
   - In Wireshark, apply filter: `icmp`
   - Locate request and reply packets
   - Select specific packets for analysis

4. **Save Capture**
   - Stop packet capture in Wireshark
   - Save the capture file (. pcap format)
   - Document observations

---

## 📊 The Time Diagram

A time diagram visualizes the sequence of network communication:

```
Client                                    Server
  |                                         |
  |-------- ICMP Echo Request --------->   |
  |         (ping packet)                   |
  |                                         |
  |<------- ICMP Echo Reply -----------    |
  |         (response packet)               |
  |                                         |
```

### Key Observations
- Round-trip time (RTT)
- Packet sequence numbers
- Time-to-Live (TTL) values
- Packet loss (if any)

---

## 🔍 Header Analysis (The Anatomy of a Packet)

### Layer 2:  Data Link Layer (Ethernet II)

**Fields Analyzed:**
- **Destination MAC Address**: The hardware address of the receiving device
- **Source MAC Address**:  The hardware address of the sending device
- **EtherType**: Protocol identifier (0x0800 for IPv4)

**Purpose**:  Provides local network delivery between devices on the same network segment. 

---

### Layer 3: Network Layer (IPv4)

**Fields Analyzed:**
- **Version**: IP version (4)
- **Header Length**: Size of the IP header
- **Type of Service (ToS)**: Priority and handling preferences
- **Total Length**: Complete packet size
- **Identification**:  Unique packet identifier
- **Flags**:  Fragmentation control
- **Fragment Offset**: Position in fragmented data
- **Time to Live (TTL)**: Maximum number of hops
- **Protocol**: Next layer protocol (1 for ICMP)
- **Header Checksum**: Error detection
- **Source IP Address**:  Sender's IP address
- **Destination IP Address**: Receiver's IP address

**Purpose**: Handles logical addressing and routing between different networks.

---

### Layer 4 / Payload:  ICMP (Internet Control Message Protocol)

**Fields Analyzed:**
- **Type**: Message type (8 for Echo Request, 0 for Echo Reply)
- **Code**: Subtype of the ICMP message
- **Checksum**: Error detection for ICMP header and data
- **Identifier**:  Matches requests with replies
- **Sequence Number**:  Tracks individual ping packets
- **Data**:  Payload sent with the ping (usually random or timestamp)

**Purpose**: Used for diagnostic and error reporting in IP networks.

---

## 📚 Learning Outcomes

By completing this lab, you will: 

1. **Understand Network Protocols**
   - Gain practical knowledge of TCP/IP protocol stack
   - Understand how different layers interact

2. **Develop Analysis Skills**
   - Learn to use Wireshark effectively
   - Interpret packet headers and data

3. **Troubleshooting Capabilities**
   - Diagnose network connectivity issues
   - Identify packet loss or delays
   - Understand network performance metrics

---

## 🧰 Tools Used

- **Wireshark**:  Network protocol analyzer
- **Ping**: Network diagnostic tool
- **Terminal/Command Prompt**: Command-line interface

---

## 📝 Report Components

Your lab report should include:

1. Screenshots of captured packets
2. Time diagrams showing packet flow
3. Detailed analysis of each protocol layer
4. Observations and findings
5. Answers to any assignment questions
6. Conclusions about network behavior

---

## 🎓 Additional Resources

- [Wireshark Official Documentation](https://www.wireshark.org/docs/)
- [ICMP Protocol Specification (RFC 792)](https://tools.ietf.org/html/rfc792)
- [TCP/IP Protocol Suite Overview](https://en.wikipedia.org/wiki/Internet_protocol_suite)

---

## ⚠️ Important Notes

- Always ensure you have permission before capturing network traffic
- Be aware of privacy and security considerations
- Some networks may block ICMP packets
- Packet capture requires elevated privileges
