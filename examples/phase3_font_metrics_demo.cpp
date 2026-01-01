#include "uiframework/UI.h"
#include "uiframework/Utils/TextUtils.h"
#include <iostream>
#include <iomanip>

using namespace ui;

int main() {
    try {
        UI ui("Phase 3: Font Metrics & Layout Demo", 900, 700);
        
        // Title
        auto title = ui.createLabel("Phase 3: Font Metrics & Layout Capabilities", 20, 20);
        title->setFont("Roboto", 18, FontStyle::Bold);
        title->setTextAlignment(UIElement::TextAlignment::TopCenter);
        
        // Font metrics demonstration
        auto metricsTitle = ui.createLabel("Font Metrics Information:", 20, 60);
        metricsTitle->setFont("Roboto", 14, FontStyle::Bold);
        
        // Get font metrics for different fonts
        auto& fontManager = FontManager::getInstance();
        
        // Roboto metrics
        FontMetrics robotoMetrics = fontManager.getFontMetrics("Roboto", 12, FontStyle::Regular);
        auto robotoInfo = ui.createLabel(
            "Roboto 12px: Ascent=" + std::to_string(robotoMetrics.ascent) + 
            ", Descent=" + std::to_string(robotoMetrics.descent) + 
            ", Height=" + std::to_string(robotoMetrics.height) + 
            ", LineSkip=" + std::to_string(robotoMetrics.lineSkip), 
            40, 90
        );
        robotoInfo->setFont("Console", 10);
        
        // Console metrics
        FontMetrics consoleMetrics = fontManager.getFontMetrics("Console", 12, FontStyle::Regular);
        auto consoleInfo = ui.createLabel(
            "Console 12px: Ascent=" + std::to_string(consoleMetrics.ascent) + 
            ", Descent=" + std::to_string(consoleMetrics.descent) + 
            ", Height=" + std::to_string(consoleMetrics.height) + 
            ", LineSkip=" + std::to_string(consoleMetrics.lineSkip), 
            40, 110
        );
        consoleInfo->setFont("Console", 10);
        
        // Text measurement demonstration
        auto measureTitle = ui.createLabel("Text Measurement:", 20, 150);
        measureTitle->setFont("Roboto", 14, FontStyle::Bold);
        
        std::string sampleText = "Sample Text for Measurement";
        auto textSize = fontManager.getTextSize(sampleText, "Roboto", 14, FontStyle::Regular);
        auto measureInfo = ui.createLabel(
            "\"" + sampleText + "\" (Roboto 14px) = " + 
            std::to_string(textSize.first) + "x" + std::to_string(textSize.second) + " pixels",
            40, 180
        );
        measureInfo->setFont("Console", 10);
        
        // Text alignment demonstration
        auto alignTitle = ui.createLabel("Text Alignment Examples:", 20, 220);
        alignTitle->setFont("Roboto", 14, FontStyle::Bold);
        
        // Create a container area for alignment examples
        SDL_Rect alignmentArea = {50, 250, 300, 150};
        
        // Top alignments
        auto topLeft = ui.createLabel("Top Left", alignmentArea.x, alignmentArea.y);
        topLeft->setFont("Roboto", 12);
        topLeft->setTextAlignment(UIElement::TextAlignment::TopLeft);
        
        auto topCenter = ui.createLabel("Top Center", alignmentArea.x + alignmentArea.w/2 - 40, alignmentArea.y);
        topCenter->setFont("Roboto", 12);
        topCenter->setTextAlignment(UIElement::TextAlignment::TopCenter);
        
        auto topRight = ui.createLabel("Top Right", alignmentArea.x + alignmentArea.w - 80, alignmentArea.y);
        topRight->setFont("Roboto", 12);
        topRight->setTextAlignment(UIElement::TextAlignment::TopRight);
        
        // Middle alignments
        auto middleLeft = ui.createLabel("Middle Left", alignmentArea.x, alignmentArea.y + alignmentArea.h/2 - 10);
        middleLeft->setFont("Roboto", 12);
        middleLeft->setTextAlignment(UIElement::TextAlignment::MiddleLeft);
        
        auto middleCenter = ui.createLabel("Middle Center", alignmentArea.x + alignmentArea.w/2 - 50, alignmentArea.y + alignmentArea.h/2 - 10);
        middleCenter->setFont("Roboto", 12);
        middleCenter->setTextAlignment(UIElement::TextAlignment::MiddleCenter);
        
        auto middleRight = ui.createLabel("Middle Right", alignmentArea.x + alignmentArea.w - 90, alignmentArea.y + alignmentArea.h/2 - 10);
        middleRight->setFont("Roboto", 12);
        middleRight->setTextAlignment(UIElement::TextAlignment::MiddleRight);
        
        // Bottom alignments
        auto bottomLeft = ui.createLabel("Bottom Left", alignmentArea.x, alignmentArea.y + alignmentArea.h - 20);
        bottomLeft->setFont("Roboto", 12);
        bottomLeft->setTextAlignment(UIElement::TextAlignment::BottomLeft);
        
        auto bottomCenter = ui.createLabel("Bottom Center", alignmentArea.x + alignmentArea.w/2 - 50, alignmentArea.y + alignmentArea.h - 20);
        bottomCenter->setFont("Roboto", 12);
        bottomCenter->setTextAlignment(UIElement::TextAlignment::BottomCenter);
        
        auto bottomRight = ui.createLabel("Bottom Right", alignmentArea.x + alignmentArea.w - 90, alignmentArea.y + alignmentArea.h - 20);
        bottomRight->setFont("Roboto", 12);
        bottomRight->setTextAlignment(UIElement::TextAlignment::BottomRight);
        
        // Baseline alignment example
        auto baselineLabel = ui.createLabel("Baseline Aligned", alignmentArea.x + alignmentArea.w/2 - 60, alignmentArea.y + alignmentArea.h/2 + 30);
        baselineLabel->setFont("Roboto", 12);
        baselineLabel->setTextAlignment(UIElement::TextAlignment::Baseline);
        
        // Advanced text utilities demonstration
        auto utilsTitle = ui.createLabel("Advanced Text Utilities:", 400, 220);
        utilsTitle->setFont("Roboto", 14, FontStyle::Bold);
        
        // Text wrapping example
        std::string longText = "This is a very long text that will be wrapped to multiple lines to demonstrate the advanced text wrapping capabilities of the font system.";
        auto wrappedLines = TextUtils::wrapTextAdvanced(longText, "Roboto", 12, 250, FontStyle::Regular);
        
        int yPos = 250;
        for (size_t i = 0; i < wrappedLines.size() && i < 5; ++i) {
            auto line = ui.createLabel(wrappedLines[i], 420, yPos);
            line->setFont("Roboto", 12);
            yPos += 20;
        }
        
        // Text truncation example
        std::string truncatedText = TextUtils::truncateWithEllipsisAdvanced(
            "This text will be truncated with ellipsis", "Roboto", 12, 200, FontStyle::Regular);
        auto truncLabel = ui.createLabel(truncatedText, 420, yPos + 20);
        truncLabel->setFont("Roboto", 12);
        
        // Text size measurement example
        auto sizeExample = TextUtils::getTextSizeAdvanced("Measured Text", "Console", 14, FontStyle::Bold);
        auto sizeLabel = ui.createLabel(
            "\"Measured Text\" (Console 14px Bold) = " + 
            std::to_string(sizeExample.first) + "x" + std::to_string(sizeExample.second),
            420, yPos + 50
        );
        sizeLabel->setFont("Console", 10);
        
        // Instructions
        auto instructions = ui.createLabel("Phase 3 demonstrates font metrics access, precise text positioning,", 20, 600);
        instructions->setFont("Roboto", 11);
        
        auto instructions2 = ui.createLabel("baseline alignment, and advanced text utilities with font-aware operations.", 20, 620);
        instructions2->setFont("Roboto", 11);
        
        auto instructions3 = ui.createLabel("All text measurements and positioning use the enhanced FontManager API.", 20, 640);
        instructions3->setFont("Roboto", 11, FontStyle::Italic);
        
        std::cout << "Phase 3 Font Metrics & Layout Demo" << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << "Features demonstrated:" << std::endl;
        std::cout << "• Font metrics access (ascent, descent, height, lineSkip)" << std::endl;
        std::cout << "• Precise text measurement with FontManager API" << std::endl;
        std::cout << "• Text alignment including baseline alignment" << std::endl;
        std::cout << "• Advanced text utilities (wrapping, truncation, sizing)" << std::endl;
        std::cout << "• Font-aware text operations" << std::endl;
        std::cout << std::endl;
        
        // Display font metrics in console
        std::cout << "Font Metrics:" << std::endl;
        std::cout << "Roboto 12px - Ascent: " << robotoMetrics.ascent 
                  << ", Descent: " << robotoMetrics.descent 
                  << ", Height: " << robotoMetrics.height 
                  << ", LineSkip: " << robotoMetrics.lineSkip << std::endl;
        std::cout << "Console 12px - Ascent: " << consoleMetrics.ascent 
                  << ", Descent: " << consoleMetrics.descent 
                  << ", Height: " << consoleMetrics.height 
                  << ", LineSkip: " << consoleMetrics.lineSkip << std::endl;
        std::cout << std::endl;
        
        std::cout << "Text Measurements:" << std::endl;
        std::cout << "\"" << sampleText << "\" (Roboto 14px) = " 
                  << textSize.first << "x" << textSize.second << " pixels" << std::endl;
        std::cout << "\"Measured Text\" (Console 14px Bold) = " 
                  << sizeExample.first << "x" << sizeExample.second << " pixels" << std::endl;
        std::cout << std::endl;
        
        std::cout << "Wrapped text lines: " << wrappedLines.size() << std::endl;
        for (size_t i = 0; i < wrappedLines.size(); ++i) {
            std::cout << "  Line " << (i+1) << ": \"" << wrappedLines[i] << "\"" << std::endl;
        }
        std::cout << std::endl;
        
        std::cout << "Truncated text: \"" << truncatedText << "\"" << std::endl;
        std::cout << std::endl;
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "UI Error: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
