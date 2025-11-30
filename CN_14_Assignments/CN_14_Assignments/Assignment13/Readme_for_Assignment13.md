# **Packet Capture & Analysis Lab**

## **Prerequisite**

Install the C pcap library so we can compile the code later.

sudo apt-get update  
sudo apt-get install libpcap-dev

## **Step 1: Create the Topology**

We need a binary tree with 7 switches.

* **Depth 3** with a **Fanout of 2** creates exactly 7 switches ($2^0 \+ 2^1 \+ 2^2 \= 1 \+ 2 \+ 4 \= 7$).

Run this command to start Mininet:

sudo mn \--topo tree,depth=3,fanout=2

## **Step 2: Capture Packets**

We need to capture traffic on the Root Switch (s1).

1. Open Terminals:  
   Inside the Mininet console mininet\>, open a terminal for the root switch (s1) and a host (h1):  
   xterm s1 h1

2. Start Capturing (Inside s1 Node):  
   In the s1 terminal (black window), run tcpdump to listen and save to a file:  
   tcpdump \-i s1-eth1 \-w traffic.pcap

   *(Note: We use s1-eth1 to capture traffic moving from the root down to the branches).*  
3. Generate Traffic (Inside h1 Node):  
   In the h1 terminal, ping another host (e.g., h8 \- the last leaf node) to force traffic through the root.  
   ping 10.0.0.8 \-c 4

4. Stop Capturing:  
   Go back to the s1 terminal and press Ctrl+C to stop tcpdump.  
   You should now have a file named traffic.pcap in the s1 directory (which is shared with your main OS folder).  
5. Exit Mininet:  
   Type exit in the terminals and exit in the Mininet console.

## **Step 3: Analyze the File**

Now we use the C program to read the traffic.pcap file you just created.

1. Create the C file:  
   Copy the C code provided in the assignment into a file named packet\_analyzer.c.  
2. Compile the Program:  
   You must link the pcap library using \-lpcap.  
   gcc packet\_analyzer.c \-o analyzer \-lpcap

3. **Run the Analyzer:**  
   ./analyzer traffic.pcap

## **Expected Output**

You should see a "Time Diagram" followed by a summary.

\---------------- TIME DIAGRAM \----------------  
Timestamp        | Protocol Flow  
\----------------------------------------------  
\[170000.123456\] ETH \-\> ARP  \-\> Request/Reply  
\[170000.124000\] ETH \-\> ARP  \-\> Request/Reply  
\[170000.125000\] ETH \-\> IP  \-\> ICMP (Ping) \[Packet Size: 98\]  
\[170000.126000\] ETH \-\> IP  \-\> ICMP (Ping) \[Packet Size: 98\]  
...  
\----------------------------------------------

SUMMARY OF DETECTED PROTOCOLS:  
Layer 2 (Data Link): Ethernet  
Layer 3 (Network)  : IPv4 ARP   
Layer 4 (Transport): ICMP 

**Explanation of Results:**

* **Layer 2:** Every packet has an Ethernet header.  
* **Layer 3:** You will see **ARP** (finding MAC addresses) and **IPv4** (carrying the data).  
* **Layer 4:** Since you used ping, you will see **ICMP**.