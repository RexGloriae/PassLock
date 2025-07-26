#include "../include/Logger.hxx"

#include <deque>
#include <fstream>
#include <iostream>

Logger* Logger::m_inst = nullptr;

void Logger::init() {
    if (Logger::m_inst == nullptr) {
        Logger::m_inst = new Logger();
    }
}
Logger& Logger::get() { return *(Logger::m_inst); }
void    Logger::kill() {
    if (Logger::m_inst != nullptr) {
        delete Logger::m_inst;
    }
    Logger::m_inst = nullptr;
}

void Logger::write(const std::string& message) {
    std::ofstream out(m_path, std::ios::app);
    out << "[PassLock Log]: " << message << "\n";
    out.close();
}

int Logger::get_log_entries_count() {
    std::ifstream file(m_path);
    int           count = 0;
    std::string   line;
    while (std::getline(file, line)) count++;
    file.close();
    return count;
}

void Logger::print_last_n_lines(int n) {
    std::ifstream           file(m_path);
    std::deque<std::string> buffer;
    std::string             line;

    while (std::getline(file, line)) {
        if (buffer.size() == n) buffer.pop_front();
        buffer.push_back(line);
    }

    std::cout << "<<PassLock Server>>$ Logs:\n";
    int it = 1;
    for (const auto& entry : buffer) {
        std::cout << "<" << it++ << ">\n";
        std::cout << entry << "\n";
    }
}