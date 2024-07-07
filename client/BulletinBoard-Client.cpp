#include <iostream>
#include <string>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

/*
*   Project Team for Socket Programming:
*   @author Florian Hajek(---)
*   @author ---
*/

const int SIZE_OF_BUFFER = 1024;
const std::string BOLD_LINE = std::string(25, '=');

/*
*   Methode responsible for sending a message to the corresponding server
*/
void sendToServer(int sockfd, const std::string& message) {
    send(sockfd, message.c_str(), message.size(), 0);
}

/*
*   Method calls the recv Syscall to receive a response from the Server, 
*   which gets saved in a buffer and than returned as a string
*/
std::string receiveAnswer(int sockfd) {
    char buffer[SIZE_OF_BUFFER];
    int bytes_received = recv(sockfd, buffer, SIZE_OF_BUFFER - 1, 0);
    buffer[bytes_received] = '\0';
    return std::string(buffer);
}

/*
*   Method which is responsible for enabling the user to send multiple 
*   Strings to the server and ends if user submits a single '&' character
*/
void managePostString(int sockfd) {
    std::string line;
    sendToServer(sockfd, "POST_STRING");

    while (std::getline(std::cin, line) && line != "&") {
        sendToServer(sockfd, line);
    }
    sendToServer(sockfd, line);

    std::string serverResponse = receiveAnswer(sockfd);
    std::cout << "Send status: " << (serverResponse == "server: OK" ? "server: OK" : "server: ERROR") << '\n';
    //std::cout << serverResponse << '\n';
}

/*
 *  Method enables the user to send a file(maximum of 256 Bytes) to the server.
 *  It checks if the file can be opened and if the size is valid for the server.
*/
void managePostFile(int sockfd) {
    
    sendToServer(sockfd, "POST_FILE");
    std::string serverResponse = receiveAnswer(sockfd);
    std::cout << serverResponse << '\n';

    // Fetches File and opens it with an ifstream
    std::string filePath;
    std::cout << "Enter the file path: ";
    std::getline(std::cin, filePath);
    
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cout << "Error: Unable to open file." << '\n';
        return;
    }

    // checks for valid file size(not exceeding 256Bytes)
    file.seekg(0, std::ios::end);
    int64_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    if (length > 256) {
        std::cout << "Error: File size exceeds 256 bytes." << '\n';
        return;
    }

    // extracts filename and declares right buffersize for file and sends them to server
    std::string filename = filePath.substr(filePath.find_last_of("/\\") + 1);
    char file_info[128 + sizeof(int64_t)];
    std::memset(file_info, 0, sizeof(file_info));
    std::memcpy(file_info, filename.c_str(), filename.length());
    std::memcpy(file_info + 128, &length, sizeof(int64_t));
    send(sockfd, file_info, sizeof(file_info), 0);

    char *buffer = new char[length];
    file.read(buffer, length);
    send(sockfd, buffer, length, 0);
    delete[] buffer;

    file.close();

    // check for Server response
    serverResponse = receiveAnswer(sockfd);
    std::cout << "Send status: " << serverResponse << '\n';
}

/*
*   Method that is responsible of calling the GET-operation of the server,
*   client lists out every user interaction within the active socket connection 
*/
void manageGet(int sockfd) {
    sendToServer(sockfd, "GET");

    std::string line;
    while ((line = receiveAnswer(sockfd)) != "server: &") {
        std::cout << line << '\n';
    }
}

/*
*   Method that calls the EXIT operation, which severes the Connection with the server
*/
void manageExit(int sockfd) {
    sendToServer(sockfd, "EXIT");
    std::string serverResponse = receiveAnswer(sockfd);
    std::cout << "Send status: " << (serverResponse == "server: OK" ? "server: OK" : "server: ERROR") << '\n';    
}



/*
*   main function fetches the input arguments of the user, creates a socket, and once
*   the connection was established, the user is asked to provide the client with commands
*   till the connection to the server is killed
*/
int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        std::cerr << "ERROR: This Form should be used: BulletinBoardClient <IP Address> <Port>" << '\n';
        return 1;
    }

    std::cout << BOLD_LINE << " Initialize Socket " << BOLD_LINE << '\n';

    std::string server_ip = argv[1];
    int port = std::stoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "ERROR: Socket couldn't get created!" << '\n';
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    std::cout << "Input IP Address: " << server_ip << '\n';
    std::cout << "Input Port Number: " << port << '\n';

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "ERROR: COuldn't connect to server!" << '\n';
        return 1;
    }

    std::cout << "Connect status: OK" << '\n';

    std::string operation;
    bool first = true;
    while (true) {
        if(first){
            std::cout << BOLD_LINE << " Input Command " << BOLD_LINE << '\n';
            std::cout << "Possible Commands: 'POST_STRING', 'POST_FILE', 'GET', 'EXIT'" << '\n';
            first = false;
        } else {
            std::cout << BOLD_LINE << " Input Next Command " << BOLD_LINE << '\n';
        }

        std::cout << "Input Command: ";
        std::getline(std::cin, operation);

        if (operation== "POST_STRING") {
            std::cout << BOLD_LINE << " Content (Type a lone '&' to end message) " << BOLD_LINE << '\n';
            managePostString(sockfd);
        } else if (operation== "POST_FILE") {
            managePostFile(sockfd);
        } else if (operation== "GET") {
            std::cout << std::string(3, '-') << " Receiving Messages " << std::string(3, '-') << '\n';
            manageGet(sockfd);
        } else if (operation== "EXIT") {
            manageExit(sockfd);
            break;
        } else {
            std::cout << "ERROR - Command not understood!" << '\n';
        }
    }

    close(sockfd);
    return 0;
}