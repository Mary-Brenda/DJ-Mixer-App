/*
  ==============================================================================

    MiddleGUI.cpp
    Created: 25 Feb 2021 9:47:31am
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MiddleGUI.h"

//==============================================================================
MiddleGUI::MiddleGUI(DJAudioPlayer* _player, 
                     juce::TooltipWindow* _tooltipWindow)
                    : player(_player),
                      tooltipWindow(_tooltipWindow),
                      cuePosition1(-1.0),
                      cuePosition2(-1.0),
                      cuePosition3(-1.0),
                      cuePosition4(-1.0),
                      experienceLevel(0)
{
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(loopSlider);

    addAndMakeVisible(cueButton1);
    addAndMakeVisible(cueButton2);
    addAndMakeVisible(cueButton3);
    addAndMakeVisible(cueButton4);

    volSlider.setRange(0, 100);
    speedSlider.setRange(0.0, 2.0);
    loopSlider.setRange(0.0, 16.0, 1.0);

    volSlider.setValue(50);
    speedSlider.setValue(1.0);

    volSlider.setTextValueSuffix("%");
    speedSlider.setTextValueSuffix(" x");
    loopSlider.setTextValueSuffix(" s");

    volSlider.addListener(this);
    speedSlider.addListener(this);
    loopSlider.addListener(this);
    cueButton1.addListener(this);
    cueButton2.addListener(this);
    cueButton3.addListener(this);
    cueButton4.addListener(this);

    if (experienceLevel <= 2)
    {
        loopSlider.setTooltip("Select how many seconds from the \ncurrent position backwards to play repeatedly.");
        cueButton1.setTooltip("Click to save the current position for easy callback.\n CTRL + click to cancel previously saved position.");
    }
}

MiddleGUI::~MiddleGUI()
{
}

void MiddleGUI::paint (juce::Graphics& g)
{
    double rowH = getHeight() / 4;
    double rowW = getWidth() / 4;

    // Fill background.
    g.fillAll(juce::Colour::fromRGBA(55, 56, 60, 255));

    // Style buttons.
    g.setColour(juce::Colours::yellowgreen);
    g.drawRoundedRectangle(15, (rowH * 3 - (rowH * 0.5)) + ((rowH / 2) - 5.5) / 2, 
                           getWidth() - 30, ((rowH / 2) - 4.5) / 2, 5, 1);
    g.drawRoundedRectangle(15, (rowH * 3.5 + 10) - ((getWidth() - 30) / 2) + 5, 
                           ((getWidth() - 30) / 2), ((getWidth() - 30) / 2) - 5, 5, 1);
    g.drawRoundedRectangle(15 + ((getWidth() - 30) / 2), (rowH * 3.5 + 10) - ((getWidth() - 30) / 2) + 5, 
                           ((getWidth() - 30) / 2), ((getWidth() - 30) / 2) - 5, 5, 1);
    g.drawRoundedRectangle(15, (rowH * 3.5 + 10), ((getWidth() - 30) / 2), 
                           ((getWidth() - 30) / 2) - 5, 5, 1);
    g.drawRoundedRectangle(15 + ((getWidth() - 30) / 2), (rowH * 3.5 + 10), 
                           ((getWidth() - 30) / 2), ((getWidth() - 30) / 2) - 5, 5, 1);

    g.setColour (Colours::white);
    g.setFont (16.0f);

    // Loop Label
    juce::Rectangle<float> loopArea(15, rowH * 2.12, getWidth() - 30, (rowH / 2) - 0.5);
    g.drawText("LOOP", loopArea, juce::Justification::centred, false);

    // Hot Cues Label
    juce::Rectangle<float> cuesArea(0, rowH * 2.9, getWidth(), rowH / 2);
    g.drawText("HOT CUES", cuesArea, juce::Justification::centred, false);

    g.setFont(13.0f);

    // Volume Label
    juce::Rectangle<float> volumeArea(4, -14, getWidth() / 2 - 2, rowH / 2 + 6);
    g.drawText("VOLUME", volumeArea, juce::Justification::centred, false);

    // Speed Label
    juce::Rectangle<float> speedArea(getWidth() / 2 - 2, -14, getWidth() / 2, rowH / 2 + 6);
    g.drawText("SPEED", speedArea, juce::Justification::centred, false);

    if (player->fileJustLoaded())
    {
        // Reset all controls.
        cuePosition1 = -1.0;
        cueButton1.setToggleState(false, juce::NotificationType::dontSendNotification);
        cuePosition2 = -1.0;
        cueButton2.setToggleState(false, juce::NotificationType::dontSendNotification);
        cuePosition3 = -1.0;
        cueButton3.setToggleState(false, juce::NotificationType::dontSendNotification);
        cuePosition4 = -1.0;
        cueButton4.setToggleState(false, juce::NotificationType::dontSendNotification);

        volSlider.setValue(50);
        speedSlider.setValue(1.0);
        loopSlider.setValue(0);

        experienceLevel++;

        if (experienceLevel >= 3)
        {
            // Remove tooltips.
            loopSlider.setTooltip("");
            cueButton1.setTooltip("");

            // Reset loop tooltip to appear after 1 hour 
            // (bug fix for JUCE inc/dec slider tooltip issue).
            tooltipWindow->setMillisecondsBeforeTipAppears(3666666);
        }
    }

    repaint();
}

void MiddleGUI::resized()
{
    double rowH = getHeight() / 4;
    double rowW = getWidth() / 4;
    double buttonW = getWidth() / 10;

    // Volume Slider
    volSlider.setNumDecimalPlacesToDisplay(0);
    volSlider.setBounds(rowW - 13, getHeight() / 15, 35, rowH * 2);
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 510, 20);
    volSlider.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    volSlider.setColour(juce::Slider::ColourIds::trackColourId, 
                        juce::Colours::yellowgreen);
    volSlider.setColour(juce::Slider::ColourIds::thumbColourId, 
                        juce::Colour::fromRGBA(102, 94, 199, 255));
    volSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId,
                        juce::Colour::fromRGBA(102, 94, 199, 255));

    // Speed Slider
    speedSlider.setNumDecimalPlacesToDisplay(1);
    speedSlider.setBounds((rowW * 2) + 7, getHeight() / 15, 35, rowH * 2);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 510, 20);
    speedSlider.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    speedSlider.setColour(juce::Slider::ColourIds::trackColourId,
                          juce::Colours::yellowgreen);
    speedSlider.setColour(juce::Slider::ColourIds::thumbColourId,
                          juce::Colour::fromRGBA(102, 94, 199, 255));
    speedSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId,
                          juce::Colour::fromRGBA(102, 94, 199, 255));

    // Loop Slider
    loopSlider.setSliderStyle(juce::Slider::IncDecButtons);
    loopSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    loopSlider.setBounds(15, rowH * 3 - (rowH * 0.5), getWidth() - 30, (rowH / 2) - 0.5);
    loopSlider.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, 
                         juce::Colour::fromRGBA(102, 94, 199, 255));

    // Cue Buttons
    cueButton1.setBounds(15, (rowH * 3.5 + 10) - ((getWidth() - 30) / 2) + 5, 
                         ((getWidth() - 30) / 2), ((getWidth() - 30) / 2) - 5);
    cueButton1.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::yellowgreen);
    cueButton1.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    cueButton1.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    cueButton2.setBounds(15 + ((getWidth() - 30) / 2), (rowH * 3.5 + 10) - ((getWidth() - 30) / 2) + 5, 
                         ((getWidth() - 30) / 2), ((getWidth() - 30) / 2) - 5);
    cueButton2.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::yellowgreen);
    cueButton2.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    cueButton2.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    cueButton3.setBounds(15, (rowH * 3.5 + 10), ((getWidth() - 30) / 2), ((getWidth() - 30) / 2) - 5);
    cueButton3.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::yellowgreen);
    cueButton3.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    cueButton3.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    cueButton4.setBounds(15 + ((getWidth() - 30) / 2), (rowH * 3.5 + 10), 
                         ((getWidth() - 30) / 2), ((getWidth() - 30) / 2) - 5);
    cueButton4.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::yellowgreen);
    cueButton4.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    cueButton4.setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void MiddleGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &loopSlider)
    {
        player->setLoop(slider->getValue());
    }
}

void MiddleGUI::buttonClicked(juce::Button* button)
{
    if (button == &cueButton1)
    {
        if (!isCommandDown())
        {
            if (cuePosition1 == -1.0)
            {
                // Set cue.
                cuePosition1 = player->getPosInTrack();
                cueButton1.setToggleState(true, juce::NotificationType::dontSendNotification);
            }
            else
            {
                // Recall cue.
                player->setPosition(cuePosition1);
            }
        }
        else
        {
            // If CTRL is held down as button is clicked, reset cue button.
            cuePosition1 = -1.0;
            cueButton1.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
    }

    if (button == &cueButton2)
    {
        if (!isCommandDown())
        {
            if (cuePosition2 == -1.0)
            {
                // Set cue.
                cuePosition2 = player->getPosInTrack();
                cueButton2.setToggleState(true, juce::NotificationType::dontSendNotification);
            }
            else
            {
                // Recall cue.
                player->setPosition(cuePosition2);
            }
        }
        else
        {
            // If CTRL is held down as button is clicked, reset cue button.
            cuePosition2 = -1.0;
            cueButton2.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
    }

    if (button == &cueButton3)
    {
        if (!isCommandDown())
        {
            if (cuePosition3 == -1.0)
            {
                // Set cue.
                cuePosition3 = player->getPosInTrack();
                cueButton3.setToggleState(true, juce::NotificationType::dontSendNotification);
            }
            else
            {
                // Recall cue.
                player->setPosition(cuePosition3);
            }
        }
        else
        {
            // If CTRL is held down as button is clicked, reset cue button.
            cuePosition3 = -1.0;
            cueButton3.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
    }

    if (button == &cueButton4)
    {
        if (!isCommandDown())
        {
            if (cuePosition4 == -1.0)
            {
                // Set cue.
                cuePosition4 = player->getPosInTrack();
                cueButton4.setToggleState(true, juce::NotificationType::dontSendNotification);
            }
            else
            {
                // Recall cue.
                player->setPosition(cuePosition4);
            }
        }
        else
        {
            // If CTRL is held down as button is clicked, reset cue button.
            cuePosition4 = -1.0;
            cueButton4.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
    }
}

bool MiddleGUI::isCommandDown() const noexcept
{
    // Check if CTRL key is held down if on Windows or Command key if on Mac.
    if (currentModifiers == juce::ModifierKeys::Flags::commandModifier)
    {
        return true;
    }

    return false;
}