/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Jan 2021 4:22:22pm
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioAppComponent, 
                      public juce::Timer 
{
    public:
        /**
        * PURPOSE: Creates the DJAudioPlayer object, initialises its data members, and starts a timer.
        * INPUTS: A reference to the juce AudioFormatManager.
        * OUTPUTS: None.
        */
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);

        /**
        * PURPOSE: Destroys the DJAudioPlayer object and stops the timer.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        ~DJAudioPlayer();

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

        /**
        * PURPOSE: Reads and loads the track to be played.
        * INPUTS: The audio URL of the track.
        * OUTPUTS: None.
        */
        void loadURL(juce::URL audioURL);

        /**
        * PURPOSE: Sets the track's volume.
        * INPUTS: The volume slider value to be set.
        * OUTPUTS: None.
        */
        void setGain(double gain);

        /**
        * PURPOSE: Sets the track's speed.
        * INPUTS: The speed slider ratio to be set.
        * OUTPUTS: None.
        */
        void setSpeed(double ratio);

        /**
        * PURPOSE: Sets the track's current position.
        * INPUTS: The current position of the track in seconds to be set.
        * OUTPUTS: None.
        */
        void setPosition(double posInSecs);

        /**
        * PURPOSE: Sets a position relative to the track's length and between 0 and 1.
        * INPUTS: The position value to be set.
        * OUTPUTS: None.
        */
        void setPositionRelative(double pos);

        /**
        * PURPOSE: Sets a section of the track to be played repeatedly.
        * INPUTS: The last X number of seconds to start the replay 
        *         from on reaching the current position.
        * OUTPUTS: None.
        */
        void setLoop(double seconds);
    
        /**
        * PURPOSE: The user-defined callback routine that actually gets called periodically.
        *          Implements juce Timer.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        void timerCallback() override;

        /**
        * PURPOSE: Plays the track.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        void start();
        
        /**
        * PURPOSE: Pauses the track.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        void pause();

        /**
        * PURPOSE: Stops the track and returns to the beginning of the track.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        void stop();

        /**
        * PURPOSE: Gets the current position in the track.
        * INPUTS: None.
        * OUTPUTS: The current position in the track.
        */
        double getPosInTrack();

        /**
        * PURPOSE: Gets the position of the playhead relative to the track's length and between 0 and 1.
        * INPUTS: None.
        * OUTPUTS: The relative position of the playhead.
        */
        double getPositionRelative();

        /**
        * PURPOSE: Checks if a file has just been loaded and resets the fileJustLoaded flag.
        * INPUTS: None.
        * OUTPUTS: A boolean; true if a file has just been loaded and false if it hasn't.
        */
        bool fileJustLoaded();

    private:
        /** DATA MEMBERS */

        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource; 
        juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};

        double loopStart;
        double loopEnd;
        double loopSeconds;
        bool justLoaded;
        bool loopIsActivated;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DJAudioPlayer)
};