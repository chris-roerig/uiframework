#pragma once
#include "Color.h"

namespace ui {

struct ThemeableElementColors {
    Color background;
    Color foreground;
    Color highlight;
    Color activeText;
    Color inactiveText;
    Color buttonText;
    Color buttonBackground;
    Color buttonForeground;
    Color buttonBorderLight;
    Color buttonBorderDark;
    Color checkboxEnabled;
    Color checkboxDisabled;
    Color checkboxChecked;
    Color checkboxUnchecked;
    Color checkboxBorderLight;
    Color checkboxBorderDark;
    Color textInputBackground;
    Color textInputText;
    Color textInputBorderLight;
    Color textInputBorderDark;
    Color selectOptionUnselected;
    Color selectOptionSelected;
    Color selectOptionBorderLight;
    Color selectOptionBorderDark;
    Color selectOptionTextSelected;
    Color selectOptionTextUnselected;
    Color labelText;
    Color labelBackground;
    Color contextMenuBackground;
    Color contextMenuHighlight;
    Color contextMenuText;
    Color contextMenuBorder;
    Color contextMenuActiveItem;
    Color modalOverlay;          // The semi-transparent overlay color.
    Color modalBackground;       // Background color for modal dialog.
    Color modalText;             // Color for the modal's main text.
    Color modalBorder;           // Border color for the modal dialog.
    Color modalButtonBackground; // Background for modal buttons.
    Color modalButtonText;       // Text color for modal buttons.
    Color modalButtonHighlight;  // Highlight color for modal buttons when focused.
    Color modalButtonBorder; 
    Color listViewBackground;
    Color listViewText;
    Color listViewBorder;
    Color gridBorder;            // Border color for grid lines
    Color listViewSelectedItem;
    Color listViewScrollbarBackground;
    Color listViewScrollbarThumb;
    Color progressBarBackground;
    Color progressBarBorder;
    Color progressBarForeground;
    Color progressBarText;
    Color focusBorder;
    Color focusBackground;
    Color sliderTrack;
    Color sliderThumb;
    Color sliderBorder;
    Color canvasBackground;
    Color canvasGrid;
    Color canvasBorder;
};

class Theme {
public:
    virtual ~Theme() = default;
    virtual ThemeableElementColors labelColors() const = 0;
    virtual ThemeableElementColors buttonColors() const = 0;
    virtual ThemeableElementColors checkboxColors() const = 0;
    virtual ThemeableElementColors optionSelectColors() const = 0;
    virtual ThemeableElementColors textInputColors() const = 0;
    virtual ThemeableElementColors contextMenuColors() const = 0;
    virtual ThemeableElementColors modalColors() const = 0;
    virtual ThemeableElementColors listViewColors() const = 0;
    virtual ThemeableElementColors progressBarColors() const = 0;
    virtual ThemeableElementColors focusColors() const = 0;
    virtual ThemeableElementColors sliderColors() const = 0;
    virtual ThemeableElementColors canvasColors() const = 0;
    virtual ThemeableElementColors gridColors() const = 0;
    virtual Color highlightColor() const = 0;
    virtual Color backgroundColor() const = 0;
    virtual Color foregroundColor() const = 0;
};

class ThemeBase : public Theme {
protected:
    Color defaultBackground;
    Color defaultForeground;
    Color defaultHighlight;
    Color defaultActiveText;
    Color defaultInactiveText;
    Color defaultButtonText;
    Color defaultButtonBackground;
    Color defaultButtonForeground;
    Color defaultButtonBorderLight;
    Color defaultButtonBorderDark;
    Color defaultCheckboxBorderLight;
    Color defaultCheckboxBorderDark;
    Color defaultCheckboxEnabled;
    Color defaultCheckboxDisabled;
    Color defaultCheckboxChecked;
    Color defaultCheckboxUnchecked;
    Color defaultSelectOptionUnselected;
    Color defaultSelectOptionSelected;
    Color defaultSelectOptionBorderLight;
    Color defaultSelectOptionBorderDark;
    Color defaultSelectOptionTextSelected;
    Color defaultSelectOptionTextUnselected;
    Color defaultTextInputBackground;
    Color defaultTextInputText;
    Color defaultTextInputBorderLight;
    Color defaultTextInputBorderDark;
    Color defaultLabelText;
    Color defaultLabelBackground;
    Color defaultContextMenuBackground;
    Color defaultContextMenuHighlight;
    Color defaultContextMenuText;
    Color defaultContextMenuBorder;
    Color defaultContextMenuActiveItem;
    Color defaultModalOverlay;
    Color defaultModalBackground;
    Color defaultModalText;
    Color defaultModalBorder;
    Color defaultModalButtonBackground;
    Color defaultModalButtonText;
    Color defaultModalButtonHighlight;
    Color defaultModalButtonBorder;
    Color defaultListViewBackground;
    Color defaultListViewText;
    Color defaultListViewBorder;
    Color defaultGridBorder;
    Color defaultListViewSelectedItem;
    Color defaultListViewScrollbarBackground;
    Color defaultListViewScrollbarThumb;
    Color defaultProgressBarBackground;
    Color defaultProgressBarBorder;
    Color defaultProgressBarForeground;
    Color defaultProgressBarText;
    Color defaultFocusBorder;
    Color defaultFocusBackground;
    Color defaultSliderTrack;
    Color defaultSliderThumb;
    Color defaultSliderBorder;
    Color defaultCanvasBackground;
    Color defaultCanvasGrid;
    Color defaultCanvasBorder;

public:
    ThemeBase()
      : defaultBackground(0,0,0),
        defaultForeground(255,255,255),
        defaultHighlight(255,255,0),
        defaultActiveText(255,255,255),
        defaultInactiveText(128,128,128),
        defaultButtonText(255,255,255),
        defaultButtonBackground(150,150,150),
        defaultButtonForeground(255,255,255),
        defaultButtonBorderLight(200,200,200),
        defaultButtonBorderDark(100,100,100),
        defaultCheckboxBorderLight(200,200,200),
        defaultCheckboxBorderDark(100,100,100),
        defaultCheckboxEnabled(255,255,255),
        defaultCheckboxDisabled(200,200,200),
        defaultCheckboxChecked(0,255,0),
        defaultCheckboxUnchecked(0,0,0),
        defaultSelectOptionUnselected(200,200,200),
        defaultSelectOptionSelected(100,149,237),
        defaultSelectOptionBorderLight(200,200,200),
        defaultSelectOptionBorderDark(100,100,100),
        defaultSelectOptionTextSelected(255,255,255),
        defaultSelectOptionTextUnselected(0,0,0),
        defaultTextInputBackground(255,255,255),
        defaultTextInputText(0,0,0),
        defaultTextInputBorderLight(200,200,200),
        defaultTextInputBorderDark(100,100,100),
        defaultLabelText(255,255,255),
        defaultLabelBackground(0,0,0),
        defaultContextMenuBackground(240,240,240),
        defaultContextMenuHighlight(100,149,237),
        defaultContextMenuText(0,0,0),
        defaultContextMenuBorder(128,128,128),
        defaultContextMenuActiveItem(100,149,237),
        defaultModalOverlay(0,0,0,128),
        defaultModalBackground(240,240,240),
        defaultModalText(0,0,0),
        defaultModalBorder(128,128,128),
        defaultModalButtonBackground(200,200,200),
        defaultModalButtonText(0,0,0),
        defaultModalButtonHighlight(100,149,237),
        defaultModalButtonBorder(128,128,128),
        defaultListViewBackground(255,255,255),
        defaultListViewText(0,0,0),
        defaultListViewBorder(128,128,128),
        defaultListViewSelectedItem(100,149,237),
        defaultListViewScrollbarBackground(240,240,240),
        defaultListViewScrollbarThumb(128,128,128),
        defaultProgressBarBackground(240,240,240),
        defaultProgressBarBorder(128,128,128),
        defaultProgressBarForeground(0,255,0),
        defaultProgressBarText(0,0,0),
        defaultFocusBorder(255,255,0),
        defaultFocusBackground(255,255,255,50),
        defaultSliderTrack(200,200,200),
        defaultSliderThumb(100,100,100),
        defaultSliderBorder(128,128,128),
        defaultCanvasBackground(255,255,255),
        defaultCanvasGrid(240,240,240),
        defaultCanvasBorder(200,200,200)
    {}

