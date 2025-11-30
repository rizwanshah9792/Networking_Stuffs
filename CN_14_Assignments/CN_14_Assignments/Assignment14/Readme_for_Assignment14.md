# **🐍 Python Leaf-Spine Lab Guide**

## **Step 1: Run the Default Topology**

By default, the script sets k=2. This creates a small network (2 Spines, 4 Leaves).

Run this command in your terminal:

sudo mn \--custom leaf\_spine\_topology.py \--topo ls \--controller=none

Verify the Structure:  
Inside the Mininet console (mininet\>), type:  
net

* **Result:** You should see s1, s2 connected to l1, l2, l3, l4.

## **Step 2: Scale Up (Test the "Radix")**

Now, let's test if your Python code is truly scalable. We will increase k to **4**. This should automatically generate a much larger network (4 Spines, 8 Leaves).

1. Exit the previous Mininet session (exit).  
2. Run with the new parameter:  
   sudo mn \--custom leaf\_spine\_topology.py \--topo ls,k=4 \--controller=none

**Verify the Scaling:**

1. Type nodes to see the list. You should now see switches up to s4 and l8.  
2. Type pingall to ensure all 8 hosts can talk to each other through the spines.

## **Step 3: Understanding the Code**

* **class LeafSpine(Topo):**: We inherit from Mininet's base class.  
* **self.addSwitch()**: Creates the node.  
* **self.addLink(spine, leaf)**: This nested loop is the most important part. By looping through every spine and every leaf, we create the **"Non-Blocking"** full mesh architecture that defines a Leaf-Spine network.