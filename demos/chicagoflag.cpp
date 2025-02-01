#include "UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>

// Helper function to draw a six-pointed star.
static void drawStar(SDL_Renderer* renderer, int centerX, int centerY, int outerRadius, int innerRadius, const ui::Color &color) {
    const int numPoints = 12;
    SDL_Point points[numPoints];
    double angle = -M_PI / 2;
    double angleStep = M_PI / 6;
    for (int i = 0; i < numPoints; i++) {
        double r = (i % 2 == 0) ? outerRadius : innerRadius;
        points[i].x = centerX + static_cast<int>(r * cos(angle));
        points[i].y = centerY + static_cast<int>(r * sin(angle));
        angle += angleStep;
    }
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLines(renderer, points, numPoints);
    SDL_RenderDrawLine(renderer, points[numPoints - 1].x, points[numPoints - 1].y, points[0].x, points[0].y);
}

// Helper function to draw the Chicago flag on a given canvas.
void drawChicagoFlag(ui::Canvas* canvas, int flagX, int flagY, int flagWidth, int flagHeight) {
    canvas->drawCommands.push_back([=](SDL_Renderer* renderer) {
        SDL_Rect flagRect = { flagX, flagY, flagWidth, flagHeight };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &flagRect);
        int stripeHeight = flagHeight / 8;
        SDL_Rect topStripe = { flagX, flagY + flagHeight / 4 - stripeHeight / 2, flagWidth, stripeHeight };
        SDL_SetRenderDrawColor(renderer, 0, 172, 237, 255);
        SDL_RenderFillRect(renderer, &topStripe);
        SDL_Rect bottomStripe = { flagX, flagY + 3 * flagHeight / 4 - stripeHeight / 2, flagWidth, stripeHeight };
        SDL_RenderFillRect(renderer, &bottomStripe);
        int numStars = 4;
        int spacing = flagWidth / (numStars + 1);
        int centerY = flagY + flagHeight / 2;
        int outerRadius = flagHeight / 10;
        int innerRadius = outerRadius / 2;
        for (int i = 0; i < numStars; i++) {
            int centerX = flagX + spacing * (i + 1);
            drawStar(renderer, centerX, centerY, outerRadius, innerRadius, ui::Color(206, 17, 38));
        }
    });
}

int main() {
    auto ui = std::make_unique<UI>("Demo UI", 800, 600);
    
    ui->label("This is a test", 50, 50);
    ui->button("Click Me", 50, 100, [](){ std::cout << "Button clicked!" << std::endl; });
    ui->textBox("Default text", 50, 170);
    ui->checkBox(false, 50, 240, [](bool state){ std::cout << "Checkbox state: " << state << std::endl; });
    
    std::vector<std::string> opts = { "Option 1", "Option 2", "Option 3" };
    ui->optionSelect(0, opts, 50, 310, [](int idx){ std::cout << "Option selected: " << idx << std::endl; });
    
    std::vector<std::string> themes = { "FrameworkDefault", "SolarizedDark", "SolarizedLight", "Molokai" };
    ui->optionSelect(0, themes, 50, 420, [uiPtr = ui.get()](int idx) {
         switch(idx) {
             case 0: uiPtr->setTheme("FrameworkDefault"); break;
             case 1: uiPtr->setTheme("SolarizedDark"); break;
             case 2: uiPtr->setTheme("SolarizedLight"); break;
             case 3: uiPtr->setTheme("Molokai"); break;
         }
         std::cout << "Theme changed to index: " << idx << std::endl;
    });
    
    auto canvas = ui->canvas(400, 10, 200, 100);
    drawChicagoFlag(canvas, 0, 0, 200, 100);
    
    ui->run();
    return 0;
}
