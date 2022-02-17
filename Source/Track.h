/*
  ==============================================================================

    Track.h
    Created: 28 Feb 2021 10:11:40am
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Track 
{
    public:
        /** 
        * PURPOSE: Track constructor used to create and initialise 
        *          a track object made of several data members.
        * INPUTS: Track title, length and path.
        * OUTPUTS: None.
        */
        Track(juce::String _title,
              juce::String _length,
              juce::String _path);


        /** DATA MEMBERS */
        juce::String title;
        juce::String length;
        juce::String path;
};