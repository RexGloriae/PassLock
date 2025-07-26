#ifndef LOGGER_HXX
#define LOGGER_HXX

#include <string>

class Logger {
   private:
    static Logger* m_inst;

    std::string m_path = "../data/server_logs.plog";

    Logger() = default;
    ~Logger() = default;

   public:
    Logger(const Logger& other) = delete;
    Logger(Logger&& other) = delete;

    static void    init();
    static Logger& get();
    static void    kill();

    /**
     * @brief Prints a message in the log files.
     *
     * @param message Message to print.
     */
    void write(const std::string& message);

    /**
     * @brief Returns the number of entries in the log file.
     *
     * @return Integer representing the number of entries in the log file.
     */
    int get_log_entries_count();

    /**
     * @brief Prints a number of most recent entries in the log file.
     *
     * @param n Number of recent entries to print.
     */
    void print_last_n_lines(int n);
};

#endif