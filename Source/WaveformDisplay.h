/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Jan 2021 3:50:16pm
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class WaveformDisplay    : public juce::Component,
                           public juce::ChangeListener
{
    public:
        /**
        * PURPOSE: Creates the WaveformDisplay object, initialises its data members
        *          and adds the change listener to the audio thumbnail.
        * INPUTS: References to juce AudioFormatManager, juce AudioThumbnailCache, 
        *         and an RGBA colour for styling.
        * OUTPUTS: None.
        */
        WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                        juce::AudioThumbnailCache& cacheToUse,
                        juce::Colour& colourToUse);

        /**
        * PURPOSE: Destroys the WaveformDisplay object.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        ~WaveformDisplay();

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
        * PURPOSE: Receives the change listener callback.
        *          Implements juce ChangeListener (i.e. function is pure virtual).
        * INPUTS: A pointer to source (i.e. the ChangeBroadcaster that triggered the callback).
        * OUTPUTS: None.
        */
        void changeListenerCallback (juce::ChangeBroadcaster *source) override;

        /**
        * PURPOSE: Load the track file to be painted.
        * INPUTS: The track's name as a juce string, length as a juce string and file path as a juce URL.
        * OUTPUTS: None.
        */
        void loadFile(juce::String fileName, juce::String fileLength, juce::URL audioURL);

        /**
        * PURPOSE: Sets the position of the playhead relative to the track's length and between 0 and 1.
        * INPUTS: The position value to be set.
        * OUTPUTS: None.
        */
        void setPositionRelative(double pos);

        /**
        * PURPOSE: Calculates the remaining duration of the track that is currently playing.
        * INPUTS: The current position of the playhead.
        * OUTPUTS: None.
        */
        void updateTrackDuration(double currentPos);

    private:
        /**
        * PURPOSE: Calculates the song length from the provided
        *          length in seconds and converts it to a string.
        * INPUTS: The song length in seconds.
        * OUTPUTS: The length of the song converted to juce string format.
        */
        static juce::String getSongLength(double lengthInSecs);


        /** DATA MEMBERS */

        juce::AudioThumbnail audioThumb;
        juce::AudioFormatManager& formatManager;

        bool fileLoaded;
        double position;
        double posInSecs;
        
        juce::String trackName;
        juce::String trackLength;
        juce::Colour accentColour;
    
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};