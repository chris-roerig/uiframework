#include "UI.h"
#include <memory>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    // Create UIManager with a window.
    ui::UIManager uiManager("Reusable SDL UI", 800, 600,
                            ui::Color::SolarizedLightBackground(),
                            ui::Color::SolarizedLightForeground());

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


    // Create an OptionSelect with vertical options.
    // The parameters are: x, y, width, height, options, initial interactive index, active index, callback.
    // Height is set to 150 to accommodate 3 options (each approximately 50 pixels tall).
    std::vector<std::string> options = { "Option 1", "Option 2", "Option 3" };
    auto optionSelect = std::make_shared<ui::OptionSelect>(
        50, 290, 200, 150, options, 0, 1, [](int idx) {
            std::cout << "Option selected index: " << idx << std::endl;
        }
    );
    uiManager.addElement(optionSelect);

    // Create an OptionSelect with vertical options.
    // The parameters are: x, y, width, height, options, initial interactive index, active index, callback.
    // Height is set to 150 to accommodate 3 options (each approximately 50 pixels tall).
    std::vector<std::string> options2 = { "Option 1", "Option 2", "Option 3" };
    auto option2Select = std::make_shared<ui::OptionSelect>(
        50, 490, 200, 150, options, 0, 1, [](int idx) {
            std::cout << "Option selected index: " << idx << std::endl;
        }
    );
    uiManager.addElement(option2Select);


    // Create a Canvas and draw some shapes.
    auto canvas = std::make_shared<ui::Canvas>(300, 50, 400, 300);
    // Draw a rectangle.
    SDL_Rect rect = { 50, 50, 100, 100 };
    canvas->rect(rect, ui::Color(255, 0, 0));
    // Draw a line.
    canvas->line(50, 50, 150, 150, ui::Color(0, 0, 255));
    uiManager.addElement(canvas);

    // Run the UI loop.
    uiManager.run();
    return 0;
}

