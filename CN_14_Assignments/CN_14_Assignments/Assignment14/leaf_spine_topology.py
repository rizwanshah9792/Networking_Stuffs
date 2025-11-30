from mininet.topo import Topo

class LeafSpine(Topo):
    """
    Scalable Leaf-Spine Topology
    k = Scale Factor (Radix)
    Number of Spine Switches = k
    Number of Leaf Switches = k * 2
    """

    def build(self, k=2):
        # 1. Initialize lists to store switch names
        spines = []
        leaves = []

        # Calculate quantities based on k
        num_spines = k
        num_leaves = k * 2

        print(f"[*] Building Topology with k={k}")
        print(f"[*] Spines: {num_spines}, Leaves: {num_leaves}")

        # ------------------------------------------------
        # 2. Create Spine Switches (The Core Layer)
        # ------------------------------------------------
        for i in range(num_spines):
            name = f's{i+1}'  # Names: s1, s2, s3...
            spine_switch = self.addSwitch(name)
            spines.append(spine_switch)

        # ------------------------------------------------
        # 3. Create Leaf Switches (The Access Layer)
        # ------------------------------------------------
        for i in range(num_leaves):
            name = f'l{i+1}'  # Names: l1, l2, l3...
            leaf_switch = self.addSwitch(name)
            leaves.append(leaf_switch)

            # Add one Host per Leaf (h1 connects to l1)
            host_name = f'h{i+1}'
            host = self.addHost(host_name)
            self.addLink(host, leaf_switch)

        # ------------------------------------------------
        # 4. Create the Interconnections (Full Mesh)
        # ------------------------------------------------
        # In a Leaf-Spine, every Spine connects to every Leaf
        for spine in spines:
            for leaf in leaves:
                self.addLink(spine, leaf)

# This enables Mininet to read the file with the --custom flag
# Maps the name 'ls' to our LeafSpine class
topos = { 'ls': ( lambda k=2: LeafSpine(k=k) ) }
