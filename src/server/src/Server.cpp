#include "../include/Server.hxx"

#include <conio.h>

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>

#include "../include/Crypto.hxx"
#include "../include/Database.hxx"
#include "../include/Exception.hxx"
#include "../include/Interpreter.hxx"
#include "../include/Mail.hxx"
#include "../include/PMan.hxx"
#include "../include/Packager.hxx"
#include "../include/RequestFactory.hxx"
#include "../include/Resolver.hxx"
#include "../include/ResponseFactory.hxx"

Server*           Server::m_inst = nullptr;
std::atomic<bool> Server::m_running = true;
std::string       Server::m_hash;
ClientID          Server::m_id_pool;
int               Server::m_connected = 0;

void Server::spawn() {
    if (Server::m_inst == nullptr) {
        Server::m_inst = new Server;
        Database::init("PassLock.db");
        Interpreter::init();
        Packager::init();
        Resolver::init();
        PMan::init();
        Crypto::init();
        Mail::init();
        Logger::init();
        Crypto::get().load_module();
        std::thread serverThread(&Server::init, Server::m_inst);
        serverThread.detach();
    }
}
Server& Server::get() {
    if (Server::m_inst == nullptr) {
        throw Exception("Server not spawned", 0x01);
    }
    return *(Server::m_inst);
}
void Server::kill() {
    if (Server::m_inst != nullptr) {
        delete Server::m_inst;
        Server::m_inst = nullptr;
        Database::kill();
        Interpreter::kill();
        Packager::kill();
        Resolver::kill();
        Crypto::kill();
        Mail::kill();
        PMan::kill();
    }
    Logger::get().write("Server has successfully shut down.");
    Logger::kill();
}
void Server::init() {
    WSADATA     wsaData;
    SOCKET      clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int         clientAddrSize = sizeof(clientAddr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    // Create a socket
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    // Bind the socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket,
             (struct sockaddr*)&serverAddr,
             sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(m_serverSocket);
        WSACleanup();
        return;
    }

    // Listen for incoming connections
    if (listen(m_serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(m_serverSocket);
        WSACleanup();
        return;
    }

    Logger::get().write(
        "Server is successfully listening for connections.");
    Server::print("Server is listening on port " +
                  std::to_string(SERVER_PORT));

    // Accept incoming connections
    fd_set         readfds;
    struct timeval timeout;

    while (m_running) {
        FD_ZERO(&readfds);
        FD_SET(m_serverSocket, &readfds);
        timeout.tv_sec = 1;  // 1 second timeout to check stop flag
        timeout.tv_usec = 0;

        int activity = select(0, &readfds, NULL, NULL, &timeout);

        if (activity < 0 && m_running) {
            std::cerr << "Select error\n";
            break;
        }

        if (activity > 0 && FD_ISSET(m_serverSocket, &readfds)) {
            clientSocket = accept(m_serverSocket,
                                  (struct sockaddr*)&clientAddr,
                                  &clientAddrSize);
            if (clientSocket == INVALID_SOCKET) {
                if (!m_running) break;  // Stop loop if shutting down
                std::cerr << "Accept failed\n";
                continue;
            }

            ClientID::t_ID id = m_id_pool.acquire();
            if (m_running == true) {
                m_connected++;
                Server::print("Client connected | ID " +
                              std::to_string(id));
                Logger::get().write(
                    "A client has connected to the server.");
            }
            std::thread clientThread(
                Server::handle_client, clientSocket, id);
            clientThread.detach();
        }
    }
    std::cout << "<<PassLock Server>>$ Server shutting down...";
    closesocket(m_serverSocket);
    WSACleanup();
}
void Server::handle_client(SOCKET clientSocket, ClientID::t_ID id) {
    char buffer[1024];
    int  recvResult;

    // Receive and send data to the client
    while ((recvResult = recv(clientSocket, buffer, sizeof(buffer), 0)) >
           0) {
        buffer[recvResult] = '\0';  // Null-terminate the received data
        Logger::get().write(
            "Server has received a new payload from a client.");
        IRequest* request =
            Interpreter::get().interpret_and_fetch(buffer, recvResult);
        IResponse* response = request->solve();
        delete request;
        if (response != nullptr) {
            std::vector<std::vector<char>> frame = response->encapsulate();
            delete response;

            for (int i = 0; i < frame.size(); i++) {
                if (i >= 1) {
                    char frame_size = frame[i].size();
                    send(clientSocket,
                         reinterpret_cast<const char*>(&frame_size),
                         1,
                         0);
                }
                send(clientSocket, frame[i].data(), frame[i].size(), 0);
            }
            Logger::get().write(
                "Server has successfully sent a response to the client.");
        }
    }

    if (recvResult == 0 && m_connected > 0) {
        Server::print("Client disconnected | ID " + std::to_string(id));
        m_id_pool.release(id);
        m_connected--;
        Logger::get().write("A Client has been disconnected.");
    }

    // Clean up client socket
    closesocket(clientSocket);
}

void Server::stop() {
    m_running = false;  // Stop the server loop

    // Create a temporary connection to unblock `accept()`
    SOCKET tmpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tmpSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create temp socket for shutdown\n";
        return;
    }

    sockaddr_in tempAddr;
    tempAddr.sin_family = AF_INET;
    tempAddr.sin_port = htons(SERVER_PORT);
    tempAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(tmpSocket,
            (struct sockaddr*)&tempAddr,
            sizeof(tempAddr));  // Unblock accept()
    closesocket(tmpSocket);
}

