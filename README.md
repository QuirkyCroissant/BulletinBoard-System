# Bulletin Board System

## Preamble

This project is a small project written as part of a Computer Networks Course(CS3201) while being on exchange at the City University of Hong Kong in Winter 2023/24. It demonstrates a simple Client-Server application that enables two devices to communicate with each other by sending and receiving messages/files. For deployment, we used two virtual machines running on Linux, connected via a mobile hotspot.

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
   - Run the server on a Linux machine.
   - Start the server:
     ```bash
     python3 2023_3201_server_final.py
     ```

2. **Client**:
   - Compile and run the client on a different Linux machine:
   - Both must be connected to the same local network to be able to communicate
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
