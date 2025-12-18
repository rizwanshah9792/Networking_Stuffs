# Computer Networks Lab – Assignment 1

## TCP Client–Server Communication (Basic Socket Programming)

---

## Objective

To gain first hands-on experience with **basic TCP socket programming** by implementing a simple client–server interaction.

---

## Problem Statement

Write a program to run **TCP client and server socket programs** where:

* The **client** sends the message **"Hi"** to the server.
* The **server** responds with the message **"Hello"**.

---

## Network Setup

* Topology: Single switch with two hosts
* **h1** → Server
* **h2** → Client

Start Mininet:

```bash
sudo mn --topo single,2
```

Open terminals for both hosts:

```bash
xterm h1 h2
```

---

## Simplified Execution Steps

### Step 1: Server Setup (h1)

Create the server program:

```bash
vim tcp_server.c
```

Compile the server:

```bash
gcc tcp_server.c -o server
```

Run the server:

```bash
./server
```

---

### Step 2: Client Setup (h2)

Create the client program:

```bash
vim tcp_client.c
```

Compile the client:

```bash
gcc tcp_client.c -o client
```

Run the client (connect to server):

```bash
./client 10.0.0.1
```

---

## Verification

* Client sends the message **"Hi"** to the server
* Server receives the message and responds with **"Hello"**
* Both messages are displayed correctly in their respective terminals

---

## Output

```
<Leave blank – add terminal output screenshot here>
```

---

## Learning Outcomes

* Understanding TCP client–server architecture
* Basics of socket creation, binding, listening, and accepting connections
* Sending and receiving data using TCP socke
