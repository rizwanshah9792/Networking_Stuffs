# **Mininet Lab: ICMP Traffic Analysis**

## **Overview**

This lab demonstrates ICMP packet crafting and IP spoofing using Mininet. The attacker (h1) sends crafted ICMP packets with spoofed source IP addresses to the victim (h2), making it appear as if the traffic is coming from other hosts (h3-h6).

---

## **Topology**

- **h1**: Attacker (10.0.0.1)
- **h2**: Victim (10.0.0.2)
- **h3-h6**:  Spoofed Agents (10.0.0.3 - 10.0.0.6)

---

## **Step 1: Create Topology**

Start Mininet with a single switch and 6 hosts: 

```bash
sudo mn --topo single,6
```

---

## **Step 2: Open Terminals**

Open terminals for the Attacker and Victim: 

```bash
mininet> xterm h1 h2
```

---

## **Step 3: Setup Monitoring (h2 - Victim)**

On the **Victim machine (h2)**, use `tcpdump` to monitor incoming ICMP traffic:

```bash
# -i h2-eth0: Listen on h2's network interface
# -n: Don't resolve hostnames
# icmp: Filter only ICMP packets
tcpdump -i h2-eth0 -n icmp
```

**Keep this terminal open** to observe incoming packets in real-time.

---

## **Step 4: Execute Packet Crafting (h1 - Attacker)**

On the **Attacker machine (h1)**, compile and run the ICMP packet crafter. 

### **4.1 Compile the Tool**

```bash
gcc icmp_crafter.c -o icmp_crafter
```

### **4.2 Simulate Traffic from Multiple Agents**

Run the tool **4 times**, each time spoofing a different source IP address to simulate traffic from h3, h4, h5, and h6:

```bash
# Pretend to be h3 (Agent 1)
sudo ./icmp_crafter 10.0.0.3 10.0.0.2

# Pretend to be h4 (Agent 2)
sudo ./icmp_crafter 10.0.0.4 10.0.0.2

# Pretend to be h5 (Agent 3)
sudo ./icmp_crafter 10.0.0.5 10.0.0.2

# Pretend to be h6 (Agent 4)
sudo ./icmp_crafter 10.0.0.6 10.0.0.2
```

**Syntax:** `./icmp_crafter <spoofed_source_ip> <destination_ip>`

---

## **Step 5: Analyze Results**

Switch to the **h2 (Victim)** terminal running `tcpdump`. You should see ICMP echo requests arriving from four different IP addresses:

```
IP 10.0.0.3 > 10.0.0.2: ICMP echo request, id 1234, seq 1, length 8
IP 10.0.0.4 > 10.0.0.2: ICMP echo request, id 1234, seq 1, length 8
IP 10.0.0.5 > 10.0.0.2: ICMP echo request, id 1234, seq 1, length 8
IP 10.0.0.6 > 10.0.0.2: ICMP echo request, id 1234, seq 1, length 8
```

### **Key Observations:**

1. **Spoofed Source IPs**: The victim (h2) receives packets that appear to come from h3, h4, h5, and h6, but they were actually sent by h1.

2. **Echo Replies**: h2 will automatically send ICMP echo replies to the spoofed IP addresses (10.0.0.3-10.0.0.6), not to the actual attacker (h1).

3. **Attacker Anonymity**: This demonstrates how IP spoofing can hide the true source of network traffic.

---

## **Learning Objectives**

- Understand how raw sockets can be used to craft custom packets
- Learn about IP spoofing and its security implications
- Observe how ICMP echo request/reply mechanism works
- Recognize the difference between actual and apparent packet sources

---

## **Security Implications**

This lab demonstrates techniques that can be used maliciously for:
- **DDoS Amplification Attacks**: Using spoofed IPs to redirect responses to a victim
- **Smurf Attacks**:  ICMP-based distributed denial of service
- **Identity Concealment**: Hiding the true source of malicious traffic

**⚠️ WARNING:** Use this knowledge responsibly and only in controlled lab environments.  Unauthorized packet spoofing is illegal and unethical.

---

## **Cleanup**

Exit Mininet:

```bash
mininet> exit
```

Clean up any residual processes:

```bash
sudo mn -c
```

---

## **Requirements**

- Mininet installed
- GCC compiler
- Root/sudo privileges (required for raw socket operations)
- `icmp_crafter. c` source code

---

## **Troubleshooting**

### Permission Denied Error
If you get a permission error, make sure to run the crafter with `sudo`:
```bash
sudo ./icmp_crafter 10.0.0.3 10.0.0.2
```

### No Packets Showing on tcpdump
- Verify tcpdump is running on the correct interface (`h2-eth0`)
- Ensure the ICMP crafter is running successfully on h1
- Check that IP addresses match the topology

### Compilation Errors
Make sure you have the necessary headers:
```bash
sudo apt-get install build-essential
```

---

## **Additional Exercises**

1. **Modify the payload**: Edit `icmp_crafter.c` to send custom data in the ICMP payload
2. **Change packet count**: Send multiple packets from each spoofed IP
3. **Add timestamp**: Include timestamps to measure response times
4. **Capture to file**: Save tcpdump output to a pcap file for analysis in Wireshark: 
   ```bash
   tcpdump -i h2-eth0 -n icmp -w capture.pcap
   ```

---

## **References**

- [RFC 792 - Internet Control Message Protocol](https://tools.ietf.org/html/rfc792)
- [Mininet Documentation](http://mininet.org/)
- [Raw Sockets Programming Guide](http://man7.org/linux/man-pages/man7/raw.7.html)
