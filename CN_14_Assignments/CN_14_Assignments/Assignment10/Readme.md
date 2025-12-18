# 📡 Assignment 10: Custom TCP and ICMP Packet Generation

This assignment demonstrates how to craft and send custom TCP and ICMP packets using raw sockets in a Mininet environment.

## 📋 Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Setup Instructions](#setup-instructions)
- [Step-by-Step Guide](#step-by-step-guide)
- [Verification](#verification)
- [Troubleshooting](#troubleshooting)
- [Resources](#resources)

---

## 🎯 Overview

In this assignment, you will: 

- ✅ Create a simple network topology using Mininet
- ✅ Generate custom TCP packets with embedded data
- ✅ Generate custom ICMP Timestamp Request packets
- ✅ Capture and analyze packets using Wireshark or TCPDump
- ✅ Understand raw socket programming in C

---

## 🔧 Prerequisites

Before starting, ensure you have the following installed:

| Tool | Purpose |
|------|---------|
| **Mininet** | Network emulator |
| **GCC** | C compiler |
| **Wireshark** (Optional) | GUI packet analyzer |
| **TCPDump** | Command-line packet analyzer |
| **Root Access** | Required for raw sockets |

---

## 🚀 Setup Instructions

### Network Topology

```
┌─────────┐         ┌─────────┐
│   h1    │────────│   h2    │
│ Sender  │         │Receiver │
│10.0.0.1 │         │10.0.0.2 │
└─────────┘         └─────────┘
```

---

## 📝 Step-by-Step Guide

### **Step 1: Start Mininet** 🌐

Open your terminal and start a simple topology with 2 hosts:

```bash
sudo mn --topo single,2
```

### **Step 2: Open Host Terminals** 💻

In the Mininet console, open terminal windows for both hosts:

```bash
xterm h1 h2
```

**Host Configuration:**
- **h1** → Sender (10.0.0.1)
- **h2** → Receiver (10.0.0.2)

---

### **Step 3: Setup the Receiver (h2)** 📥

Choose one of the following options to capture packets:

#### **Option A: Using Wireshark** 🔍 (Recommended with GUI)

In the **h2** terminal:

```bash
wireshark &
```

**Configuration Steps:**
1. Double-click the **h2-eth0** interface
2. Apply the filter: `tcp || icmp`
3. Start capturing packets

#### **Option B: Using TCPDump** 📊 (Text-based)

In the **h2** terminal:

```bash
tcpdump -i h2-eth0 -X -v
```

**Flags Explained:**
- `-i h2-eth0` → Interface to listen on
- `-X` → Display packet data in hex and ASCII
- `-v` → Verbose output

---

### **Step 4:  Prepare the Sender (h1)** 📤

#### **A. Create Source Files** 📄

In the **h1** terminal, create the necessary files:

1. **checksum.h** - Checksum calculation header
2. **tcp_raw. c** - TCP packet generator
3. **icmp_raw.c** - ICMP packet generator

Use `nano` or your preferred editor:

```bash
nano checksum.h
nano tcp_raw.c
nano icmp_raw.c
```

#### **B. Compile the Programs** ⚙️

```bash
gcc tcp_raw.c -o tcp_gen
gcc icmp_raw.c -o icmp_gen
```

**Expected Output:**
- Executables: `tcp_gen` and `icmp_gen`
- No compilation errors

---

### **Step 5: Execute TCP Packet Generation** 📨

#### **Run the TCP Generator:**

```bash
sudo ./tcp_gen
```

#### **Expected Behavior:**

✅ **On h1 (Sender):**
- Program sends a custom TCP packet
- Displays confirmation message

✅ **On h2 (Receiver):**
- Wireshark/TCPDump captures the packet
- Look for **TCP payload** containing:  `"RollNo: CS2023-01"`
- Packet appears in the capture list

---

### **Step 6: Execute ICMP Packet Generation** 🔔

#### **Run the ICMP Generator:**

```bash
sudo ./icmp_gen
```

#### **Expected Behavior:**

✅ **On h1 (Sender):**
- Program sends a custom ICMP packet
- Displays confirmation message

✅ **On h2 (Receiver):**
- Wireshark identifies:  **ICMP Timestamp Request (Type 13)**
- TCPDump shows ICMP packet details
- Packet appears with ICMP protocol

---

## ✅ Verification

### Verification Checklist

Use this checklist to ensure everything works correctly:

- [ ] Mininet topology started successfully
- [ ] Both h1 and h2 terminals are open
- [ ] Packet capture running on h2
- [ ] Programs compiled without errors
- [ ] TCP packet captured with Roll Number visible
- [ ] ICMP Timestamp Request (Type 13) captured
- [ ] No critical errors in packet structure

---

### 🔍 Packet Analysis

#### **TCP Packet Structure:**

| Field | Value |
|-------|-------|
| Source IP | 10.0.0.1 |
| Destination IP | 10.0.0.2 |
| Protocol | TCP (6) |
| Payload | "RollNo: CS2023-01" |

#### **ICMP Packet Structure:**

| Field | Value |
|-------|-------|
| Source IP | 10.0.0.1 |
| Destination IP | 10.0.0.2 |
| Protocol | ICMP (1) |
| Type | 13 (Timestamp Request) |
| Code | 0 |

---

## ⚠️ Troubleshooting

### Common Issues and Solutions

#### **1. Checksum Incorrect Warning** ⚠️

**Issue:** Wireshark shows "Header Checksum: 0x0000 [incorrect]"

**Solution:** 
- This is often due to checksum offloading
- If the packet **appears** in the capture, your code works correctly
- For testing purposes, this warning can be ignored

#### **2. Permission Denied** 🚫

**Issue:** "Operation not permitted" when running programs

**Solution:**
```bash
sudo ./tcp_gen
sudo ./icmp_gen
```

#### **3. No Packets Captured** ❌

**Issue:** Receiver doesn't see any packets

**Solutions:**
- Verify interface name:  `ifconfig` on h2
- Check filter syntax in Wireshark
- Ensure programs run without errors
- Verify IP addresses in source code

#### **4. Compilation Errors** 🔧

**Issue:** GCC fails to compile

**Solutions:**
- Check for syntax errors in C files
- Ensure `checksum.h` is in the same directory
- Verify GCC installation:  `gcc --version`

#### **5. Mininet Not Starting** 🛑

**Issue:** `sudo mn` command fails

**Solutions:**
```bash
sudo mn -c  # Clean up previous sessions
sudo service openvswitch-switch restart
```

---

## 📚 Resources

### Documentation

- [Mininet Documentation](http://mininet.org/walkthrough/)
- [Raw Sockets Programming Guide](https://www.binarytides.com/raw-sockets-c-code-linux/)
- [TCP/IP Protocol Suite](https://www.rfc-editor.org/rfc/rfc793)
- [ICMP Specification](https://www.rfc-editor.org/rfc/rfc792)

### Useful Commands

```bash
# View network interfaces
ifconfig

# Check connectivity
ping -c 3 10.0.0.2

# Clean Mininet
sudo mn -c

# View running processes
ps aux | grep mininet
```

---

## 🎓 Learning Outcomes

After completing this assignment, you should understand:

- 🔹 How to create custom network packets using raw sockets
- 🔹 TCP and ICMP packet structure and headers
- 🔹 Network packet capture and analysis techniques
- 🔹 Mininet topology creation and management
- 🔹 Checksum calculation for network protocols

---

## 📝 Notes

> **Important:** This assignment uses raw sockets which require root/sudo privileges. Always run packet generation programs with elevated permissions.

> **Tip:** Keep both h1 and h2 terminals visible side-by-side to observe real-time packet transmission and capture.

---

## 👨‍💻 Assignment Details

- **Course:** Computer Networks (CN-14)
- **Assignment:** 10
- **Topic:** Custom Packet Generation with Raw Sockets
- **Roll Number Format:** CS2023-01 (Update with your actual roll number)

---

**Happy Networking!  🌐✨**
