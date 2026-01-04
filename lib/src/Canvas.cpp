#include "uiframework/UIElements/Canvas.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <atomic>
#include <algorithm>
#include <cmath>

namespace ui {

void Canvas::renderImpl(const RenderContext& ctx) {
    // Set clipping rectangle to canvas bounds
    SDL_Rect clipRect = { x, y, width, height };
    SDL_RenderSetClipRect(ctx.renderer, &clipRect);
    
    // Draw canvas background
    auto colors = ctx.labelColors(); // Canvas uses label colors as fallback
    drawFilledRect(ctx.renderer, clipRect, colors.canvasBackground);
    
    // Phase 3: Optimized rendering pipeline
    optimizeRenderPipeline(ctx.renderer);
    
    // Reset clipping
    SDL_RenderSetClipRect(ctx.renderer, nullptr);
    
    // Draw border if focused
    if (hasFocus) {
        auto colors = ctx.labelColors();
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(ctx.renderer, colors.focusBorder.r, colors.focusBorder.g, colors.focusBorder.b, colors.focusBorder.a);
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }
}

void Canvas::rect(const SDL_Rect &r, const Color &color) {
    std::lock_guard<std::mutex> lock(drawCommandsMutex);
    if (drawCommands.size() >= maxCapacity) return; // Prevent unbounded growth
    
    int canvasX = x, canvasY = y; // Optimize lambda captures
    drawCommands.push_back([canvasX, canvasY, r, color](SDL_Renderer* renderer) {
        SDL_Rect rect = { canvasX + r.x, canvasY + r.y, r.w, r.h };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(renderer, &rect);
    });
}

void Canvas::filledRect(const SDL_Rect &r, const Color &color) {
    std::lock_guard<std::mutex> lock(drawCommandsMutex);
    if (drawCommands.size() >= maxCapacity) return;
    
    int canvasX = x, canvasY = y;
    drawCommands.push_back([canvasX, canvasY, r, color](SDL_Renderer* renderer) {
        SDL_Rect rect = { canvasX + r.x, canvasY + r.y, r.w, r.h };
        drawFilledRect(renderer, rect, color);
    });
}

void Canvas::line(int x1, int y1, int x2, int y2, const Color &color) {
    std::lock_guard<std::mutex> lock(drawCommandsMutex);
    if (drawCommands.size() >= maxCapacity) return;
    
    int canvasX = x, canvasY = y;
    drawCommands.push_back([canvasX, canvasY, x1, y1, x2, y2, color](SDL_Renderer* renderer) {
        drawLine(renderer, canvasX + x1, canvasY + y1, canvasX + x2, canvasY + y2, color);
    });
}

void Canvas::point(int px, int py, const Color &color) {
    std::lock_guard<std::mutex> lock(drawCommandsMutex);
    if (drawCommands.size() >= maxCapacity) return;
    
    int canvasX = x, canvasY = y;
    drawCommands.push_back([canvasX, canvasY, px, py, color](SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint(renderer, canvasX + px, canvasY + py);
    });
}

void Canvas::clear() {
    std::lock_guard<std::mutex> lock(drawCommandsMutex);
    drawCommands.clear();
}

void Canvas::clearAndReserve() {
    std::lock_guard<std::mutex> lock(drawCommandsMutex);
    size_t currentCapacity = drawCommands.capacity();
    drawCommands.clear();
    drawCommands.reserve(currentCapacity); // Preserve capacity
}

// Real-time safe methods (lock-free, audio thread safe)
void Canvas::realtimeSetWaveform(const float* samples, size_t count, const Color& color, float amplitude) {
    int bufferIndex = 1 - currentWaveformBuffer.load(std::memory_order_acquire);
    WaveformData& buffer = waveformBuffers[bufferIndex];
    
    buffer.samples.assign(samples, samples + count);
    buffer.color = color;
    buffer.amplitude = amplitude;
    buffer.dirty = true;
    
    currentWaveformBuffer.store(bufferIndex, std::memory_order_release);
}

void Canvas::realtimeSetLevelMeter(float level, float peak, const Color& fillColor, const Color& peakColor) {
    int bufferIndex = 1 - currentLevelMeterBuffer.load(std::memory_order_acquire);
    LevelMeterData& buffer = levelMeterBuffers[bufferIndex];
    
    buffer.level = level;
    buffer.peak = peak;
    buffer.fillColor = fillColor;
    buffer.peakColor = peakColor;
    buffer.dirty = true;
    
    currentLevelMeterBuffer.store(bufferIndex, std::memory_order_release);
}

void Canvas::realtimeSetSpectrum(const float* bins, size_t count, const Color& color, bool logarithmic) {
    int bufferIndex = 1 - currentSpectrumBuffer.load(std::memory_order_acquire);
    SpectrumData& buffer = spectrumBuffers[bufferIndex];
    
    buffer.bins.assign(bins, bins + count);
    buffer.color = color;
    buffer.logarithmic = logarithmic;
    buffer.dirty = true;
    
    currentSpectrumBuffer.store(bufferIndex, std::memory_order_release);
}

void Canvas::realtimeClearWaveform() {
    int bufferIndex = 1 - currentWaveformBuffer.load(std::memory_order_acquire);
    waveformBuffers[bufferIndex].samples.clear();
    waveformBuffers[bufferIndex].dirty = true;
    currentWaveformBuffer.store(bufferIndex, std::memory_order_release);
}

void Canvas::realtimeClearLevelMeter() {
    int bufferIndex = 1 - currentLevelMeterBuffer.load(std::memory_order_acquire);
    levelMeterBuffers[bufferIndex].level = 0.0f;
    levelMeterBuffers[bufferIndex].peak = 0.0f;
    levelMeterBuffers[bufferIndex].dirty = true;
    currentLevelMeterBuffer.store(bufferIndex, std::memory_order_release);
}

void Canvas::realtimeClearSpectrum() {
    int bufferIndex = 1 - currentSpectrumBuffer.load(std::memory_order_acquire);
    spectrumBuffers[bufferIndex].bins.clear();
    spectrumBuffers[bufferIndex].dirty = true;
    currentSpectrumBuffer.store(bufferIndex, std::memory_order_release);
}

// Audio visualization primitives (thread-safe)
void Canvas::waveform(const std::vector<float>& samples, const Color& color, float amplitude) {
    std::lock_guard<std::mutex> lock(drawCommandsMutex);
    if (drawCommands.size() >= maxCapacity) return;
    
    int canvasX = x, canvasY = y, canvasW = width, canvasH = height;
    drawCommands.push_back([canvasX, canvasY, canvasW, canvasH, samples, color, amplitude](SDL_Renderer* renderer) {
        if (samples.empty()) return;
        
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        int centerY = canvasY + canvasH / 2;
        int maxAmplitude = canvasH / 2;
        
        for (size_t i = 1; i < samples.size(); ++i) {
            int x1 = canvasX + (i - 1) * canvasW / samples.size();
            int y1 = centerY - (int)(samples[i - 1] * amplitude * maxAmplitude);
            int x2 = canvasX + i * canvasW / samples.size();
            int y2 = centerY - (int)(samples[i] * amplitude * maxAmplitude);
            
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    });
}

void Canvas::levelMeter(float level, float peak, const Color& fillColor, const Color& peakColor) {
    std::lock_guard<std::mutex> lock(drawCommandsMutex);
    if (drawCommands.size() >= maxCapacity) return;
    
    int canvasX = x, canvasY = y, canvasW = width, canvasH = height;
    drawCommands.push_back([canvasX, canvasY, canvasW, canvasH, level, peak, fillColor, peakColor](SDL_Renderer* renderer) {
        // Level fill
        int fillHeight = (int)(level * canvasH);
        if (fillHeight > 0) {
            SDL_Rect fillRect = { canvasX, canvasY + canvasH - fillHeight, canvasW, fillHeight };
            drawFilledRect(renderer, fillRect, fillColor);
        }
        
        // Peak indicator
        if (peak > 0.0f) {
            int peakY = canvasY + canvasH - (int)(peak * canvasH);
            SDL_SetRenderDrawColor(renderer, peakColor.r, peakColor.g, peakColor.b, peakColor.a);
            SDL_RenderDrawLine(renderer, canvasX, peakY, canvasX + canvasW - 1, peakY);
        }
    });
}

void Canvas::spectrumBars(const std::vector<float>& bins, const Color& color, bool logarithmic) {
    std::lock_guard<std::mutex> lock(drawCommandsMutex);
    if (drawCommands.size() >= maxCapacity) return;
    
    int canvasX = x, canvasY = y, canvasW = width, canvasH = height;
    drawCommands.push_back([canvasX, canvasY, canvasW, canvasH, bins, color, logarithmic](SDL_Renderer* renderer) {
        if (bins.empty()) return;
        
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        int barWidth = std::max(1, canvasW / (int)bins.size());
        
        for (size_t i = 0; i < bins.size(); ++i) {
            float value = logarithmic ? (bins[i] > 0 ? std::log10(bins[i] + 1) / std::log10(2) : 0) : bins[i];
            int barHeight = (int)(value * canvasH);
            
            if (barHeight > 0) {
                SDL_Rect barRect = { 
                    canvasX + (int)i * barWidth, 
                    canvasY + canvasH - barHeight, 
                    barWidth - 1, 
                    barHeight 
                };
                SDL_RenderFillRect(renderer, &barRect);
            }
        }
    });
}

void Canvas::gradientRect(const SDL_Rect& rect, const Color& startColor, const Color& endColor, bool vertical) {
    std::lock_guard<std::mutex> lock(drawCommandsMutex);
    if (drawCommands.size() >= maxCapacity) return;
    
    int canvasX = x, canvasY = y;
    drawCommands.push_back([canvasX, canvasY, rect, startColor, endColor, vertical](SDL_Renderer* renderer) {
        SDL_Rect adjustedRect = { canvasX + rect.x, canvasY + rect.y, rect.w, rect.h };
        
        if (vertical) {
            for (int i = 0; i < adjustedRect.h; ++i) {
                float ratio = (float)i / adjustedRect.h;
                Color blendColor = {
                    (Uint8)(startColor.r + ratio * (endColor.r - startColor.r)),
                    (Uint8)(startColor.g + ratio * (endColor.g - startColor.g)),
                    (Uint8)(startColor.b + ratio * (endColor.b - startColor.b)),
                    (Uint8)(startColor.a + ratio * (endColor.a - startColor.a))
                };
                
                SDL_SetRenderDrawColor(renderer, blendColor.r, blendColor.g, blendColor.b, blendColor.a);
                SDL_RenderDrawLine(renderer, adjustedRect.x, adjustedRect.y + i, 
                                 adjustedRect.x + adjustedRect.w - 1, adjustedRect.y + i);
            }
        } else {
            for (int i = 0; i < adjustedRect.w; ++i) {
                float ratio = (float)i / adjustedRect.w;
                Color blendColor = {
                    (Uint8)(startColor.r + ratio * (endColor.r - startColor.r)),
                    (Uint8)(startColor.g + ratio * (endColor.g - startColor.g)),
                    (Uint8)(startColor.b + ratio * (endColor.b - startColor.b)),
                    (Uint8)(startColor.a + ratio * (endColor.a - startColor.a))
                };
                
                SDL_SetRenderDrawColor(renderer, blendColor.r, blendColor.g, blendColor.b, blendColor.a);
                SDL_RenderDrawLine(renderer, adjustedRect.x + i, adjustedRect.y, 
                                 adjustedRect.x + i, adjustedRect.y + adjustedRect.h - 1);
            }
        }
    });
}

// Convenience methods with relative coordinates
void Canvas::rectRel(int relX, int relY, int w, int h, const Color &color) {
    rect({relX, relY, w, h}, color);
}

void Canvas::filledRectRel(int relX, int relY, int w, int h, const Color &color) {
    filledRect({relX, relY, w, h}, color);
}

void Canvas::lineRel(int x1, int y1, int x2, int y2, const Color &color) {
    line(x1, y1, x2, y2, color);
}

void Canvas::pointRel(int relX, int relY, const Color &color) {
    point(relX, relY, color);
}

// Private helper methods for real-time rendering
void Canvas::renderWaveform(SDL_Renderer* renderer) {
    int bufferIndex = currentWaveformBuffer.load(std::memory_order_acquire);
    const WaveformData& data = waveformBuffers[bufferIndex];
    
    if (!data.dirty || data.samples.empty()) return;
    
    SDL_SetRenderDrawColor(renderer, data.color.r, data.color.g, data.color.b, data.color.a);
    int centerY = y + height / 2;
    int maxAmplitude = height / 2;
    
    for (size_t i = 1; i < data.samples.size(); ++i) {
        int x1 = x + (i - 1) * width / data.samples.size();
        int y1 = centerY - (int)(data.samples[i - 1] * data.amplitude * maxAmplitude);
        int x2 = x + i * width / data.samples.size();
        int y2 = centerY - (int)(data.samples[i] * data.amplitude * maxAmplitude);
        
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void Canvas::renderLevelMeter(SDL_Renderer* renderer) {
    int bufferIndex = currentLevelMeterBuffer.load(std::memory_order_acquire);
    const LevelMeterData& data = levelMeterBuffers[bufferIndex];
    
    if (!data.dirty) return;
    
    // Level fill
    int fillHeight = (int)(data.level * height);
    if (fillHeight > 0) {
        SDL_Rect fillRect = { x, y + height - fillHeight, width, fillHeight };
        drawFilledRect(renderer, fillRect, data.fillColor);
    }
    
    // Peak indicator
    if (data.peak > 0.0f) {
        int peakY = y + height - (int)(data.peak * height);
        SDL_SetRenderDrawColor(renderer, data.peakColor.r, data.peakColor.g, data.peakColor.b, data.peakColor.a);
        SDL_RenderDrawLine(renderer, x, peakY, x + width - 1, peakY);
    }
}

void Canvas::renderSpectrum(SDL_Renderer* renderer) {
    int bufferIndex = currentSpectrumBuffer.load(std::memory_order_acquire);
    const SpectrumData& data = spectrumBuffers[bufferIndex];
    
    if (!data.dirty || data.bins.empty()) return;
    
    SDL_SetRenderDrawColor(renderer, data.color.r, data.color.g, data.color.b, data.color.a);
    int barWidth = std::max(1, width / (int)data.bins.size());
    
    for (size_t i = 0; i < data.bins.size(); ++i) {
        float value = data.logarithmic ? 
            (data.bins[i] > 0 ? std::log10(data.bins[i] + 1) / std::log10(2) : 0) : 
            data.bins[i];
        int barHeight = (int)(value * height);
        
        if (barHeight > 0) {
            SDL_Rect barRect = { 
                x + (int)i * barWidth, 
                y + height - barHeight, 
                barWidth - 1, 
                barHeight 
            };
            SDL_RenderFillRect(renderer, &barRect);
        }
    }
}

// Phase 3: Performance optimization implementations
void Canvas::batchRect(const SDL_Rect& rect, const Color& color) {
    std::lock_guard<std::mutex> lock(batchMutex);
    if (batchedCommands.size() >= maxCapacity) return;
    
    BatchedCommand cmd;
    cmd.type = BatchedCommand::RECT;
    cmd.rect = rect;
    cmd.color = color;
    batchedCommands.push_back(cmd);
    
    markDirty(rect);
}

void Canvas::batchLine(int x1, int y1, int x2, int y2, const Color& color) {
    std::lock_guard<std::mutex> lock(batchMutex);
    if (batchedCommands.size() >= maxCapacity) return;
    
    BatchedCommand cmd;
    cmd.type = BatchedCommand::LINE;
    cmd.x1 = x1; cmd.y1 = y1; cmd.x2 = x2; cmd.y2 = y2;
    cmd.color = color;
    batchedCommands.push_back(cmd);
    
    SDL_Rect lineRect = { std::min(x1, x2), std::min(y1, y2), 
                         std::abs(x2 - x1) + 1, std::abs(y2 - y1) + 1 };
    markDirty(lineRect);
}

void Canvas::batchPoint(int px, int py, const Color& color) {
    std::lock_guard<std::mutex> lock(batchMutex);
    if (batchedCommands.size() >= maxCapacity) return;
    
    BatchedCommand cmd;
    cmd.type = BatchedCommand::POINT;
    cmd.x1 = px; cmd.y1 = py;
    cmd.color = color;
    batchedCommands.push_back(cmd);
    
    SDL_Rect pointRect = { px, py, 1, 1 };
    markDirty(pointRect);
}

void Canvas::flushBatch() {
    std::lock_guard<std::mutex> lock(batchMutex);
    batchedCommands.clear();
    clearDirty();
}

void Canvas::markDirty(const SDL_Rect& rect) {
    std::lock_guard<std::mutex> lock(dirtyMutex);
    if (!hasDirtyRect) {
        dirtyRect = rect;
        hasDirtyRect = true;
    } else {
        dirtyRect = unionRect(dirtyRect, rect);
    }
}

void Canvas::clearDirty() {
    std::lock_guard<std::mutex> lock(dirtyMutex);
    hasDirtyRect = false;
    dirtyRect = {0, 0, 0, 0};
}

SDL_Rect Canvas::getDirtyRect() const {
    std::lock_guard<std::mutex> lock(dirtyMutex);
    return hasDirtyRect ? dirtyRect : SDL_Rect{0, 0, 0, 0};
}

void Canvas::renderBatchedCommands(SDL_Renderer* renderer) {
    std::lock_guard<std::mutex> lock(batchMutex);
    
    // Group by color to reduce SDL state changes
    Color currentColor = {0, 0, 0, 0};
    
    for (const auto& cmd : batchedCommands) {
        if (cmd.color.r != currentColor.r || cmd.color.g != currentColor.g || 
            cmd.color.b != currentColor.b || cmd.color.a != currentColor.a) {
            SDL_SetRenderDrawColor(renderer, cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
            currentColor = cmd.color;
        }
        
        switch (cmd.type) {
            case BatchedCommand::RECT:
                SDL_RenderFillRect(renderer, &cmd.rect);
                break;
            case BatchedCommand::LINE:
                SDL_RenderDrawLine(renderer, cmd.x1, cmd.y1, cmd.x2, cmd.y2);
                break;
            case BatchedCommand::POINT:
                SDL_RenderDrawPoint(renderer, cmd.x1, cmd.y1);
                break;
            case BatchedCommand::CIRCLE:
            case BatchedCommand::TEXTURE:
                // Not implemented yet
                break;
        }
    }
}

void Canvas::optimizeRenderPipeline(SDL_Renderer* renderer) {
    // Render batched commands first (optimized)
    renderBatchedCommands(renderer);
    
    // Execute legacy draw commands (thread-safe)
    {
        std::lock_guard<std::mutex> lock(drawCommandsMutex);
        for (auto& cmd : drawCommands) {
            if (cmd) {
                cmd(renderer);
            }
        }
    }
    
    // Render real-time data (lock-free)
    renderWaveform(renderer);
    renderLevelMeter(renderer);
    renderSpectrum(renderer);
}

SDL_Rect Canvas::unionRect(const SDL_Rect& a, const SDL_Rect& b) {
    int x1 = std::min(a.x, b.x);
    int y1 = std::min(a.y, b.y);
    int x2 = std::max(a.x + a.w, b.x + b.w);
    int y2 = std::max(a.y + a.h, b.y + b.h);
    return {x1, y1, x2 - x1, y2 - y1};
}

} // namespace ui
