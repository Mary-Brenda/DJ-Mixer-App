/*
  ==============================================================================

    PlaylistComponent.h
    Created: 17 Feb 2021 3:44:24pm
    Author:  Brenda Akoda

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    
    void paintRowBackground(Graphics&, 
                            int rowNumber, 
                            int width, 
                            int height, 
                            bool rowIsSelected) override;

    void paintCell(Graphics&, 
                   int rowNumber, 
                   int columnId, 
                   int width, 
                   int height, 
                   bool rowIsSelected) override;

    void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;

    juce::Component* refreshComponentForCell(int rowNumber, 
                                             int columnId, 
                                             bool isRowSelected, 
                                             juce::Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

private:
    juce::TableListBox tableComponent;
    std::vector<std::string> trackTitles;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
