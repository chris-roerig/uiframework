#pragma once
#include "UIElement.h"
#include <string>
#include <functional>
#include <vector>

namespace ui {

class Modal : public UIElement {
private:
    int buttonFocusIndex = 0;
    bool dismissed = false;
    
public:
    std::string message;
    std::vector<std::string> buttonLabels;
    std::vector<std::function<void()>> buttonCallbacks;
    std::function<void()> onDismiss;
    
    // Constructor for general modal
    Modal(int x_, int y_, int w_, int h_, const std::string &msg, const std::string &buttonText, 
          bool hasCancel = false, std::function<void()> onCloseCallback = nullptr);
    
    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;
    void handleEvent(const SDL_Event &e) override;
    SDL_Rect getFocusRect() const override;
    bool isInteractive() const override { return true; }
    void activate() override;
    
    // Modal management
    void dismiss();
    bool isDismissed() const { return dismissed; }
    void setMessage(const std::string& msg) { message = msg; }
    
    // Button management
    void addButton(const std::string& label, std::function<void()> callback);
    void clearButtons();
    void setButtonFocus(int index);
    int getButtonFocus() const { return buttonFocusIndex; }
    
    // Utility methods
    void centerOnScreen(int screenWidth, int screenHeight);
    SDL_Rect getButtonRect(int buttonIndex) const;
    int getButtonAt(int mouseX, int mouseY) const;
};

} // namespace ui
