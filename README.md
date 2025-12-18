# Computer Networks Lab

This repository contains implementations for **Computer Networks Laboratory assignments**.
Each section below lists **only the problem statement (question)** as given in the lab manual.
**Outputs, screenshots.**

---

## Assignment 1: TCP Client–Server (Basic)

### Objective

To gain first hands-on experience of basic socket programming.

### Question

Write a program to run TCP client and server socket programs where the client first says **"Hi"** and in response the server says **"Hello"**.

### Output

<img width="1041" height="937" alt="output_as1" src="https://github.com/user-attachments/assets/ca2ef0d9-9609-4c1b-b198-c1f2bc162567" />


---
## Assignment 2: TCP Socket Programming (Multi-client Application)

### Objective

To gain experience of TCP socket programming for simple applications.

### Question

Write a program using TCP sockets to implement the following:

* Server maintains records of fruits in the format: fruit-name, quantity, last-sold (server timestamp)
* Multiple clients purchase fruits one at a time
* Fruit quantity is updated after every sale
* Server sends a regret message if requested quantity is not available
* Server displays customer IDs ⟨IP, Port⟩ who have completed transactions
* Server displays the total number of unique customers after each transaction

### Output

<img width="1041" height="993" alt="output_as2" src="https://github.com/user-attachments/assets/c54e4ae6-073f-4126-9541-4de618736a25" />


---
## Assignment 3: UDP Socket Programming

### Objective

To gain experience of UDP socket programming for simple applications.

### Question

Redo **Assignment 2** using **UDP sockets** instead of TCP sockets.

### Output

<img width="1041" height="993" alt="output_ass3" src="https://github.com/user-attachments/assets/ff8469e4-05f4-4739-951f-a207c9ef6dc0" />



---
## Assignment 4: Packet Capture and Protocol Analysis

### Objective

To gain knowledge of packet capturing tools and understand header structures.

### Question

Install Wireshark in a virtual machine environment. Capture packets during a **PING** operation and:

* Draw a time diagram showing protocol steps
* List L2, L3, and L4 header fields extracted from the captured `.pcap` file

### Output


<img width="1041" height="993" alt="output_ass4" src="https://github.com/user-attachments/assets/3c395894-53b2-491a-90bc-ca41edec6f11" />


---
## Assignment 5: Packet Generation Tools

### Objective

To gain knowledge of packet capturing and generation tools.

### Question

Learn and use the maximum number of packet generation tools such as **iperf**, **D-ITG**, etc., and generate traffic between Mininet hosts.

### Output


**Decode(D-itg_file)**
<img width="996" height="999" alt="Decode(D-itg_file)" src="https://github.com/user-attachments/assets/4a00f5c6-3dfa-44e2-bde4-deb7f18434d7" />

**D-ITG(gen_netTraffic_test_netPerfor)**
<img width="828" height="469" alt="D-ITG(gen_netTraffic_test_netPerfor)" src="https://github.com/user-attachments/assets/002bb832-b9d5-4e2d-956f-2da5e5bca686" />

**hping3(flood_custom_net_packets)**
<img width="996" height="1019" alt="hping3(flood_custom_net_packets)" src="https://github.com/user-attachments/assets/84d65e30-f894-4ee7-a132-403a2a7b4df8" />

**output_ass5_iperf3(speed,thourghtput_test)**
<img width="996" height="1019" alt="output_ass5_iperf3(speed,thourghtput_test)" src="https://github.com/user-attachments/assets/ae839e99-782f-4645-9cfe-3312f86a5046" />


---
## Assignment 6: TCP/IP Libraries in C

### Objective

To gain knowledge of TCP/IP C libraries.

### Question

Develop a simple C-based network simulator to analyze TCP traffic by:

* Listening to incoming packets at the Ethernet port
* Extracting headers and data from incoming packets

### Output

<img width="1106" height="924" alt="output_ass6" src="https://github.com/user-attachments/assets/3515f98f-42d8-40d6-8b75-25d6ea0ab763" />


---
## Assignment 7: UDP-Based Scientific Calculator

### Objective

Client–server communication using UDP packets.

### Question

Write a UDP client–server program where:

