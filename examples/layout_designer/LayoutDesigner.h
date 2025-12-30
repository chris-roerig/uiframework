#pragma once

#include "uiframework/UI.h"
#include <memory>
#include <vector>
#include <string>

struct DesignElement {
    std::shared_ptr<ui::UIElement> element;
    std::string type;
    int x, y, width, height;
    
    DesignElement(std::shared_ptr<ui::UIElement> elem, const std::string& t, int px, int py, int w, int h)
        : element(elem), type(t), x(px), y(py), width(w), height(h) {}
};

class LayoutDesigner {
private:
    UI ui;
    std::vector<DesignElement> designElements;
    
    // Toolbar buttons - Row 1
    std::shared_ptr<ui::Button> addButtonBtn;
    std::shared_ptr<ui::Button> addLabelBtn;
    std::shared_ptr<ui::Button> addTextBoxBtn;
    std::shared_ptr<ui::Button> addCheckBoxBtn;
    std::shared_ptr<ui::Button> addCanvasBtn;
    
    // Toolbar buttons - Row 2
    std::shared_ptr<ui::Button> addSliderBtn;
    std::shared_ptr<ui::Button> addProgressBarBtn;
    std::shared_ptr<ui::Button> addListViewBtn;
    std::shared_ptr<ui::Button> addOptionSelectBtn;
    std::shared_ptr<ui::Button> addImageBtn;
    
    // Toolbar buttons - Row 3
    std::shared_ptr<ui::Button> showLayoutBtn;
    std::shared_ptr<ui::Button> clearAllBtn;
    std::shared_ptr<ui::Button> randomizeBtn;
    
    std::shared_ptr<ui::Label> instructionsLabel;
    std::shared_ptr<ui::Label> statsLabel;
    std::shared_ptr<ui::Canvas> designArea;
    
    static constexpr int TOOLBAR_HEIGHT = 120;
    static constexpr int DESIGN_AREA_Y = TOOLBAR_HEIGHT + 10;
    
public:
    LayoutDesigner();
    void run();
    
private:
    void setupToolbar();
    void addButton();
    void addLabel();
    void addTextBox();
    void addCheckBox();
    void addCanvas();
    void addSlider();
    void addProgressBar();
    void addListView();
    void addOptionSelect();
    void addImage();
    void showLayout();
    void clearAll();
    void randomizePositions();
    void updateStats();
    bool isInDesignArea(int x, int y);
};
