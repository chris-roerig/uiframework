#include "uiframework/Rendering/RenderContext.h"
#include "uiframework/ErrorHandling.h"

namespace ui {

bool RenderContext::isValid() const {
    return ErrorHandling::validateRenderParams(renderer, theme);
}

RenderContext RenderContext::create(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    return RenderContext{renderer, font, theme};
}

ThemeableElementColors RenderContext::buttonColors() const {
    return theme->buttonColors();
}

ThemeableElementColors RenderContext::checkboxColors() const {
    return theme->checkboxColors();
}

ThemeableElementColors RenderContext::textInputColors() const {
    return theme->textInputColors();
}

ThemeableElementColors RenderContext::optionSelectColors() const {
    return theme->optionSelectColors();
}

ThemeableElementColors RenderContext::labelColors() const {
    return theme->labelColors();
}

ThemeableElementColors RenderContext::progressBarColors() const {
    return theme->progressBarColors();
}

ThemeableElementColors RenderContext::sliderColors() const {
    return theme->sliderColors();
}

ThemeableElementColors RenderContext::listViewColors() const {
    return theme->listViewColors();
}

ThemeableElementColors RenderContext::modalColors() const {
    return theme->modalColors();
}

ThemeableElementColors RenderContext::contextMenuColors() const {
    return theme->contextMenuColors();
}

ThemeableElementColors RenderContext::tabbedPanelColors() const {
    return theme->tabbedPanelColors();
}

ThemeableElementColors RenderContext::cycleListColors() const {
    return theme->cycleListColors();
}

} // namespace ui