bool Server::is_set() { return std::filesystem::exists(ADMIN_FILE); }

void Server::setup() {
    std::cout << "<<PassLock Server>>$ Set up a password: ";
    char        c;
    std::string aux;
    std::string pass;
    while (true) {
        if (_kbhit()) {
            c = _getch();
            if (c == '\r') break;
            if (c == '\b' && aux.empty() == false) {
                aux.pop_back();
                pass.pop_back();
                system("cls");
                std::cout << "<<PassLock Server>>$ Set up a password: "
                          << aux;
            } else {
                aux.push_back('*');
                pass.push_back(c);
                std::cout << "*";
            }
        }
    }
    std::cout << "\n";

    Crypto::init();

    std::string hash = Crypto::get().sha3_512(pass);
    Server::m_hash = hash;
    std::ofstream out(ADMIN_FILE, std::ios::binary);
    size_t        size = hash.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(hash.c_str(), size);
    out.close();

    Server::setup_salt();

    Crypto::kill();
}
bool Server::login() {
    std::string password, aux;
    std::cout << "<<PassLock Server>>$ Enter password: ";
    char c;
    while (true) {
        if (_kbhit()) {
            c = _getch();
            if (c == '\r') break;
            if (c == '\b' && aux.empty() == false) {
                aux.pop_back();
                password.pop_back();
                system("cls");
                std::cout << "<<PassLock Server>>$ Enter password: "
                          << aux;
            } else {
                aux.push_back('*');
                password.push_back(c);
                std::cout << "*";
            }
        }
    }
    std::cout << "\n";

    Crypto::init();
    std::string hash = Crypto::get().sha3_512(password);
    Server::m_hash = hash;
    Crypto::kill();

    std::ifstream in(ADMIN_FILE, std::ios::binary);
    size_t        size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::string buffer(size, '\0');
    in.read(&buffer[0], size);

    return buffer == hash;
}

void Server::setup_salt() {
    std::vector<char> salt = Crypto::get().generate_random_array();

    std::ofstream out(SALT_FILE, std::ios::binary);
    size_t        size = salt.size();

    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(reinterpret_cast<const char*>(salt.data()), size);

    out.close();
}

std::vector<char> Server::get_salt() {
    std::ifstream in(SALT_FILE, std::ios::binary);

    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::vector<char> salt(size);

    in.read(salt.data(), size);
    in.close();

    return salt;
}

std::string Server::get_hash() const { return m_hash; }

void Server::print(const std::string& text) {
    std::cout << "\n<<PassLock Server>>$ " << text << "\n";
    std::cout << "<<PassLock Server>>$ ";
}
void Server::print_no_new_line(const std::string& text) {
    std::cout << "<<PassLock Server>>$ " << text << "\n";
    std::cout << "<<PassLock Server>>$ ";
}

int Server::get_connected() { return m_connected; }

void Server::print_help() {
    std::string info = R"(
         --- PassLock Server Commands ---

    stats                               prints number of connected users
    stop                                stops the server
    log-show [n]                        shows the last n log entries
    log-showall                         shows all log entries
    log-count                           shows the total number of log entries
    )";
    Server::print(info);
}