#pragma once
#include "UIElement.h"
#include "uiframework/Theme/Color.h"
#include <vector>
#include <string>
#include <functional>
#include <mutex>

namespace ui {

class Canvas : public UIElement {
private:
    mutable std::mutex drawCommandsMutex;
    std::vector<std::function<void(SDL_Renderer*)>> drawCommands;
    // Real-time safe data structures
    struct WaveformData {
        std::vector<float> samples;
        Color color;
        float amplitude = 1.0f;
        bool dirty = false;
    };
    
    struct LevelMeterData {
        float level = 0.0f;
        float peak = 0.0f;
        Color fillColor;
        Color peakColor;
        bool dirty = false;
    };
    
    struct SpectrumData {
        std::vector<float> bins;
        Color color;
        bool logarithmic = true;
        bool dirty = false;
    };
    
    // Real-time safe buffers (lock-free)
    mutable std::atomic<WaveformData*> waveformBuffer{nullptr};
    mutable std::atomic<LevelMeterData*> levelMeterBuffer{nullptr};
    mutable std::atomic<SpectrumData*> spectrumBuffer{nullptr};
    
    // Double buffering for real-time updates
    WaveformData waveformBuffers[2];
    LevelMeterData levelMeterBuffers[2];
    SpectrumData spectrumBuffers[2];
    std::atomic<int> currentWaveformBuffer{0};
    std::atomic<int> currentLevelMeterBuffer{0};
    std::atomic<int> currentSpectrumBuffer{0};
    size_t maxCapacity = 10000; // Default capacity limit
    
    // Phase 3: Performance optimization
    struct BatchedCommand {
        enum Type { RECT, LINE, POINT, CIRCLE, TEXTURE } type;
        SDL_Rect rect;
        Color color;
        SDL_Texture* texture = nullptr;
        int x1, y1, x2, y2; // For lines
    };
    
    std::vector<BatchedCommand> batchedCommands;
    mutable std::mutex batchMutex;
    
    // Dirty rectangle tracking
    SDL_Rect dirtyRect = {0, 0, 0, 0};
    bool hasDirtyRect = false;
    mutable std::mutex dirtyMutex;
    
public:
    Canvas(int x_, int y_, int w_, int h_) : UIElement(x_, y_, w_, h_) {
        drawCommands.reserve(1000); // Pre-allocate reasonable capacity
    }
    
    // Memory management
    void setMaxCapacity(size_t capacity) { maxCapacity = capacity; }
    size_t getMaxCapacity() const { return maxCapacity; }
    size_t getCommandCount() const { 
        std::lock_guard<std::mutex> lock(drawCommandsMutex);
        return drawCommands.size(); 
    }
    void reserveCapacity(size_t capacity) {
        std::lock_guard<std::mutex> lock(drawCommandsMutex);
        drawCommands.reserve(std::min(capacity, maxCapacity));
    }
    
protected:
    void renderImpl(const RenderContext& ctx) override;
    
public:
    // Drawing methods (thread-safe)
    void rect(const SDL_Rect &rect, const Color &color);
    void filledRect(const SDL_Rect &rect, const Color &color);
    void line(int x1, int y1, int x2, int y2, const Color &color);
    void point(int x, int y, const Color &color);
    void clear();
    void clearAndReserve(); // Clear but preserve capacity
    
    // Convenience methods with relative coordinates
    void rectRel(int relX, int relY, int w, int h, const Color &color);
    void filledRectRel(int relX, int relY, int w, int h, const Color &color);
    void lineRel(int x1, int y1, int x2, int y2, const Color &color);
    void pointRel(int relX, int relY, const Color &color);
    
    // Real-time safe methods (lock-free, audio thread safe)
    void realtimeSetWaveform(const float* samples, size_t count, const Color& color, float amplitude = 1.0f);
    void realtimeSetLevelMeter(float level, float peak, const Color& fillColor, const Color& peakColor);
    void realtimeSetSpectrum(const float* bins, size_t count, const Color& color, bool logarithmic = true);
    void realtimeClearWaveform();
    void realtimeClearLevelMeter();
    void realtimeClearSpectrum();
    
    // Audio visualization primitives (thread-safe)
    void waveform(const std::vector<float>& samples, const Color& color, float amplitude = 1.0f);
    void levelMeter(float level, float peak, const Color& fillColor, const Color& peakColor);
    void spectrumBars(const std::vector<float>& bins, const Color& color, bool logarithmic = true);
    void gradientRect(const SDL_Rect& rect, const Color& startColor, const Color& endColor, bool vertical = true);
    
    // Phase 3: Performance optimization methods
    void batchRect(const SDL_Rect& rect, const Color& color);
    void batchLine(int x1, int y1, int x2, int y2, const Color& color);
    void batchPoint(int x, int y, const Color& color);
    void flushBatch();
    void markDirty(const SDL_Rect& rect);
    void clearDirty();
    SDL_Rect getDirtyRect() const;
    
private:
    void renderWaveform(SDL_Renderer* renderer);
    void renderLevelMeter(SDL_Renderer* renderer);
    void renderSpectrum(SDL_Renderer* renderer);
    void swapWaveformBuffer();
    void swapLevelMeterBuffer();
    void swapSpectrumBuffer();
    
    // Phase 3: Performance optimization helpers
    void renderBatchedCommands(SDL_Renderer* renderer);
    void optimizeRenderPipeline(SDL_Renderer* renderer);
    SDL_Rect unionRect(const SDL_Rect& a, const SDL_Rect& b);
};

} // namespace ui
