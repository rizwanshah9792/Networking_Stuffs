# 🔬 Packet Generation & Analysis Lab

## 📋 Assignment Overview

This lab focuses on installing and using three essential networking tools for packet generation and analysis. 

---

## 📥 Step 1: Installation & Setup

Install the following tools on your system:

### Required Tools:
1. **Iperf3** - The Speed Tester 🚀
2. **Hping3** - The Custom Crafter 🔨
3. **D-ITG** - The Realism Mimic 🎭

---

## 🛠️ Tool 1: Iperf3 (The Speed Tester)

**Purpose:** Network speed and performance testing

### Installation
```bash
# Ubuntu/Debian
sudo apt-get install iperf3

# macOS
brew install iperf3

# Windows
# Download from official website
```

### Usage Examples
```bash
# Server mode
iperf3 -s

# Client mode
iperf3 -c <server-ip>

# Custom bandwidth test
iperf3 -c <server-ip> -b 100M -t 60
```

---

## 🛠️ Tool 2: Hping3 (The Custom Crafter)

**Purpose:** Advanced packet crafting and network testing

### Installation
```bash
# Ubuntu/Debian
sudo apt-get install hping3

# macOS
brew install hping

# Compile from source if needed
```

### Usage Examples
```bash
# TCP SYN scan
sudo hping3 -S <target-ip> -p 80

# UDP flood
sudo hping3 --udp <target-ip> -p 53

# Custom packet crafting
sudo hping3 -c 10 -d 120 -S -p 80 <target-ip>
```

---

## 🛠️ Tool 3: D-ITG (The Realism Mimic)

**Purpose:** Realistic traffic generation for network testing

### Installation
```bash
# Download from official source
wget http://traffic.comics.unina.it/software/ITG/codice/D-ITG-2.8.1-r1023-src.zip

# Extract and compile
unzip D-ITG-2.8.1-r1023-src. zip
cd D-ITG-2.8.1-r1023
make

# Install
sudo make install
```

### Usage Examples
```bash
# Start receiver
ITGRecv

# Start sender with specific traffic pattern
ITGSend -a <receiver-ip> -T TCP -C 100 -c 500 -t 30000

# Generate VoIP-like traffic
ITGSend -a <receiver-ip> -E 8000 -e 512 -T UDP
```

---

## 🎯 Learning Outcomes

By completing this lab, you will be able to: 

- ✅ Install and configure network analysis tools
- ✅ Generate custom network packets
- ✅ Analyze network performance and bandwidth
- ✅ Understand different traffic patterns
- ✅ Perform basic network testing and diagnostics

---

## 📊 Lab Tasks

### Task 1: Speed Testing with Iperf3
- Set up client-server communication
- Measure TCP/UDP throughput
- Document bandwidth results

### Task 2: Packet Crafting with Hping3
- Create custom TCP/UDP packets
- Perform port scanning
- Test firewall responses

### Task 3: Traffic Generation with D-ITG
- Generate realistic application traffic
- Simulate VoIP, video streaming
- Analyze traffic patterns

---

## 📝 Report Requirements

Your lab report should include:

1. **Installation screenshots** for all three tools
2. **Command outputs** with explanations
3. **Performance measurements** and graphs
4. **Analysis** of results
5. **Challenges faced** and solutions

---

## ⚠️ Important Notes

- Always use these tools in **authorized environments only**
- Obtain proper **permissions** before testing on any network
- These tools can be used for **educational purposes** only
- Understand **legal implications** of network testing

---

## 📚 Additional Resources

- [Iperf3 Documentation](https://iperf.fr/)
- [Hping3 Manual](http://www.hping.org/)
- [D-ITG Official Site](http://traffic.comics.unina.it/software/ITG/)

---
