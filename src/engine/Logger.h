#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger {
private:
    std::ofstream logFile;
    bool consoleOutput;
    std::string logFilePath;

public:
    Logger();
    ~Logger();

    bool init(const std::string& filePath, bool outputToConsole = true);
    void log(LogLevel level, const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);
    void close();
};

#endif // LOGGER_H