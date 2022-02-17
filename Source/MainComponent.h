/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "MiddleGUI.h"
#include "PlaylistComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where we should put all
    our controls and content.
*/
class MainComponent   : public juce::AudioAppComponent
{
    public:
        //==============================================================================
        /**
        * PURPOSE: Creates the MainComponent object, initialises its data members
        *          and registers the basic music file formats.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        MainComponent();
        
        /**
        * PURPOSE: Destroys the MainComponent object, including shutting down
        *          the audio device and clearing the audio source.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        ~MainComponent();

        //==============================================================================
        /**
        * PURPOSE: Tells the source to prepare for playing.
        *          Implements juce AudioAppComponent (i.e. function is pure virtual).
        * INPUTS: The number of samples that the source will be expected to supply each time 
        *         its getNextAudioBlock() method is called and the sample rate that the output 
        *         will be used at - this is needed by sources such as tone generators.
        * OUTPUTS: None.
        */
        void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
        
        /**
        * PURPOSE: Called repeatedly to fetch subsequent blocks of audio data.
        *          Implements juce AudioAppComponent (i.e. function is pure virtual).
        * INPUTS: A reference to the buffer to be filled.
        * OUTPUTS: None.
        */
        void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;

        /**
        * PURPOSE: Allows the source to release anything it no longer needs after playback has stopped.
        *          Implements juce AudioAppComponent (i.e. function is pure virtual).
        * INPUTS: None.
        * OUTPUTS: None.
        */
        void releaseResources() override;

        //==============================================================================
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

    private:
        //==============================================================================

        juce::AudioFormatManager formatManager;
        juce::AudioThumbnailCache thumbCache{100}; 

        DJAudioPlayer player1{formatManager};
        juce::Colour blueDeckColour{ juce::Colour::fromRGBA(37, 136, 238, 255) };
        DeckGUI deckGUI1{&player1, formatManager, thumbCache, blueDeckColour, &tooltipWindow };
        MiddleGUI middleGUI1{ &player1, &tooltipWindow };

        DJAudioPlayer player2{formatManager};
        juce::Colour redDeckColour{ juce::Colour::fromRGBA(146, 14, 27, 255) };
        DeckGUI deckGUI2{&player2, formatManager, thumbCache, redDeckColour, &tooltipWindow };
        MiddleGUI middleGUI2{ &player2, &tooltipWindow };

        juce::MixerAudioSource mixerSource; 

        PlaylistComponent playlistComponent{ formatManager, &deckGUI1, &deckGUI2 };
        juce::TooltipWindow tooltipWindow{ this, 700 };
    
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};