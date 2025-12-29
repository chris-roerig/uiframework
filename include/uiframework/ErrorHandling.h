#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <stdexcept>
#include <string>
#include "Logger.h"

namespace ui {

// Forward declaration
class Theme;

// Exception classes for better error handling
class UIException : public std::runtime_error {
public:
    explicit UIException(const std::string& message) : std::runtime_error(message) {}
};

class InitializationException : public UIException {
public:
    explicit InitializationException(const std::string& message) 
        : UIException("Initialization failed: " + message) {}
};

class RenderException : public UIException {
public:
    explicit RenderException(const std::string& message) 
        : UIException("Render failed: " + message) {}
};

// Error severity levels (deprecated - use LogLevel instead)
enum class ErrorSeverity { INFO, WARNING, ERROR, FATAL };

/**
 * Standardized error handling utilities for UI elements
 */
namespace ErrorHandling {
    
    /**
     * @brief Log message with specified severity level (deprecated - use Logger::log)
     */
    [[deprecated("Use Logger::log instead")]]
    inline void log(ErrorSeverity level, const std::string& message) {
        LogLevel logLevel;
        switch (level) {
            case ErrorSeverity::INFO: logLevel = LogLevel::INFO; break;
            case ErrorSeverity::WARNING: logLevel = LogLevel::WARNING; break;
            case ErrorSeverity::ERROR: logLevel = LogLevel::ERROR; break;
            case ErrorSeverity::FATAL: logLevel = LogLevel::FATAL; break;
        }
        Logger::log(logLevel, message);
    }
    
    /**
     * @brief Set minimum log level for filtering messages (deprecated - use Logger::setLevel)
     */
    [[deprecated("Use Logger::setLevel instead")]]
    inline void setLogLevel(ErrorSeverity level) {
        LogLevel logLevel;
        switch (level) {
            case ErrorSeverity::INFO: logLevel = LogLevel::INFO; break;
            case ErrorSeverity::WARNING: logLevel = LogLevel::WARNING; break;
            case ErrorSeverity::ERROR: logLevel = LogLevel::ERROR; break;
            case ErrorSeverity::FATAL: logLevel = LogLevel::FATAL; break;
        }
        Logger::setLevel(logLevel);
    }
    
    /**
     * Validates common render parameters
     * @param renderer SDL renderer (required)
     * @param theme Theme object (required)
     * @param font Font object (optional, can be null)
     * @return true if basic requirements are met for rendering
     */
    inline bool validateRenderParams(const SDL_Renderer* renderer, std::shared_ptr<Theme> theme, const TTF_Font* font = nullptr) {
        return renderer != nullptr && theme != nullptr;
    }
    
    /**
     * Validates text rendering parameters
     * @param renderer SDL renderer (required)
     * @param theme Theme object (required)
     * @param font Font object (required for text)
     * @param text Text content (required, non-empty)
     * @return true if all requirements are met for text rendering
     */
    inline bool validateTextRenderParams(const SDL_Renderer* renderer, std::shared_ptr<Theme> theme, const TTF_Font* font, const std::string& text) {
        return renderer != nullptr && theme != nullptr && font != nullptr && !text.empty();
    }
    
    /**
     * Validates SDL surface creation result
     * @param surface SDL surface pointer
     * @return true if surface is valid
     */
    inline bool validateSurface(const SDL_Surface* surface) {
        if (!surface) {
            Logger::log(LogLevel::ERROR, "SDL surface creation failed: " + std::string(SDL_GetError()));
            return false;
        }
        return true;
    }
    
    /**
     * Validates SDL texture creation result
     * @param texture SDL texture pointer
     * @return true if texture is valid
     */
    inline bool validateTexture(const SDL_Texture* texture) {
        if (!texture) {
            Logger::log(LogLevel::ERROR, "SDL texture creation failed: " + std::string(SDL_GetError()));
            return false;
        }
        return true;
    }
}

} // namespace ui
