# UDP Fruit Store - Socket Programming

## Compilation

```bash
# Compile server
gcc udp_fruit_server.c -o server

# Compile client
gcc udp_fruit_client.c -o client
```

## Execution

### On Server Terminal:
```bash
./server
```

### On Client Terminal 1 (localhost):
```bash
./client
```

### On Client Terminal 2 (localhost):
```bash
./client
```

### For Mininet Hosts:

**On Mininet:**
```bash
# Start mininet with 3 hosts
sudo mn --topo single,3

# On host h1 (server)
mininet> h1 ./server &

# On host h2 (client 1)
mininet> h2 ./client 10.0.0.1

# On host h3 (client 2)
mininet> h3 ./client 10.0.0.1
```

## Features Implemented

✓ Server maintains fruit records (name, quantity, last-sold timestamp)
✓ Multiple clients can purchase fruits
✓ Quantity updates after each sale
✓ Regret message for insufficient quantity
✓ Display customer IDs (IP:Port) who did transactions
✓ Show total unique customers to each client
✓ Server runs continuously in a loop
✓ UDP connectionless communication

## Sample Usage

**Client Operations:**
1. View Stock - See all available fruits
2. Buy Fruit - Purchase specific quantity
3. Exit - Close connection

**Example:**
- Buy Apple 10
- Buy Banana 5
- View current stock with timestamps
