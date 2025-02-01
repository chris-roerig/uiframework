#include "UI.h"
#include <memory>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    // Create UIManager with an initial theme.
    ui::UIManager uiManager("Reusable SDL UI", 800, 600,
                            ui::Color::SolarizedDarkBackground(),
                            ui::Color::SolarizedDarkForeground());

    // Create a Label.
    auto label = std::make_shared<ui::Label>(50, 50, "Press TAB to change focus");
    uiManager.addElement(label);

    // Create a Button.
    auto button = std::make_shared<ui::Button>(50, 100, 200, 50, "Click Me", [](){
        std::cout << "Button clicked!" << std::endl;
    });
    uiManager.addElement(button);

    // Create a TextBox.
    auto textBox = std::make_shared<ui::TextBox>(50, 170, 200, 50);
    uiManager.addElement(textBox);

    // Create a CheckBox.
    auto checkBox = std::make_shared<ui::CheckBox>(50, 240, 30, false, [](bool state){
        std::cout << "CheckBox state: " << state << std::endl;
    });
    uiManager.addElement(checkBox);

    // Create an OptionSelect for theme switching.
    // The control displays the three theme options vertically.
    std::vector<std::string> themeOptions = { "Solarized Dark", "Solarized Light", "Molokai" };
    // Set the height to accommodate all options (e.g. 3 options * 50 pixels = 150).
    auto themeSelect = std::make_shared<ui::OptionSelect>(
        50, 310, 200, 150, themeOptions,
        0, // initial interactive index
        0, // active theme index defaults to 0 (Solarized Dark)
        // Callback: update the UIManager theme based on selection.
        [&uiManager](int idx) {
            std::cout << "Theme selected index: " << idx << std::endl;
            switch (idx) {
                case 0:
                    uiManager.setBackgroundColor(ui::Color::SolarizedDarkBackground());
                    uiManager.setFontColor(ui::Color::SolarizedDarkForeground());
                    uiManager.setHighlightColor(ui::Color::SolarizedDarkHighlight());
                    break;
                case 1:
                    uiManager.setBackgroundColor(ui::Color::SolarizedLightBackground());
                    uiManager.setFontColor(ui::Color::SolarizedLightForeground());
                    uiManager.setHighlightColor(ui::Color::SolarizedLightHighlight());
                    break;
                case 2:
                    uiManager.setBackgroundColor(ui::Color::MolokaiBackground());
                    uiManager.setFontColor(ui::Color::MolokaiForeground());
                    uiManager.setHighlightColor(ui::Color::MolokaiHighlight());
                    break;
                default:
                    break;
            }
        }
    );
    uiManager.addElement(themeSelect);

    // Create a Canvas and draw some shapes.
    auto canvas = std::make_shared<ui::Canvas>(300, 50, 400, 300);
    SDL_Rect rect = { 50, 50, 100, 100 };
    canvas->rect(rect, ui::Color(255, 0, 0));
    canvas->line(50, 50, 150, 150, ui::Color(0, 0, 255));
    uiManager.addElement(canvas);

    uiManager.run();
    return 0;
}