    ThemeableElementColors labelColors() const override {
        ThemeableElementColors c;
        c.labelText = defaultLabelText;
        c.labelBackground = defaultLabelBackground;
        return c;
    }
    
    ThemeableElementColors buttonColors() const override {
        ThemeableElementColors c;
        c.buttonText = defaultButtonText;
        c.buttonBackground = defaultButtonBackground;
        c.buttonForeground = defaultButtonForeground;
        c.buttonBorderLight = defaultButtonBorderLight;
        c.buttonBorderDark  = defaultButtonBorderDark;
        return c;
    }
    
    ThemeableElementColors checkboxColors() const override {
        ThemeableElementColors c;
        c.checkboxEnabled = defaultCheckboxEnabled;
        c.checkboxDisabled = defaultCheckboxDisabled;
        c.checkboxChecked = defaultCheckboxChecked;
        c.checkboxUnchecked = defaultCheckboxUnchecked;
        c.checkboxBorderLight = defaultCheckboxBorderLight;
        c.checkboxBorderDark  = defaultCheckboxBorderDark;
        return c;
    }
    
    ThemeableElementColors optionSelectColors() const override {
        ThemeableElementColors c;
        c.selectOptionSelected = defaultSelectOptionSelected;
        c.selectOptionUnselected = defaultSelectOptionUnselected;
        c.selectOptionBorderLight = defaultSelectOptionBorderLight;
        c.selectOptionBorderDark = defaultSelectOptionBorderDark;
        c.selectOptionTextSelected = defaultSelectOptionTextSelected;
        c.selectOptionTextUnselected = defaultSelectOptionTextUnselected;
        return c;
    }
    
