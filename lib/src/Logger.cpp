#include "uiframework/Logger.h"

namespace ui {

std::atomic<LogLevel> Logger::currentLevel{LogLevel::WARNING};

std::mutex& Logger::getLogMutex() {
    static std::mutex logMutex;
    return logMutex;
}

bool Logger::isShuttingDown() {
    static std::atomic<bool> shuttingDown{false};
    static struct ShutdownDetector {
        ~ShutdownDetector() { shuttingDown = true; }
    } detector;
    return shuttingDown.load();
}

void Logger::log(LogLevel level, const std::string& message) {
    if (isShuttingDown() || level < currentLevel.load()) {
        return;
    }
    
    try {
        std::lock_guard<std::mutex> lock(getLogMutex());
        std::cerr << "[UI Framework " << levelToString(level) << "] " << message << std::endl;
    } catch (...) {
        // Ignore errors during shutdown
    }
}

void Logger::setLevel(LogLevel minLevel) {
    currentLevel.store(minLevel);
}

LogLevel Logger::getLevel() {
    return currentLevel.load();
}

const char* Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

} // namespace ui
