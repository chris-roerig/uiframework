#pragma once

#include "uiframework/UI.h"
#include <memory>
#include <vector>
#include <string>

struct DesignElement {
    std::shared_ptr<ui::UIElement> element;
    std::string type;
    int x, y, width, height;
    bool isDragging = false;
    bool isResizing = false;
    int dragOffsetX = 0, dragOffsetY = 0;
    
    DesignElement(std::shared_ptr<ui::UIElement> elem, const std::string& t, int px, int py, int w, int h)
        : element(elem), type(t), x(px), y(py), width(w), height(h) {}
};

class LayoutDesigner {
private:
    UI ui;
    std::vector<DesignElement> designElements;
    std::shared_ptr<ui::Button> addButtonBtn;
    std::shared_ptr<ui::Button> addLabelBtn;
    std::shared_ptr<ui::Button> addTextInputBtn;
    std::shared_ptr<ui::Button> addCanvasBtn;
    std::shared_ptr<ui::Button> showLayoutBtn;
    std::shared_ptr<ui::Label> instructionsLabel;
    
    int mouseX = 0, mouseY = 0;
    bool mouseDown = false;
    DesignElement* draggedElement = nullptr;
    
    static constexpr int TOOLBAR_HEIGHT = 80;
    static constexpr int DESIGN_AREA_Y = TOOLBAR_HEIGHT + 10;
    
public:
    LayoutDesigner();
    void run();
    
private:
    void setupToolbar();
    void addButton();
    void addLabel();
    void addTextInput();
    void addCanvas();
    void showLayout();
    void handleMouseDown(int x, int y);
    void handleMouseUp(int x, int y);
    void handleMouseMove(int x, int y);
    DesignElement* findElementAt(int x, int y);
    bool isInDesignArea(int x, int y);
};
