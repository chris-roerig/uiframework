// Generated from dev/examples/demo.ui
// DO NOT EDIT - This file is auto-generated

#pragma once
#include "uiframework/UI.h"
#include <memory>

namespace ui_generated {

struct DemoWidgets {
    std::shared_ptr<ui::Label> title;
    std::shared_ptr<ui::Label> version;
    std::shared_ptr<ui::Label> oscLabel;
    std::shared_ptr<ui::Button> sineWave;
    std::shared_ptr<ui::Button> sawWave;
    std::shared_ptr<ui::Button> squareWave;
    std::shared_ptr<ui::Label> filterLabel;
    std::shared_ptr<ui::Label> cutoffLabel;
    std::shared_ptr<ui::TextBox> cutoffValue;
    std::shared_ptr<ui::Label> resLabel;
    std::shared_ptr<ui::TextBox> resValue;
    std::shared_ptr<ui::Label> envLabel;
    std::shared_ptr<ui::Label> attackLabel;
    std::shared_ptr<ui::TextBox> attackValue;
    std::shared_ptr<ui::Label> decayLabel;
    std::shared_ptr<ui::TextBox> decayValue;
    std::shared_ptr<ui::Label> sustainLabel;
    std::shared_ptr<ui::TextBox> sustainValue;
    std::shared_ptr<ui::Label> releaseLabel;
    std::shared_ptr<ui::TextBox> releaseValue;
    std::shared_ptr<ui::Label> sampleLabel;
    std::shared_ptr<ui::Button> loadSample;
    std::shared_ptr<ui::Label> sampleName;
    std::shared_ptr<ui::CheckBox> loopEnable;
    std::shared_ptr<ui::Label> loopLabel;
    std::shared_ptr<ui::Label> fxLabel;
    std::shared_ptr<ui::CheckBox> reverbEnable;
    std::shared_ptr<ui::Label> reverbLabel;
    std::shared_ptr<ui::CheckBox> delayEnable;
    std::shared_ptr<ui::Label> delayLabel;
    std::shared_ptr<ui::CheckBox> chorusEnable;
    std::shared_ptr<ui::Label> chorusLabel;
    std::shared_ptr<ui::Label> masterLabel;
    std::shared_ptr<ui::Label> volumeLabel;
    std::shared_ptr<ui::TextBox> masterVolume;
    std::shared_ptr<ui::Label> panLabel;
    std::shared_ptr<ui::TextBox> masterPan;
    std::shared_ptr<ui::Button> playButton;
    std::shared_ptr<ui::Button> stopButton;
    std::shared_ptr<ui::Button> recordButton;
    std::shared_ptr<ui::Label> status;
    std::shared_ptr<ui::Label> midiStatus;
};

inline DemoWidgets createDemo(UI& ui) {
    DemoWidgets widgets;
    
    try {
        widgets.title = ui.createLabel("SynthSampler Pro", 20, 10);
        widgets.version = ui.createLabel("v1.0", 600, 10);
        widgets.oscLabel = ui.createLabel("OSCILLATOR", 20, 50);
        widgets.sineWave = ui.createButton("SINE", 20, 80, nullptr);
        widgets.sawWave = ui.createButton("SAW", 90, 80, nullptr);
        widgets.squareWave = ui.createButton("SQR", 160, 80, nullptr);
        widgets.filterLabel = ui.createLabel("FILTER", 250, 50);
        widgets.cutoffLabel = ui.createLabel("Cutoff:", 250, 80);
        widgets.cutoffValue = ui.createTextBox("1000", 310, 75);
        widgets.resLabel = ui.createLabel("Res:", 250, 115);
        widgets.resValue = ui.createTextBox("0.5", 310, 110);
        widgets.envLabel = ui.createLabel("ENVELOPE", 450, 50);
        widgets.attackLabel = ui.createLabel("A:", 450, 80);
        widgets.attackValue = ui.createTextBox("0.1", 480, 75);
        widgets.decayLabel = ui.createLabel("D:", 550, 80);
        widgets.decayValue = ui.createTextBox("0.2", 580, 75);
        widgets.sustainLabel = ui.createLabel("S:", 450, 115);
        widgets.sustainValue = ui.createTextBox("0.7", 480, 110);
        widgets.releaseLabel = ui.createLabel("R:", 550, 115);
        widgets.releaseValue = ui.createTextBox("0.5", 580, 110);
        widgets.sampleLabel = ui.createLabel("SAMPLER", 20, 170);
        widgets.loadSample = ui.createButton("LOAD SAMPLE", 20, 200, nullptr);
        widgets.sampleName = ui.createLabel("No sample loaded", 150, 205);
        widgets.loopEnable = ui.createCheckBox(false, 20, 240, nullptr);
        widgets.loopLabel = ui.createLabel("Loop", 50, 245);
        widgets.fxLabel = ui.createLabel("EFFECTS", 350, 170);
        widgets.reverbEnable = ui.createCheckBox(false, 350, 200, nullptr);
        widgets.reverbLabel = ui.createLabel("Reverb", 380, 205);
        widgets.delayEnable = ui.createCheckBox(false, 470, 200, nullptr);
        widgets.delayLabel = ui.createLabel("Delay", 500, 205);
        widgets.chorusEnable = ui.createCheckBox(false, 350, 240, nullptr);
        widgets.chorusLabel = ui.createLabel("Chorus", 380, 245);
        widgets.masterLabel = ui.createLabel("MASTER", 580, 170);
        widgets.volumeLabel = ui.createLabel("Volume:", 580, 200);
        widgets.masterVolume = ui.createTextBox("100", 580, 225);
        widgets.panLabel = ui.createLabel("Pan:", 580, 260);
        widgets.masterPan = ui.createTextBox("0", 580, 285);
        widgets.playButton = ui.createButton("PLAY", 20, 320, nullptr);
        widgets.stopButton = ui.createButton("STOP", 90, 320, nullptr);
        widgets.recordButton = ui.createButton("REC", 160, 320, nullptr);
        widgets.status = ui.createLabel("Ready", 20, 370);
        widgets.midiStatus = ui.createLabel("MIDI: Disconnected", 350, 370);
    } catch (const std::exception& e) {
        throw ui::UIException("Failed to create demo widgets: " + std::string(e.what()));
    }
    
    return widgets;
}

} // namespace ui_generated
