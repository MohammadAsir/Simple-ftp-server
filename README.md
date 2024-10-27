
# FTP Server Project

This project is a simple FTP server implemented in C, running on a Linux environment. It allows clients to transfer files to and from the server via a TCP socket connection. The server can handle basic FTP commands, including file upload (`put`), file download (`get`), and directory listing (`ls`).

## Features

- **Concurrent Connections**: Allows multiple clients to connect and transfer files concurrently.
- **Directory Navigation**: Clients can navigate the server and client file systems with `cd` and `lcd`.
- **File Transfer**: Clients can upload and download files from the server using `put` and `get`.
- **Persistent Connection**: The server remains active, listening on a designated port until terminated.
- **Simple Commands**: The server supports basic commands for file and directory management.

## Requirements

- A Linux system with GCC for compilation
- Networking libraries: `<unistd.h>`, `<sys/types.h>`, `<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`

## How It Works

1. **Server Startup**: Run the server program, and it will start listening for client connections on a specified port.
2. **Client Connection**: The client connects to the server using its IP and port.
3. **File Transfer Protocol (FTP) Commands**:
   - `ls`: Lists files in the server’s current directory.
   - `cd <directory>`: Changes the server’s current directory.
   - `lcd <directory>`: Changes the client's current directory.
   - `get <filename>`: Downloads a file from the server.
   - `put <filename>`: Uploads a file to the server.
4. **Transfer Execution**: After issuing `put` or `get`, the client connects to the server's data port to transfer the specified file.
5. **Termination**: Use `bye` to end the client connection and shut down the server.

## Usage

1. **Compiling**:
   ```bash
   gcc -o server ftp_server.c
   gcc -o client ftp_client.c
   ```

2. **Starting the Server**:
   ```bash
   ./server
   ```
   The server will start listening on the predefined port (5237).

3. **Connecting the Client**:
   ```bash
   ./client <server_ip_address>
   ```
   Replace `<server_ip_address>` with the IP of the machine running the server.

4. **Using FTP Commands**:
   - To list files: `ls`
   - To change directories on the server: `cd <directory_name>`
   - To change directories on the client: `lcd <directory_name>`
   - To upload a file: `put <filename>`
   - To download a file: `get <filename>`
   - To exit: `bye`

### Example Session

1. Start the server:
   ```bash
   ./server
   ```
   Server outputs:
   ```
   Server listening on port 5237
   ```

2. Connect the client:
   ```bash
   ./client 192.168.1.2
   ```
   Client outputs:
   ```
   ftp>
   ```

3. Use FTP commands from the client prompt.

## Notes

- This server requires Linux system calls, which may not be compatible with other operating systems.
- Ensure the server runs with proper permissions if accessing restricted directories or files.
  
