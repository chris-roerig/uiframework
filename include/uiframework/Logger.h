#pragma once
#include <string>
#include <iostream>
#include <mutex>
#include <atomic>

namespace ui {

enum class LogLevel { DEBUG, INFO, WARNING, ERROR, FATAL };

class Logger {
public:
    static void log(LogLevel level, const std::string& message);
    static void setLevel(LogLevel minLevel);
    static LogLevel getLevel();

private:
    static std::atomic<LogLevel> currentLevel;
    static std::mutex& getLogMutex();
    static const char* levelToString(LogLevel level);
    static bool isShuttingDown();
};

} // namespace ui
