sudo mn --topo single,2
*(This creates one switch and two hosts: h1 and h2)*

#### Step 2: Open Terminals for Hosts
Inside the Mininet console `mininet>`, type:
```bash
xterm h1 h2
*(Two black terminal windows will pop up. h1 will be the Server, h2 will be the Client)*

#### Step 3: Prepare the Server (In h1 Terminal)
1.  Create a folder for the server:
    ```bash
    mkdir server_folder
    cd server_folder
    2.  Create the C file (copy-paste the Server code above):
    ```bash
    nano server.c
    # (Paste code, Save: Ctrl+O, Enter, Exit: Ctrl+X)
    3.  Compile it:
    ```bash
    gcc server.c -o server
    4.  **Crucial:** Create the dummy file that needs to be downloaded by the client:
    ```bash
    # Creates a dummy file with some text
    echo "This is a file residing on the server." > server_data.txt
    5.  Run the server:
    ```bash
    ./server
    
#### Step 4: Prepare the Client (In h2 Terminal)
1.  Create a folder for the client:
    ```bash
    mkdir client_folder
    cd client_folder
    2.  Create the C file (copy-paste the Client code above):
    ```bash
    nano client.c
    # (Paste code, Save and Exit)
    3.  Compile it:
    ```bash
    gcc client.c -o client
    4.  **Crucial:** Create the dummy file that the client will upload to the server:
    ```bash
    echo "This is the client's secret file." > client_data.txt
    5.  Run the client (h1's IP is usually 10.0.0.1 in Mininet):
    ```bash
    ./client 10.0.0.1
    
### 4. Expected Output

**In the Client (h2) window:**
You should see "Connected," then "Download Complete," then "Upload Complete," along with the time taken (e.g., `0.000240 seconds`).

**In the Server (h1) window:**
You should see "Client connected," then "File Sent," then "File Received."

**Verification:**
If you list the files in the folders (`ls -l`), you will see:
* `server_folder`: contains `uploaded_by_client.txt` (New!)
* `client_folder`: contains `downloaded_from_server.txt` (New!)