    ThemeableElementColors textInputColors() const override {
        ThemeableElementColors c;
        c.textInputBackground = defaultTextInputBackground;
        c.textInputText = defaultTextInputText;
        c.textInputBorderLight = defaultTextInputBorderLight;
        c.textInputBorderDark  = defaultTextInputBorderDark;
        return c;
    }
    
    ThemeableElementColors contextMenuColors() const override {
        ThemeableElementColors c;
        c.contextMenuBackground = defaultContextMenuBackground;
        c.contextMenuText = defaultContextMenuText;
        c.contextMenuHighlight = defaultContextMenuHighlight;
        c.contextMenuBorder = defaultContextMenuBorder;
        c.contextMenuActiveItem = defaultContextMenuActiveItem;
        return c;
    }
    
    ThemeableElementColors modalColors() const override {
        ThemeableElementColors c;
        c.modalOverlay = defaultModalOverlay;
        c.modalBackground = defaultModalBackground;
        c.modalText = defaultModalText;
        c.modalBorder = defaultModalBorder;
        c.modalButtonBackground = defaultModalButtonBackground;
        c.modalButtonText = defaultModalButtonText;
        c.modalButtonHighlight = defaultModalButtonHighlight;
        c.modalButtonBorder = defaultModalButtonBorder;
        return c;
    }
    
    ThemeableElementColors listViewColors() const override {
        ThemeableElementColors c;
        c.listViewBackground = defaultListViewBackground;
        c.listViewText = defaultListViewText;
        c.listViewBorder = defaultListViewBorder;
        c.gridBorder = defaultGridBorder;
        c.listViewSelectedItem = defaultListViewSelectedItem;
        c.listViewScrollbarBackground = defaultListViewScrollbarBackground;
        c.listViewScrollbarThumb = defaultListViewScrollbarThumb;
        return c;
    }
    
    ThemeableElementColors progressBarColors() const override {
        ThemeableElementColors c;
        c.progressBarBackground = defaultProgressBarBackground;
        c.progressBarBorder = defaultProgressBarBorder;
        c.progressBarForeground = defaultProgressBarForeground;
        c.progressBarText = defaultProgressBarText;
        return c;
    }
    
    ThemeableElementColors focusColors() const override {
        ThemeableElementColors c;
        c.focusBorder = defaultFocusBorder;
        c.focusBackground = defaultFocusBackground;
        return c;
    }
    
    ThemeableElementColors sliderColors() const override {
        ThemeableElementColors c;
        c.sliderTrack = defaultSliderTrack;
        c.sliderThumb = defaultSliderThumb;
        c.sliderBorder = defaultSliderBorder;
        return c;
    }
    
    ThemeableElementColors canvasColors() const override {
        ThemeableElementColors c;
        c.canvasBackground = defaultCanvasBackground;
        c.canvasGrid = defaultCanvasGrid;
        c.canvasBorder = defaultCanvasBorder;
        return c;
    }
    
    ThemeableElementColors gridColors() const override {
        ThemeableElementColors c;
        c.gridBorder = defaultGridBorder;
        return c;
    }
    
    Color highlightColor() const override { return defaultHighlight; }
    Color backgroundColor() const override { return defaultBackground; }
    Color foregroundColor() const override { return defaultForeground; }
};

} // namespace ui
