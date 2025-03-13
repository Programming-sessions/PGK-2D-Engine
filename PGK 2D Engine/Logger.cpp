#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

Logger::Logger() : consoleOutput(true) {}

Logger::~Logger() {
    close();
}

bool Logger::init(const std::string& filePath, bool outputToConsole) {
    logFilePath = filePath;
    consoleOutput = outputToConsole;

    logFile.open(filePath, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        if (consoleOutput) {
            std::cerr << "Failed to open log file: " << filePath << std::endl;
        }
        return false;
    }

    return true;
}

void Logger::log(LogLevel level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    // Bezpieczna konwersja czasu do ci¹gu znaków
    char timeStr[26];
    ctime_s(timeStr, sizeof(timeStr), &time);
    std::string timeString(timeStr);
    // Usuniêcie znaku nowej linii, który dodaje ctime_s
    if (!timeString.empty() && timeString[timeString.length() - 1] == '\n') {
        timeString.erase(timeString.length() - 1);
    }

    std::string levelString;
    switch (level) {
    case LogLevel::INFO:
        levelString = "INFO";
        break;
    case LogLevel::WARNING:
        levelString = "WARNING";
        break;
    case LogLevel::ERROR:
        levelString = "ERROR";
        break;
    case LogLevel::FATAL:
        levelString = "FATAL";
        break;
    }

    std::string logMessage = "[" + levelString + "] " +
        timeString + " - " + message;

    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
        logFile.flush();
    }

    if (consoleOutput) {
        if (level == LogLevel::ERROR || level == LogLevel::FATAL) {
            std::cerr << logMessage << std::endl;
        }
        else {
            std::cout << logMessage << std::endl;
        }
    }
}


void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::fatal(const std::string& message) {
    log(LogLevel::FATAL, message);
}

void Logger::close() {
    if (logFile.is_open()) {
        logFile.close();
    }
}
