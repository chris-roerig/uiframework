#include "UIAbstraction.h"
#include <memory>
#include <vector>
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>

// Helper function to draw a six-pointed star.
static void drawStar(SDL_Renderer* renderer, int centerX, int centerY, int outerRadius, int innerRadius, const ui::Color &color) {
    const int numPoints = 12; // 6 outer and 6 inner vertices.
    SDL_Point points[numPoints];
    double angle = -M_PI / 2;         // Start at the top (270°).
    double angleStep = M_PI / 6;        // 30° steps.
    for (int i = 0; i < numPoints; i++) {
        double r = (i % 2 == 0) ? outerRadius : innerRadius;
        points[i].x = centerX + static_cast<int>(r * cos(angle));
        points[i].y = centerY + static_cast<int>(r * sin(angle));
        angle += angleStep;
    }
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLines(renderer, points, numPoints);
    // Connect last point to first.
    SDL_RenderDrawLine(renderer, points[numPoints - 1].x, points[numPoints - 1].y, points[0].x, points[0].y);
}

// Helper function to draw the Chicago flag on a given canvas.
// flagX, flagY, flagWidth, flagHeight define the flag's area within the canvas.
void drawChicagoFlag(ui::Canvas* canvas, int flagX, int flagY, int flagWidth, int flagHeight) {
    // Add a drawing command lambda to the canvas.
    canvas->drawCommands.push_back([=](SDL_Renderer* renderer) {
        // 1. Fill flag area with white.
        SDL_Rect flagRect = { flagX, flagY, flagWidth, flagHeight };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &flagRect);
        
        // 2. Draw two horizontal blue stripes.
        // Let each stripe be 1/8 of flag height thick.
        int stripeHeight = flagHeight / 8;
        // Top stripe: centered vertically at 1/4 of flag height.
        SDL_Rect topStripe = { flagX, flagY + flagHeight / 4 - stripeHeight / 2, flagWidth, stripeHeight };
        SDL_SetRenderDrawColor(renderer, 0, 172, 237, 255); // Chicago blue (approximate).
        SDL_RenderFillRect(renderer, &topStripe);
        // Bottom stripe: centered at 3/4 of flag height.
        SDL_Rect bottomStripe = { flagX, flagY + 3 * flagHeight / 4 - stripeHeight / 2, flagWidth, stripeHeight };
        SDL_RenderFillRect(renderer, &bottomStripe);
        
        // 3. Draw four red six-pointed stars in a horizontal row.
        int numStars = 4;
        // Compute horizontal spacing: leave a margin equal to spacing on left and right.
        int spacing = flagWidth / (numStars + 1);
        int centerY = flagY + flagHeight / 2;
        // Choose star size relative to flag height.
        int outerRadius = flagHeight / 10;  
        int innerRadius = outerRadius / 2;  // Typical ratio.
        for (int i = 0; i < numStars; i++) {
            int centerX = flagX + spacing * (i + 1);
            drawStar(renderer, centerX, centerY, outerRadius, innerRadius, ui::Color(206, 17, 38)); // Chicago red.
        }
    });
}


int main() {
    // Create the UI abstraction with a window title and dimensions.
    auto ui = std::make_unique<UIAbstraction>("Demo UI", 800, 600);

    ui->label("This is a test", 50, 50);
    ui->label("This is another test", 50, 200);
    ui->button("Click Me", 50, 100, [](){
         std::cout << "Button clicked!" << std::endl;
    });
    ui->textBox("Default text", 50, 170);
    ui->checkBox(false, 50, 240, [](bool state){
         std::cout << "Checkbox state: " << state << std::endl;
    });
    
    // A sample OptionSelect (generic)
    std::vector<std::string> opts = { "Option 1", "Option 2", "Option 3" };
    ui->optionSelect(0, opts, 50, 310, [](int idx){
         std::cout << "Option selected: " << idx << std::endl;
    });
    
    // Theme selection OptionSelect.
    // The themes are: Solarized Dark, Solarized Light, and Molokai.
    std::vector<std::string> themes = { "Solarized Dark", "Solarized Light", "Molokai" };
    // Use a height that fits three options (e.g., 150 pixels).
    ui->optionSelect(0, themes, 50, 420, [uiPtr = ui.get()](int idx) {
         std::cout << "Theme selected index: " << idx << std::endl;
         switch (idx) {
             case 0:
                 uiPtr->setBackgroundColor(ui::Color::SolarizedDarkBackground());
                 uiPtr->setFontColor(ui::Color::SolarizedDarkForeground());
                 uiPtr->setHighlightColor(ui::Color::SolarizedDarkHighlight());
                 break;
             case 1:
                 uiPtr->setBackgroundColor(ui::Color::SolarizedLightBackground());
                 uiPtr->setFontColor(ui::Color::SolarizedLightForeground());
                 uiPtr->setHighlightColor(ui::Color::SolarizedLightHighlight());
                 break;
             case 2:
                 uiPtr->setBackgroundColor(ui::Color::MolokaiBackground());
                 uiPtr->setFontColor(ui::Color::MolokaiForeground());
                 uiPtr->setHighlightColor(ui::Color::MolokaiHighlight());
                 break;
         }
    });
    
    // Create a canvas sized to display the Chicago flag.
    // Here, we set the canvas size to 200x100.
    auto canvas = ui->canvas(400, 10, 200, 100);
    
    // Draw the Chicago flag on the canvas.
    // We let the flag fill the entire canvas area.
    drawChicagoFlag(canvas, 400, 10, 200, 100);
    
    ui->run();
    return 0;
}

