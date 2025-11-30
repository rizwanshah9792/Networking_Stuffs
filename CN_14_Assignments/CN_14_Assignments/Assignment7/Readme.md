How to Execute in Mininet

This guide explains how to compile, run, and verify the packet loss requirement using Mininet and Wireshark.

Step 1: Create the Mininet Topology

We need a simple network with 2 hosts (h1 and h2) and a switch. To test reliability, we will artificially introduce packet loss (10% loss) on the link.

Open your terminal and run:

sudo mn --topo single,2 --link tc,loss=20


Note: loss=20 means there is a 20% chance a packet will be dropped, perfect for testing our timeout code.

Step 2: Open Host Terminals

Inside the Mininet prompt mininet>, open terminals for both hosts:

xterm h1 h2


Step 3: Compile the Code

In the h1 terminal (Server), type:

gcc server.c -o server -lm


In the h2 terminal (Client), type:

gcc client.c -o client


(Note: -lm is required to link the math library)

Step 4: Start Wireshark

To see the packets, we need Wireshark. In the Mininet prompt (or a new system terminal), run:

sudo wireshark &


When Wireshark opens, look for the interface h1-eth0 (this is Host 1's network card) or s1-eth1.

Double-click it to start capturing.

In the filter bar at the top, type udp and press Enter. This filters out the noise.

Step 5: Run the Programs

In terminal h1 (Server):

./server


(It will wait for connections...)

In terminal h2 (Client):
First, find the IP of h1. In h1's terminal, type ifconfig (usually 10.0.0.1).
Then in h2:

./client 10.0.0.1


Step 6: Testing & Detection

Normal Case: Enter add, 10, 20.

Client: Shows result 30.00.

Wireshark: You will see two blue UDP packets. One from 10.0.0.2 -> 10.0.0.1 (Request) and one back (Reply).

Packet Loss Case: Keep entering calculations. Because we set loss=20 in Step 1, eventually a request will fail.

Client: The program will freeze for 3 seconds (our timeout) and then print: [!] Error: Request timed out. Packet lost or server down.

Wireshark: You will see the Request packet go out, but no Reply packet follows immediately.
