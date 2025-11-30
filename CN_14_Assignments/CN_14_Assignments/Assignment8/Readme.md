For this assignment, we need a "Star Topology." We will have one central Switch (s1), and three hosts connected to it:

h1: Will act as the Server.

h2: Will be Alice (Client 1).

h3: Will be Bob (Client 2).

$sudo mn --topo single,3 --mac

--topo single,3: Creates 1 switch connected to 3 hosts.

--mac: Sets simple MAC addresses (h1 gets 00:00:00:00:00:01, etc.) and usually sets IPs sequentially (h1=10.0.0.1, h2=10.0.0.2).


xterm h1 h2 h3


Compile the Code
You must compile the code inside these new windows. Crucial Note: Because we used the <pthread.h> library, you must add the -pthread flag, or the code will fail to compile.

In the h1 (Server) terminal:

$gcc server.c -o server -pthread

In the h2 (Client) terminal:

$gcc client.c -o client -pthread

In the h3 (Client) terminal: You can re-type the command, or just use the same executable if they share a folder. To be safe, just compile it here too:

$gcc client.c -o client -pthread

h1: ./server
h2: ./client 10.0.0.1 // Enter name: Alice
h3: ./client 10.0.0.1 // Enter name: Bob

h2: "Hello Bob!
h3: "Hi Alice!"


Check the generated text file:
$cat log.txt
