#include "client.h"

SOCKET* startClient(){
    WSADATA wsaData;
    SOCKET* clientSocket = new SOCKET;
    sockaddr_in serverAddr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        delete clientSocket;
        return nullptr;
    }

    // Create a socket
    *clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        delete clientSocket;
        WSACleanup();
        return nullptr;
    }

    // Set up the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(*clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        stopClient(clientSocket);
        return nullptr;
    }

    std::cout << "Connected to server\n";

    return clientSocket;
}

void stopClient(SOCKET* socket){
    closesocket(*socket);
    delete socket;
    WSACleanup();
}
