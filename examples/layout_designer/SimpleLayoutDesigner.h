#pragma once

#include "uiframework/UI.h"
#include <memory>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

struct DesignRect {
    std::string type;
    int x, y, width, height;
    bool isDragging = false;
    bool isResizing = false;
    int dragOffsetX = 0, dragOffsetY = 0;
    SDL_Color color;
    
    DesignRect(const std::string& t, int px, int py, int w, int h, SDL_Color c)
        : type(t), x(px), y(py), width(w), height(h), color(c) {}
        
    bool contains(int mx, int my) const {
        return mx >= x && mx < x + width && my >= y && my < y + height;
    }
    
    bool isOnResizeHandle(int mx, int my) const {
        // Bottom-right corner resize handle (10x10 pixels)
        return mx >= x + width - 10 && mx < x + width &&
               my >= y + height - 10 && my < y + height;
    }
};

class SimpleLayoutDesigner {
private:
    UI ui;
    std::vector<DesignRect> designRects;
    std::shared_ptr<ui::Canvas> canvas;
    
    // Toolbar buttons
    std::shared_ptr<ui::Button> addButtonBtn;
    std::shared_ptr<ui::Button> addLabelBtn;
    std::shared_ptr<ui::Button> addTextBoxBtn;
    std::shared_ptr<ui::Button> addCanvasBtn;
    std::shared_ptr<ui::Button> showLayoutBtn;
    std::shared_ptr<ui::Button> clearAllBtn;
    
    std::shared_ptr<ui::Label> instructionsLabel;
    std::shared_ptr<ui::Label> positionLabel;
    
    int mouseX = 0, mouseY = 0;
    bool mouseDown = false;
    DesignRect* selectedRect = nullptr;
    
    static constexpr int TOOLBAR_HEIGHT = 80;
    static constexpr int DESIGN_AREA_Y = TOOLBAR_HEIGHT + 10;
    
public:
    SimpleLayoutDesigner();
    void run();
    
private:
    void setupUI();
    void addButton();
    void addLabel();
    void addTextBox();
    void addCanvas();
    void showLayout();
    void clearAll();
    void updateCanvas();
    void updatePositionLabel();
    void drawSimpleText(int x, int y, const std::string& text);
    DesignRect* findRectAt(int x, int y);
    bool isInDesignArea(int x, int y);
};