* Client sends one or two numbers and a scientific operation (sin, cos, +, -, *, /, inverse, etc.)
* Server evaluates the expression and sends back the result
* Packet loss is detected using Wireshark in Mininet

### Output

<img width="1067" height="1000" alt="output_ass7" src="https://github.com/user-attachments/assets/3333c6cf-4b76-4da9-9d42-6695180b3500" />


---
## Assignment 8: Multi-threaded TCP Chat Server

### Objective

Using thread library and TCP sockets.

### Question

Write a C program using threads and TCP sockets to build a **group chat server** where:

* Multiple clients communicate through the server
* Each client is handled by a separate server thread
* Server maintains a message log with timestamps
* All clients receive messages in real time

### Output

<img width="1106" height="924" alt="output_ass8" src="https://github.com/user-attachments/assets/69bf40b7-0c4a-43a4-867c-737e34b879b1" />



---
## Assignment 9: File Upload and Download Using TCP

### Objective

File upload and download using TCP.

### Question

Write a TCP client–server program for:

* Downloading a file from server to client
* Uploading a file from client to server
* Measuring transfer time using `time.h` on both client and server sides

### Output

<img width="1106" height="924" alt="output_ass9" src="https://github.com/user-attachments/assets/f0d2599d-07a7-46ef-97ac-b0330561ab88" />



---
## Assignment 10: RAW Socket Packet Generation

### Objective

Using RAW sockets to generate packets.

### Question

Write two C programs using RAW sockets to:

* Send a TCP packet containing your roll number in the payload
* Send ICMP timestamp messages to a target IP

### Output

<img width="1911" height="1045" alt="output_ass10" src="https://github.com/user-attachments/assets/ea0eeaa7-4402-4b9f-9ff9-0455952e6810" />



---
## Assignment 11: TCP SYN Flood Attack Using RAW Sockets

### Objective

Using RAW sockets to generate TCP flooding attack.

### Question

Write a RAW socket program to generate a **TCP SYN flood DDoS attack** using:

* One attacker host
* One victim host
* Four spoofed agent hosts in Mininet

### Output

<img width="1089" height="940" alt="output_ass11" src="https://github.com/user-attachments/assets/61b7f51e-ec79-4d2d-88b3-d25d736be27d" />



---
## Assignment 12: ICMP Flooding Attack Using RAW Sockets

### Objective

Using RAW sockets to generate ICMP flooding attack.

### Question

Implement the same flooding attack as Assignment 11 using **ICMP packets** instead of TCP.

### Output

<img width="1089" height="940" alt="output_ass12" src="https://github.com/user-attachments/assets/830fd9f8-d513-4ac5-8abb-e7f3f514ad8b" />



---
## Assignment 13: Packet Analysis in Custom Topology

### Objective

To learn packet capturing and analysis.

### Question

Create a **binary tree topology** with 7 switches in Mininet and:

* Capture packets at the root switch during a PING operation
* Extract headers using a C program
* Draw a time diagram of protocol activity
* List L2, L3, and L4 protocols identified

### Output

**Analysing the Packets**
<img width="1089" height="1036" alt="analyzingThePackets" src="https://github.com/user-attachments/assets/2ee96418-e68e-477c-881e-80700f2506b4" />

**Capturing the Packets**
<img width="1089" height="940" alt="capturingPackets" src="https://github.com/user-attachments/assets/f91b1e6a-8d03-42a0-8762-e2e79dbbe0d5" />



---
## Assignment 14: Custom Mininet Topology

### Objective

Creating customized topologies in Mininet.

### Question

Create a **scalable leaf–spine topology** in Mininet using Python and Mininet APIs. A reference to fat-tree topology may be used.

### Output

**Default Topology**
<img width="1320" height="1036" alt="default_topology" src="https://github.com/user-attachments/assets/3f15da5a-f072-4013-b1e4-566b6ac0f003" />

**Scale-Up Radix**
<img width="1876" height="457" alt="ScaleUpRadix" src="https://github.com/user-attachments/assets/76350d45-68c1-4b32-a9bb-9f3b31308678" />



### Notes

* All programs are expected to run in a **Linux + Mininet** environment.
* Screenshots, `.pcap` files, and execution outputs are be added in the respective assignment folders.
