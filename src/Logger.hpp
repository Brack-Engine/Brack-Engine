// Logger.hpp

#ifndef BRACKOCALYPSE_LOGGER_HPP
#define BRACKOCALYPSE_LOGGER_HPP

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <condition_variable>
#include <thread>
#include <chrono>

// Log levels definitions
#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

// If CURRENT_LOG_LEVEL is not defined elsewhere, set it to the most verbose level
#ifndef CURRENT_LOG_LEVEL
#define CURRENT_LOG_LEVEL LOG_LEVEL_DEBUG
#endif

class Logger {
public:
    // Public interface for logging
    static void error(const std::string &message);

    static void warning(const std::string &message);

    static void info(const std::string &message);

    static void debug(const std::string &message);

    void CheckAndCleanOldLogs();

    void initialize();

    void shutdown();

    // Deleted copy constructor and assignment operator for singleton
    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

// Static method to get the singleton instance
    static Logger &GetInstance();

private:
    // Private constructor and destructor
    Logger();

    ~Logger();

    // Instance methods to perform the actual logging
    void logError(const std::string &message);

    void logWarning(const std::string &message);

    void logInfo(const std::string &message);

    void logDebug(const std::string &message);

    void log(const std::string &level, const std::string &message);

    void processLogQueue();

#ifdef LOG_TO_FILE
    std::ofstream logFile;
    std::mutex fileMutex;
    void openLogFile(const std::string& filename);
#endif

    std::vector<std::string> log_queue_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::thread logging_thread_;
    bool shutdown_ = false;

    void CreateDirectories(const std::string &dir);
};

#endif // BRACKOCALYPSE_LOGGER_HPP
