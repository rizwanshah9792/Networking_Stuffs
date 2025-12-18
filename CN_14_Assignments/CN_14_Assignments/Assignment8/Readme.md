# Assignment 8: Multi-Client Chat Server

## Overview
This assignment implements a multi-threaded chat server and client application using a star topology network.  The server handles multiple client connections simultaneously and logs all conversations. 

## Network Topology

The implementation uses a **Star Topology** with the following components: 

- **Switch (s1)**: Central switch connecting all hosts
- **Host 1 (h1)**: Chat Server (IP: 10.0.0.1)
- **Host 2 (h2)**: Client - Alice (IP: 10.0.0.2)
- **Host 3 (h3)**: Client - Bob (IP: 10.0.0.3)

## Setup Instructions

### 1. Create Mininet Topology

Launch Mininet with a single switch and three hosts:

```bash
sudo mn --topo single,3 --mac
```

**Parameters:**
- `--topo single,3`: Creates one switch connected to three hosts
- `--mac`: Assigns simple MAC addresses (h1: 00:00:00:00:00:01, h2: 00:00:00:00:00:02, h3: 00:00:00:00:00:03)
- Automatically assigns sequential IP addresses (10.0.0.1, 10.0.0.2, 10.0.0.3)

### 2. Open Terminal Windows

Open separate terminal windows for each host:

```bash
xterm h1 h2 h3
```

### 3. Compile the Code

Compile the server and client programs on their respective hosts.  **Important**: Include the `-pthread` flag since the code uses the POSIX threads library. 

**On h1 (Server terminal):**
```bash
gcc server.c -o server -pthread
```

**On h2 (Client terminal):**
```bash
gcc client.c -o client -pthread
```

**On h3 (Client terminal):**
```bash
gcc client.c -o client -pthread
```

## Running the Application

### 4. Start the Server

**On h1:**
```bash
./server
```

The server will start listening for incoming client connections.

### 5. Connect Clients

**On h2 (Alice):**
```bash
./client 10.0.0.1
```
When prompted, enter the name:  `Alice`

**On h3 (Bob):**
```bash
./client 10.0.0.1
```
When prompted, enter the name: `Bob`

### 6. Send Messages

Once both clients are connected, they can send messages to each other. 

**Example conversation:**

**From h2 (Alice):**
```
Hello Bob!
```

**From h3 (Bob):**
```
Hi Alice!
```

All messages will be broadcast to all connected clients and logged on the server.

## Verify Chat Logs

The server automatically logs all messages to a text file. To view the conversation history:

```bash
cat log.txt
```

## Features

- Multi-threaded server supporting concurrent client connections
- Real-time message broadcasting to all connected clients
- Conversation logging to file
- Client identification by username
- Thread-safe message handling

## Requirements

- Mininet network emulator
- GCC compiler with pthread support
- Linux/Unix environment

## Troubleshooting

- If compilation fails, ensure you include the `-pthread` flag
- If clients cannot connect, verify the server is running on h1
- Check that the IP address (10.0.0.1) matches the server host
- Ensure all terminals are opened within the Mininet environment
