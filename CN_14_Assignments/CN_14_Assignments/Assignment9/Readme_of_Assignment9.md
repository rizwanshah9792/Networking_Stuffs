# **🖧 Mininet TCP File Transfer Lab Guide**

This guide details how to execute the TCP file transfer assignment using Mininet. You will set up a network, create a server environment, create a client environment, and measure transfer times.

## **🟢 Phase 1: Start the Network**

**Perform this in your main system terminal.**

1. **Launch Mininet** with a single switch and two hosts:  
   sudo mn \--topo single,2

2. Open Terminals for the virtual hosts:  
   Inside the mininet\> console, type:  
   xterm h1 h2

   *(Two black terminal windows will pop up. **h1** will be your Server, **h2** will be your Client).*

## **🔵 Phase 2: Configure the Server (Host h1)**

**Perform these steps inside the h1 terminal window.**

### **1\. Setup Environment**

We need a clean directory to store the server's files.

mkdir server\_folder  
cd server\_folder

### **2\. Create the Code**

Create the C file.

nano server.c

* **Action:** Paste the **Server Code** provided previously.  
* **Save:** Press Ctrl+O, then Enter.  
* **Exit:** Press Ctrl+X.

### **3\. Compile**

Convert the code into an executable program.

gcc server.c \-o server

### **4\. Create Dummy Data**

Create the file that the client will download.

echo "This is a file residing on the server." \> server\_data.txt

### **5\. Run Server**

Start listening for connections.

./server

*(The terminal will now hang, waiting for the client to connect...)*

## **🟣 Phase 3: Configure the Client (Host h2)**

**Perform these steps inside the h2 terminal window.**

### **1\. Setup Environment**

Create a clean directory for the client.

mkdir client\_folder  
cd client\_folder

### **2\. Create the Code**

Create the C file.

nano client.c

* **Action:** Paste the **Client Code** provided previously.  
* **Save:** Press Ctrl+O, then Enter.  
* **Exit:** Press Ctrl+X.

### **3\. Compile**

Convert the code into an executable program.

gcc client.c \-o client

### **4\. Create Dummy Data**

Create the file that the client will upload to the server.

echo "This is the client's secret file." \> client\_data.txt

### **5\. Run Client**

Connect to the server (Host 1 is always 10.0.0.1 in this topology).

./client 10.0.0.1

## **✅ Phase 4: Verification & Results**

Once the client runs, look at your terminals.

### **🖥️ Client Terminal (h2) Output**

You should see the transfer times calculated:

\[+\] Connected to Server.  
\[\*\] Downloading 'server\_data.txt' as 'downloaded\_from\_server.txt'...  
\[+\] Download Complete\! Time: 0.000150 sec  
\[\*\] Uploading 'client\_data.txt' to Server...  
\[+\] Upload Complete\! Time: 0.000120 sec

### **🖥️ Server Terminal (h1) Output**

You should see the status updates:

\[+\] Client connected.  
\[\*\] Starting transfer...  
\[+\] File Sent\! Time taken: 0.000140 seconds  
\[\*\] Waiting to receive...  
\[+\] File Received\! Time taken: 0.000130 seconds

### **📂 File Verification**

To prove the files actually moved, run ls \-l in both terminals.

* **In h1 (Server):** You should now see uploaded\_by\_client.txt.  
* **In h2 (Client):** You should now see downloaded\_from\_server.txt.