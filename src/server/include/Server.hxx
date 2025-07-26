#ifndef SERVER_HXX
#define SERVER_HXX

#include <winsock2.h>

#include <atomic>
#include <iostream>
#include <string>
#include <thread>  // Include for threads
#include <vector>

#include "ClientID.hxx"
#include "Logger.hxx"

#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

#define SERVER_PORT 54000  // Port to listen on

#define ADMIN_FILE "../data/admin.sv"
#define SALT_FILE "../data/salt.sv"

class Server {
   private:
    static Server*           m_inst;
    static std::atomic<bool> m_running;

    static std::string m_hash;

    static ClientID m_id_pool;

    SOCKET     m_serverSocket;
    static int m_connected;

    Server() = default;
    ~Server() = default;

    /**
     * @brief Loads all necessary modules for the server to run.
     */
    void init();

    /**
     * @brief Handles a client's requests and sends responses.
     *
     * @param clientSocket Socket on which the client is connected.
     * @param id Client's ID.
     */
    static void handle_client(SOCKET clientSocket, ClientID::t_ID id);

    /**
     * @brief Set the server salt.
     */
    static void setup_salt();

   public:
    Server(const Server& other) = delete;
    Server(Server&& other) = delete;

    static void    spawn();
    static Server& get();
    static void    kill();

    /**
     * @brief Checks if a server-password has been set.
     *
     * @return True if a password has been set, false otherwise.
     */
    static bool is_set();

    /**
     * @brief Prompts the user to set up a password for the server.
     */
    static void setup();

    /**
     * @brief Prompts the user enter the set password.
     *
     * @return True if the password entered is the right one, false
     * otherwise.`
     */
    static bool login();

    /**
     * @brief Gets the server-password hash.
     *
     * @return A std::string representing the server password hash.
     */
    std::string get_hash() const;

    /**
     * @brief Gets the server salt.
     *
     * @return A std::vector<char> representing the server salt.
     */
    static std::vector<char> get_salt();

    /**
     * @brief Gets the number of connected clients.
     *
     * @return An integer representing the value of connected clients.
     */
    int get_connected();

    /**
     * @brief Prints a newline then a line of text on the server terminal.
     *
     * @param text The text to print.
     */
    static void print(const std::string& text);

    /**
     * @brief Prints directly a line of text on the server teminal.
     *
     * @param text The text to print.
     */
    static void print_no_new_line(const std::string& text);
    /**
     * @brief Prints all available commands on the server terminal.
     */
    static void print_help();

    void stop();
};

#endif  // SERVER_HXX