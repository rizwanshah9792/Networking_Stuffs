# 🐍 Python Leaf-Spine Lab Guide

## Overview

This lab demonstrates the implementation of a scalable **Leaf-Spine network topology** using Python and Mininet. The Leaf-Spine architecture is a common data center network design that provides non-blocking, high-bandwidth connectivity between hosts.

---

## 📋 Prerequisites

- **Mininet** installed on your system
- **Python 3.x**
- **Root/sudo** access
- Basic knowledge of network topologies

---

## 🚀 Getting Started

### Step 1: Run the Default Topology

By default, the script uses `k=2`, which creates a small network with: 
- **2 Spine switches** (s1, s2)
- **4 Leaf switches** (l1, l2, l3, l4)
- **4 Hosts** (h1, h2, h3, h4)

**Run the command:**

```bash
sudo mn --custom leaf_spine_topology.py --topo ls --controller=none
```

**Verify the Network Structure:**

Inside the Mininet console (`mininet>`), type:

```bash
mininet> net
```

**Expected Result:**  
You should see spine switches (s1, s2) connected to all leaf switches (l1, l2, l3, l4), with each leaf connected to a host.

**Test Connectivity:**

```bash
mininet> pingall
```

All hosts should be able to ping each other successfully.  

**Exit Mininet:**

```bash
mininet> exit
```

---

### Step 2: Scale Up the Topology (Testing Scalability)

Now let's test the scalability by increasing `k` to **4**, which will generate: 
- **4 Spine switches** (s1, s2, s3, s4)
- **8 Leaf switches** (l1, l2, l3, l4, l5, l6, l7, l8)
- **8 Hosts** (h1-h8)

**Run with the new parameter:**

```bash
sudo mn --custom leaf_spine_topology. py --topo ls,k=4 --controller=none
```

**Verify the Scaling:**

1. **List all nodes:**
   ```bash
   mininet> nodes
   ```
   You should see switches up to **s4** (spines) and **l8** (leaves).

2. **Check the topology:**
   ```bash
   mininet> net
   ```

3. **Test full connectivity:**
   ```bash
   mininet> pingall
   ```
   All 8 hosts should be able to communicate through the spine-leaf fabric.

**Exit Mininet:**

```bash
mininet> exit
```

---

## 🔍 Understanding the Code

### Key Components

#### **1. Class Definition**
```python
class LeafSpine(Topo):
```
- Inherits from Mininet's base `Topo` class
- Enables custom topology creation

#### **2. Switch Creation**
```python
self.addSwitch()
```
- Creates spine and leaf switches dynamically based on the `k` parameter

#### **3. Full Mesh Connectivity**
```python
self.addLink(spine, leaf)
```
- The nested loop connects **every spine to every leaf**
- This creates a **non-blocking, full mesh** architecture
- Ensures maximum bandwidth and redundancy

#### **4. Host Assignment**
```python
self.addHost()
self.addLink(leaf, host)
```
- Each leaf switch connects to one or more hosts
- Provides edge connectivity for endpoints

---

## 📊 Topology Architecture

### Leaf-Spine Benefits

✅ **Non-blocking**: Every leaf has equal access to all spines  
✅ **Scalable**: Easy to add more spines or leaves  
✅ **Predictable latency**: Maximum of 2 hops between any two hosts  
✅ **High bandwidth**: Multiple equal-cost paths between hosts  
✅ **Fault tolerant**:  Redundant paths for high availability

### Network Diagram

```
        [Spine-1] ---- [Spine-2] ---- [Spine-k]
         /  |  \        /  |  \        /  |  \
        /   |   \      /   |   \      /   |   \
    [Leaf1][Leaf2].. .[Leafk*2]
      |      |           |
    [Host] [Host]     [Host]
```

---

## 🧪 Testing Commands

| Command | Description |
|---------|-------------|
| `net` | Display network topology |
| `nodes` | List all nodes (switches and hosts) |
| `links` | Show all links between nodes |
| `pingall` | Test connectivity between all hosts |
| `iperf h1 h2` | Test bandwidth between two hosts |
| `dump` | Show detailed node information |
| `exit` | Exit Mininet |

---

## 📝 Assignment Tasks

1. ✅ Run the default topology (k=2)
2. ✅ Scale up to k=4 and verify connectivity
3. 📊 Analyze the network performance using `pingall`
4. 🔧 Modify the code to add additional parameters (optional)
5. 📈 Test with k=6 or k=8 to understand scaling limits

---

## 🛠️ Troubleshooting

### Common Issues

**Problem:** "Command not found:  mn"  
**Solution:** Install Mininet:  `sudo apt-get install mininet`

**Problem:** "Permission denied"  
**Solution:** Run with sudo: `sudo mn --custom ...`

**Problem:** "Custom topology file not found"  
**Solution:** Ensure `leaf_spine_topology.py` is in the current directory or provide full path

**Problem:** "Pingall fails"  
**Solution:** Run `sudo mn -c` to clean up previous Mininet sessions, then try again

---

## 📚 Additional Resources

- [Mininet Documentation](http://mininet.org/walkthrough/)
- [Leaf-Spine Architecture Overview](https://en.wikipedia.org/wiki/Leaf_spine)
- [Python Custom Topologies](https://github.com/mininet/mininet/wiki/Introduction-to-Mininet)

---

## 👨‍💻 Author

**Rizwan Shah**  
Repository: [Networking_Stuffs](https://github.com/rizwanshah9792/Networking_Stuffs)

---

## 📄 License

This project is part of CN_14_Assignments coursework. 

---

**Happy Networking!  🌐**
