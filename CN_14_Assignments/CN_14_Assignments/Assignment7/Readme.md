# Assignment 7: UDP Client-Server with Packet Loss Testing

This guide demonstrates how to compile, run, and verify packet loss handling using Mininet and Wireshark for testing UDP communication reliability.

## Prerequisites

- Mininet installed
- GCC compiler
- Wireshark
- Root/sudo access

## Network Setup

### Step 1: Create the Mininet Topology

Create a simple network with 2 hosts (`h1` and `h2`) and a switch.  We'll introduce 20% packet loss to test reliability: 

```bash
sudo mn --topo single,2 --link tc,loss=20
```

> **Note:** `loss=20` introduces a 20% packet drop rate, ideal for testing timeout mechanisms.

### Step 2: Open Host Terminals

Inside the Mininet prompt (`mininet>`), open terminals for both hosts: 

```bash
xterm h1 h2
```

## Compilation

### On Host 1 (Server)

```bash
gcc server.c -o server -lm
```

### On Host 2 (Client)

```bash
gcc client.c -o client
```

> **Note:** The `-lm` flag links the math library, which is required for this implementation.

## Monitoring with Wireshark

### Step 4: Start Packet Capture

1. Launch Wireshark from the Mininet prompt or a new terminal: 

```bash
sudo wireshark &
```

2. Select the interface `h1-eth0` (Host 1's network interface) or `s1-eth1`
3. Double-click the interface to start capturing
4. Apply the filter: `udp` in the filter bar and press Enter

## Running the Programs

### Start the Server (Host 1)

In the `h1` terminal:

```bash
./server
```

The server will now wait for incoming connections. 

### Start the Client (Host 2)

1. First, obtain Host 1's IP address.  In the `h1` terminal:

```bash
ifconfig
```

(Typically `10.0.0.1`)

2. In the `h2` terminal, run:

```bash
./client 10.0.0.1
```

## Testing & Verification

### Normal Operation

**Input:**
```
add 10 20
```

**Expected Output:**
- **Client:** `Result: 30.00`
- **Wireshark:** Two UDP packets visible: 
  - Request:  `10.0.0.2 → 10.0.0.1`
  - Reply: `10.0.0.1 → 10.0.0.2`

### Packet Loss Scenario

Continue entering calculations. Due to the 20% packet loss rate configured in Step 1, some requests will fail. 

**Expected Behavior:**
- **Client:** Waits for 3 seconds (timeout period), then displays: 
  ```
  [!] Error: Request timed out.  Packet lost or server down.
  ```
- **Wireshark:** Request packet is sent (`10.0.0.2 → 10.0.0.1`), but no reply packet is received

## Troubleshooting

| Issue | Solution |
|-------|----------|
| `command not found:  mn` | Install Mininet: `sudo apt-get install mininet` |
| Compilation error with `-lm` | Ensure build-essential is installed: `sudo apt-get install build-essential` |
| Wireshark shows no packets | Verify you're capturing on the correct interface (`h1-eth0` or `s1-eth1`) |
| Client cannot connect | Verify server IP with `ifconfig` on `h1` |

## Cleanup

To exit Mininet:

```bash
mininet> exit
```

To clean up Mininet:

```bash
sudo mn -c
```

## Network Topology

```
    h1 (10.0.0.1)
      |
     s1 (switch with 20% loss)
      |
    h2 (10.0.0.2)
```

## Protocol Details

- **Transport Layer:** UDP
- **Port:** (Specified in code)
- **Timeout:** 3 seconds
- **Packet Loss Rate:** 20% (configurable)

---

**Author:** rizwanshah9792  
**Repository:** [Networking_Stuffs](https://github.com/rizwanshah9792/Networking_Stuffs)
