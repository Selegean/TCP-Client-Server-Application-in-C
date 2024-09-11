# TCP-Client-Server-Application-in-C

This repository contains a basic example of a client-server application written in C, demonstrating network communication using sockets.

## Overview

The project is split into two main components:

### Server

- **Function:** Listens for incoming TCP connections on port `22315`.
- **Features:**
  - Accepts client connections.
  - Uses `fork()` to handle each client in a separate process.
  - Responds to client commands:
    - **"06#":** Sends an HTML page.
    - **Other commands:** Sends a "Command not implemented" message.
- **Purpose:** Illustrates basic server operations and handling multiple client connections.

### Client

- **Function:** Connects to a server and sends an HTTP GET request.
- **Features:**
  - Connects to `www.traceroute6.net` on port `80`.
  - Sends a request for the homepage.
  - Receives and saves the response into a file named `pagina.html`.
- **Purpose:** Demonstrates how to send an HTTP request and handle the server’s response.

## How to Run

1. **Compile the Server and Client:**
   ```bash
   gcc -o server server.c
   gcc -o client client.c
2. **Start the server**
   ```bash
   ./server
3. **Start the client**
   ```bash
   ./client
