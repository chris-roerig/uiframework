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
    std::shared_ptr<ui::Button> addTextBoxBtn;
    std::shared_ptr<ui::Button> addCanvasBtn;
    std::shared_ptr<ui::Button> showLayoutBtn;
    std::shared_ptr<ui::Label> instructionsLabel;
    std::shared_ptr<ui::Canvas> designArea;
    
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
    void addTextBox();
    void addCanvas();
    void showLayout();
    void handleEvents();
    DesignElement* findElementAt(int x, int y);
    bool isInDesignArea(int x, int y);
};
