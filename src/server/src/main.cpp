#include <cstring>
#include <iomanip>
#include <iostream>

#include "../include/Server.hxx"

int main() {
    if (Server::is_set() == true) {
        bool success = Server::login();
        if (success == false) {
            Server::print("Wrong credentials. Server shutting down...");
            return 0;
        }
    } else {
        Server::setup();
    }

    Server::spawn();
    Server::print("Enter \"help\" to see available commands");
    std::string word;
    bool        run = true;
    while (run) {
        std::cin >> word;
        if (stricmp("stop", word.c_str()) == 0) run = false;
        else if (stricmp("stats", word.c_str()) == 0) {
            Server::print_no_new_line(
                "Connected clients: " +
                std::to_string(Server::get().get_connected()));
        } else if (stricmp("help", word.c_str()) == 0) {
            Server::print_help();
        } else if (stricmp("log-show", word.c_str()) == 0) {
            int entries;
            std::cin >> entries;
            Logger::get().print_last_n_lines(entries);
            std::cout << "<<PassLock Server>>$ ";
        } else if (stricmp("log-showall", word.c_str()) == 0) {
            Logger::get().print_last_n_lines(
                Logger::get().get_log_entries_count());
            std::cout << "<<PassLock Server>>$ ";
        } else if (stricmp("log-count", word.c_str()) == 0) {
            int entries = Logger::get().get_log_entries_count();
            Server::print_no_new_line("Total number of log entries: " +
                                      std::to_string(entries));
        } else {
            Server::print_no_new_line("Unknown command!");
        }
    }
    Server::get().stop();
    Server::kill();
    return 0;
}