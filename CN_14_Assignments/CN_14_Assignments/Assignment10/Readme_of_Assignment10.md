## **Step 1: Start Mininet**

Open your terminal and start a simple topology:

sudo mn \--topo single,2

## **Step 2: Open Host Terminals**

In the Mininet console:

xterm h1 h2

* **h1** will be the Sender (10.0.0.1).  
* **h2** will be the Receiver (10.0.0.2).

## **Step 3: Setup the Receiver (h2)**

We need to see the packets arriving. Since these are raw/custom packets, standard tools might ignore them, but **Wireshark** or **TCPDump** will see them.

Option A: Using Wireshark (Preferred if GUI is available)  
In the h2 terminal:  
wireshark &

1. When it opens, double-click the interface h2-eth0.  
2. In the filter bar, type tcp || icmp and press Enter.

Option B: Using TCPDump (Text-based)  
In the h2 terminal:  
\# Listen for TCP or ICMP packets and print them in ASCII so we can see the Roll No  
tcpdump \-i h2-eth0 \-X \-v

## **Step 4: Run the Sender (h1)**

A. Setup Files  
In the h1 terminal, create the files checksum.h, tcp\_raw.c, and icmp\_raw.c using nano.  
**B. Compile**

gcc tcp\_raw.c \-o tcp\_gen  
gcc icmp\_raw.c \-o icmp\_gen

**C. Execute TCP Task**

./tcp\_gen

*Check h2:* You should see a TCP packet. Look at the bottom of the packet details (hex/ascii dump). You will see your text: **"RollNo: CS2023-01"**.

**D. Execute ICMP Task**

./icmp\_gen

*Check h2:* You should see an ICMP packet. Wireshark will identify it specifically as **"Timestamp Request (13)"**.

## **Verification Tips**

1. If Wireshark shows "Header Checksum: 0x0000 \[incorrect\]", don't panic. This often happens because Mininet hosts verify checksums strictly. However, if the packet *appears* in the list, you have successfully generated it using a Raw Socket.  
2. The "Roll Number" will be inside the TCP "Payload" section.