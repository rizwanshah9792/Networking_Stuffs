# Computer Networks Lab – Assignment 9

## TCP File Upload and Download Using Mininet

This README consolidates and cleans the provided README files for **Assignment 9**. Duplicate explanations and repeated steps have been removed, and the execution flow is simplified and consistent.

---

## Objective

To implement a TCP-based client–server application that supports **file download and file upload** between two Mininet hosts and to measure the **transfer time** for both operations.

---

## Network Setup

* Topology: **Single switch with two hosts**
* h1 → Server (IP: `10.0.0.1`)
* h2 → Client (IP: `10.0.0.2`)

Start Mininet:

```bash
sudo mn --topo single,2
```

Open terminals for both hosts:

```bash
xterm h1 h2
```

---

## Execution Steps

### Step 1: Server Setup (h1)

```bash
mkdir server_folder
cd server_folder
```

Create server program:

```bash
nano server.c
```

*(Paste the server code and save)*

Compile:

```bash
gcc server.c -o server
```

Create the file to be downloaded by client:

```bash
echo "This is a file residing on the server." > server_data.txt
```

Run the server:

```bash
./server
```

---

### Step 2: Client Setup (h2)

```bash
mkdir client_folder
cd client_folder
```

Create client program:

```bash
nano client.c
```

*(Paste the client code and save)*

Compile:

```bash
gcc client.c -o client
```

Create the file to be uploaded to server:

```bash
echo "This is the client's secret file." > client_data.txt
```

Run the client (connect to server):

```bash
./client 10.0.0.1
```

---

## Verification

### Client Side (h2)

* Connection to server is established
* Server file is downloaded successfully
* Client file is uploaded successfully
* Download and upload times are displayed

### Server Side (h1)

* Client connection is accepted
* File is sent to client
* File is received from client

Verify files:

```bash
ls -l
```

* `server_folder` contains `uploaded_by_client.txt`
* `client_folder` contains `downloaded_from_server.txt`

---

## Output

```
<Leave blank – add screenshots / terminal output here>
```

---

## Learning Outcomes

* Understanding TCP client–server communication
* File transfer using sockets
* Measuring transmission time
* Practical use of Mininet for network experiments

---
