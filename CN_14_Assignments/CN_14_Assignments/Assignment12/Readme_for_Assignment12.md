# **Mininet Lab: ICMP Traffic Analysis**

## **Step 1: Create Topology**

Start Mininet with a single switch and 6 hosts.

* **h1**: Attacker  
* **h2**: Victim  
* **h3-h6**: The IPs we will spoof (the "Agents")

sudo mn \--topo single,6

## **Step 2: Open Terminals**

Open the terminals for the Attacker and Victim.

mininet\> xterm h1 h2

## **Step 3: Setup Monitoring (h2 \- Victim)**

On the Victim machine (h2), use TCPDump to watch for incoming ICMP traffic.

\# \-n: Don't resolve hostnames  
\# icmp: Filter only ICMP packets  
tcpdump \-i h2-eth0 \-n icmp

## **Step 4: Execute Packet Crafting (h1 \- Attacker)**

On the Attacker machine (h1), compile and run the tool.

1. **Compile:**  
   gcc icmp\_crafter.c \-o icmp\_crafter

2. Simulate the Agents:  
   Run the tool 4 times, changing the Source IP (first argument) each time to match the IPs of hosts h3, h4, h5, and h6. This mimics traffic coming from 4 different devices.  
   \# Pretend to be h3 (Agent 1\)  
   ./icmp\_crafter 10.0.0.3 10.0.0.2

   \# Pretend to be h4 (Agent 2\)  
   ./icmp\_crafter 10.0.0.4 10.0.0.2

   \# Pretend to be h5 (Agent 3\)  
   ./icmp\_crafter 10.0.0.5 10.0.0.2

   \# Pretend to be h6 (Agent 4\)  
   ./icmp\_crafter 10.0.0.6 10.0.0.2

## **Step 5: Analyze Results**

Look at the **h2 (Victim)** terminal. You should see requests arriving from four different IP addresses:

IP 10.0.0.3 \> 10.0.0.2: ICMP echo request, id 1234, seq 1, length 8  
IP 10.0.0.4 \> 10.0.0.2: ICMP echo request, id 1234, seq 1, length 8  
IP 10.0.0.5 \> 10.0.0.2: ICMP echo request, id 1234, seq 1, length 8  
IP 10.0.0.6 \> 10.0.0.2: ICMP echo request, id 1234, seq 1, length 8

Notice that **h2** will automatically try to reply to these IPs. The "Echo Reply" packets will go to the real h3, h4, etc., not to h1 (the attacker). This demonstrates how spoofing hides the attacker's location during an ICMP flood.