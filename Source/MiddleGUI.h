/*
  ==============================================================================

    MiddleGUI.h
    Created: 25 Feb 2021 9:47:31am
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class MiddleGUI  : public juce::Component,
                   public juce::Slider::Listener,
                   public juce::Button::Listener,
                   public juce::ModifierKeys
{
    public:
        /**
        * PURPOSE: Creates the MiddleGUI object, initialises its data members
        *          (including adding listeners and setting tooltips).
        * INPUTS: Pointers to DJAudioPlayer and juce TooltipWindow.
        * OUTPUTS: None.
        */
        MiddleGUI(DJAudioPlayer* _player, juce::TooltipWindow* _tooltipWindow);

        /**
        * PURPOSE: Destroys the MiddleGUI object.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        ~MiddleGUI() override;

        /**
        * PURPOSE: Draws the component's contents. Overrides juce Component member function.
        * INPUTS: The graphics context that must be used to do the drawing operations.
        * OUTPUTS: None.
        */
        void paint (juce::Graphics& g) override;
        
        /**
        * PURPOSE: Layouts the component's children when its width or height changes.
        *          Overrides juce Component member function.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        void resized() override;

        /**
        * PURPOSE: Implements juce Slider::Listener.
        * INPUTS: A pointer to the juce slider that had its value changed.
        * OUTPUTS: None.
        */
        void sliderValueChanged(juce::Slider* slider) override;

        /**
        * PURPOSE: Implements juce Button::Listener.
        * INPUTS: A pointer to the juce button that was clicked.
        * OUTPUTS: None.
        */
        void buttonClicked(juce::Button* button) override;

        /**
        * PURPOSE: Checks whether the 'command' key flag is set (or 'CTRL' on Windows/Linux).
        *          In other words, checks if user is holding down the command/CTRL key.
        *          Overrides juce ModifierKeys member function.
        * INPUTS: None.
        * OUTPUTS: A boolean; more specifically true if command key is down and false if it isn't.
        */
        bool isCommandDown() const noexcept;

    private:
        juce::Slider volSlider;
        juce::Slider speedSlider;
        juce::Slider loopSlider;

        juce::TextButton cueButton1{ "1" };
        juce::TextButton cueButton2{ "2" };
        juce::TextButton cueButton3{ "3" };
        juce::TextButton cueButton4{ "4" };
    
        double cuePosition1;
        double cuePosition2;
        double cuePosition3;
        double cuePosition4;
        int experienceLevel;

        DJAudioPlayer* player;
        juce::TooltipWindow* tooltipWindow;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiddleGUI)
};