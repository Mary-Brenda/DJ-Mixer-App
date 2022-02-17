/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Jan 2021 4:22:22pm
Author:  Mary-Brenda Akoda

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) 
                            : formatManager(_formatManager), 
                              loopIsActivated(false),
                              loopEnd(0.0),
                              loopStart(0.0),
                              loopSeconds(0.0),
                              justLoaded(false)
{
    startTimer(500);
}

DJAudioPlayer::~DJAudioPlayer()
{
    stopTimer();
}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{   
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    
    if (reader != nullptr) // good file!
    {       
        std::unique_ptr<juce::AudioFormatReaderSource> newSource 
        (
            new juce::AudioFormatReaderSource (reader, true)
        ); 
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
        readerSource.reset (newSource.release());          
    }
    else
    {
        DBG("DJAudioPlayer::loadURL Bad audio file!");
    }

    justLoaded = true;
}

void DJAudioPlayer::setGain(double gain)
{
    gain = gain / 100;

    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else 
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos \
                      should be between 0 and 1" << std::endl;
    }
    else 
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
        justLoaded = false;
    }
}

void DJAudioPlayer::setLoop(double seconds)
{
    if (seconds <= 0 || seconds > 16.0)
    {
        loopIsActivated = false;
        std::cout << "DJAudioPlayer::setLoop seconds should be greater \
                      than 0 and less than or equal to 16" << std::endl;
    }
    else
    {
        loopIsActivated = true;
        loopSeconds = seconds;
        loopEnd = transportSource.getCurrentPosition();
        loopStart = loopEnd - seconds;
    }
}

void DJAudioPlayer::timerCallback()
{
    if (loopIsActivated)
    {
        if (transportSource.getCurrentPosition() >= loopEnd)
        {
            transportSource.setPosition(loopStart);
        }

        if (loopSeconds == 0)
        {
            loopIsActivated = false;
            return;
        }
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
    justLoaded = false;
}

void DJAudioPlayer::pause()
{
    transportSource.stop();
    justLoaded = false;
}

void DJAudioPlayer::stop()
{
  transportSource.stop();
  transportSource.setPosition(0);
  justLoaded = false;
}

double DJAudioPlayer::getPosInTrack()
{
    return transportSource.getCurrentPosition();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

bool DJAudioPlayer::fileJustLoaded()
{
    if (justLoaded)
    {
        justLoaded = false;
        return true;
    }

    return false;
}