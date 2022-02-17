/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Jan 2021 6:44:48pm
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
    public:
        /**
        * PURPOSE: Creates the DeckGUI object, initialises its data members 
                   (including adding listeners) and starts a timer.
        * INPUTS: Pointers to DJAudioPlayer and juce TooltipWindow and references to 
                  juce AudioFormatManager, juce AudioThumbnailCache, and an RGBA colour for styling.
        * OUTPUTS: None.
        */
        DeckGUI(DJAudioPlayer* _player, 
                juce::AudioFormatManager& formatManagerToUse,
                juce::AudioThumbnailCache& cacheToUse, 
                juce::Colour& colourToUse,
                juce::TooltipWindow* _tooltipWindow);

        /**
        * PURPOSE: Destroys the DeckGUI object and stops the timer.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        ~DeckGUI();

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
        * PURPOSE: Implements juce Button::Listener.
        * INPUTS: A pointer to the juce button that was clicked.
        * OUTPUTS: None.
        */
        void buttonClicked (juce::Button* button) override;

        /**
        * PURPOSE: Implements juce Slider::Listener.
        * INPUTS: A pointer to the juce slider that had its value changed.
        * OUTPUTS: None.
        */
        void sliderValueChanged (juce::Slider *slider) override;

        /**
        * PURPOSE: Callback to check whether this target is interested in the set of files being offered.
        *          Implements juce FileDragAndDropTarget (i.e. function is pure virtual).
        * INPUTS: The set of (absolute) pathnames of the files that the user is dragging.
        * OUTPUTS: A boolean; more specifically, true if this component wants to receive the other callbacks
        *          regarding this type of object; if it returns false, no other callbacks will be made.
        */
        bool isInterestedInFileDrag (const juce::StringArray &files) override;

        /**
        * PURPOSE: Callback to indicate that the user has dropped the files onto this component.
        *          Implements juce FileDragAndDropTarget (i.e. function is pure virtual).
        * INPUTS: The set of (absolute) pathnames of the files that the user is dragging, 
        *         the mouse x position, relative to this component and 
        *         the mouse y position, relative to this component
        * OUTPUTS: None.
        */
        void filesDropped (const juce::StringArray &files, int x, int y) override;

        /**
        * PURPOSE: The user-defined callback routine that actually gets called periodically.
        *          Implements juce Timer.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        void timerCallback() override;

        /**
        * PURPOSE: Loads the track to be played.
        * INPUTS: The track name and track length as juce strings, and the track file path as a juce URL.
        * OUTPUTS: None.
        */
        void loadTrack(juce::String trackName, juce::String trackLength, juce::URL trackPath);

        /**
        * PURPOSE: Gets the song title from the song file.
        * INPUTS: The song file as a juce File.
        * OUTPUTS: The title of the song as a juce string.
        */
        static juce::String getSongTitle(juce::File songFile);

        /**
        * PURPOSE: Calculates the song length from the song file.
        * INPUTS: The song file as a juce File.
        * OUTPUTS: The length of the song converted to juce string format.
        */
        juce::String getSongLength(juce::File songFile);

    private:
        /**
        * PURPOSE: Creates input stream for the disc image file.
        * INPUTS: A pointer to the image file path.
        * OUTPUTS: A unique pointer to the juce InputStream.
        */
        static inline std::unique_ptr<juce::InputStream> createImgFileInputStream(const char* resourcePath);

        /**
        * PURPOSE: Gets the disc record image to be displayed in the deck.
        * INPUTS: A pointer to the file name.
        * OUTPUTS: The disc record image as a juce Image.
        */
        static inline juce::Image getImageFromResources(const char* assetName);

        /**
        * PURPOSE: Styles the control buttons (i.e. play, pause, stop and load buttons).
        * INPUTS: The button to be styled and its preferred x position.
        * OUTPUTS: None.
        */
        void styleButton(juce::TextButton& button, double x);

        /**
        * PURPOSE: Applies a rotation to the disc record as a function of the track's current position.
        * INPUTS: None.
        * OUTPUTS: A 2D Affine Transformation Matrix representing a rotation.
        */
        juce::AffineTransform getTransform();


        /** DATA MEMBERS */

        juce::TextButton playButton{"PLAY"};
        juce::TextButton pauseButton{"PAUSE"};
        juce::TextButton stopButton{"STOP"};
        juce::TextButton loadButton{"LOAD"};
        juce::Slider posSlider;
        juce::Image disc;
        juce::Colour accentColour;
        bool isLoaded;
        int userExperienceLevel;

        juce::AudioFormatManager& formatManager;
        WaveformDisplay waveformDisplay;
        DJAudioPlayer* player;
        juce::TooltipWindow* tooltipWindow;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};