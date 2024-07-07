# Bulletin Board System

## Overview

This project implements a simple bulletin board system where a client can send strings and files to a server, and retrieve stored messages.

## Components

- **Server**: `2023_3201_server_final.py` (Python)
- **Client**: `BulletinBoard-Client.cpp` (C++)

## Requirements

- Python 3.11.2
- GCC (g++ 10.2.1)

## Setup

1. **Server**:
   - Run the server on a Kali Linux machine.
   - Start the server:
     ```bash
     python3 2023_3201_server_final.py
     ```

2. **Client**:
   - Compile and run the client on a Debian Linux machine:
     ```bash
     g++ BulletinBoard-Client.cpp -o client
     ./client <ip> <port>
     ```

## Usage

The client can send the following commands to the server:
- `POST_STRING`: Send a string message.
- `POST_FILE`: Send a file (max 256 bytes).
- `GET`: Retrieve all stored messages.
- `EXIT`: Close the connection.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
