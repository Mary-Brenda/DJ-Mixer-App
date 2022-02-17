/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Jan 2021 3:50:16pm
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager & formatManagerToUse,
                                 juce::AudioThumbnailCache & cacheToUse,
                                 juce::Colour& colourToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0.0),
                                 posInSecs(0.0),
                                 trackName(""),
                                 trackLength(""),
                                 formatManager(formatManagerToUse),
                                 accentColour(colourToUse)
                          
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGBA(40, 40, 40, 255)); // set the background.

    g.setColour (juce::Colours::orangered);
    if (fileLoaded)
    {
        double rowW = getWidth() / 4.0;

        g.setFont(20.0f);
        g.setColour(Colours::white);
        
        // Draw song title.
        juce::Rectangle<float> titleArea(10, 5, (float) (rowW * 3) - 5, 20);
        g.drawText(trackName, titleArea, juce::Justification::centredLeft, true);

        // Draw song length.
        juce::Rectangle<float> durationArea((float)(rowW * 2.8), 5, (float) rowW, 20);
        g.drawText(trackLength, durationArea, juce::Justification::centredRight, true);

        // Draw left channel.
        g.setColour(accentColour);
        juce::Rectangle<int> waveformArea(0, 30, getWidth(), getHeight() - 30);
        audioThumb.drawChannel(g, 
                               waveformArea, 
                               0.0, 
                               audioThumb.getTotalLength(), 
                               0, 
                               1.0f);
        g.setColour(Colours::lightgreen);
        
        // Draw playhead.
        double playheadPos = position * getWidth();
        juce::Line<float> arrowLine(juce::Point<float>((float) playheadPos, 30),
                                     juce::Point<float>((float) playheadPos, 40));
        g.drawArrow(arrowLine, 20, 20, 55);
        g.drawRect((int) playheadPos, 35, 2, getHeight() - 5);
    }
    else 
    {
      g.setFont (20.0f);
      g.drawText ("File not loaded...", getLocalBounds(),
                  juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{

}

void WaveformDisplay::loadFile(juce::String fileName, juce::String fileLength, juce::URL audioURL)
{
    trackName = fileName;
    trackLength = fileLength;
    
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded)
    {
        repaint();
    }
    else {
        DBG("WaveformDisplay: not loaded!");
    }
}

void WaveformDisplay::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    posInSecs = pos;

    if (pos != position && !isnan(pos))
    {
        position = pos; // goes from 0 to 1;
        repaint();
    }
}

juce::String WaveformDisplay::getSongLength(double lengthInSecs)
{
    int minutes = (int) floor(lengthInSecs / 60.0);
    int seconds = (int) floor(lengthInSecs - (minutes * 60.0));

    juce::String songLength;

    if (seconds >= 0 && seconds < 10)
    {
        songLength = juce::String{ minutes } + ":0" + juce::String{ seconds };
    }
    else
    {
        songLength = juce::String{ minutes } + ":" + juce::String{ seconds };
    }

    return songLength;
}

void WaveformDisplay::updateTrackDuration(double currentPos)
{
    double remainingDuration;
    double totalDuration = audioThumb.getTotalLength();

    if (currentPos >= totalDuration)
    {
        remainingDuration = 0.0;
    }
    else
    {
        remainingDuration = totalDuration - currentPos;
    }
  
    trackLength = getSongLength(remainingDuration);
}