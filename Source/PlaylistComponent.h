/*
  ==============================================================================

    PlaylistComponent.h
    Created: 17 Feb 2021 3:44:24pm
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistFileProcessor.h"
#include "Track.h"
#include <vector>
#include <string>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener,
                           public juce::FileDragAndDropTarget
{
    public:
        /**
        * PURPOSE: Creates the PlaylistComponent object, loads the music library database if existent, 
        *          and initialises its data members (including adding listeners and setting the table headers).
        * INPUTS: A reference to the juce AudioFormatManager and pointers to both decks.
        * OUTPUTS: None.
        */
        PlaylistComponent(juce::AudioFormatManager& _formatManager, DeckGUI* deckGUI1, DeckGUI* deckGUI2);
        
        /**
        * PURPOSE: Destroys the PlaylistComponent object.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        ~PlaylistComponent() override;

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
        * PURPOSE: Returns the number of rows currently in the table.
        *          Implements juce TableListBoxModel (i.e. function is pure virtual).
        * INPUTS: None.
        * OUTPUTS: Number of rows currently in the table.
        */
        int getNumRows() override;
    
        /**
        * PURPOSE: Draw the background behind one of the rows in the table.
        *          Implements juce TableListBoxModel (i.e. function is pure virtual).
        * INPUTS: The graphics context, row number, width of row, height of row, 
        *         and a boolean indicating if the row has been selected.
        * OUTPUTS: None.
        */
        void paintRowBackground(juce::Graphics& g,
                                int rowNumber, 
                                int width, 
                                int height, 
                                bool rowIsSelected) override;

        /**
        * PURPOSE: Draws one of the cells.
        *          Implements juce TableListBoxModel (i.e. function is pure virtual).
        * INPUTS: The graphics context, row number, column id, width of cell, height of cell,
        *         and a boolean indicating if the cell has been selected.
        * OUTPUTS: None.
        */
        void paintCell(juce::Graphics& g,
                       int rowNumber, 
                       int columnId, 
                       int width, 
                       int height, 
                       bool rowIsSelected) override;

        /**
        * PURPOSE: Creates or updates a custom component to go in a cell.
        *          Overrides juce TableListBoxModel virtual member function.
        * INPUTS: The row number, column id, a boolean indicating if the row 
        *          has been selected and a pointer to the component to be updated.
        * OUTPUTS: A pointer to the updated component.
        */
        juce::Component* refreshComponentForCell(int rowNumber, 
                                                 int columnId, 
                                                 bool isRowSelected, 
                                                 juce::Component* existingComponentToUpdate) override;

        /**
        * PURPOSE: Implements juce Button::Listener.
        * INPUTS: A pointer to the juce button that was clicked.
        * OUTPUTS: None.
        */
        void buttonClicked(juce::Button* button) override;

        /**
        * PURPOSE: Called when the user changes the text in some way.
        * INPUTS: A reference to the juce text editor that had its text changed.
        * OUTPUTS: None.
        */
        void textEditorTextChanged(juce::TextEditor& editor);

        /**
        * PURPOSE: Callback to check whether this target is interested in the set of files being offered.
        *          Implements juce FileDragAndDropTarget (i.e. function is pure virtual).
        * INPUTS: The set of (absolute) pathnames of the files that the user is dragging.
        * OUTPUTS: A boolean; more specifically, true if this component wants to receive the other callbacks
        *          regarding this type of object; if it returns false, no other callbacks will be made.
        */
        bool isInterestedInFileDrag(const juce::StringArray& files) override;
        
        /**
        * PURPOSE: Callback to indicate that the user has dropped the files onto this component.
        *          Implements juce FileDragAndDropTarget (i.e. function is pure virtual).
        * INPUTS: The set of (absolute) pathnames of the files that the user is dragging,
        *         the mouse x position, relative to this component and
        *         the mouse y position, relative to this component.
        * OUTPUTS: None.
        */
        void filesDropped(const juce::StringArray& files, int x, int y) override;

    private:
        /**
        * PURPOSE: Checks if there is already a music library database stored as a file.
        * INPUTS: None.
        * OUTPUTS: A boolean. True if there is already a playlist file and false if there isn't.
        */
        static bool playlistFileExists();

        /**
        * PURPOSE: Gets the file path where the music library database is stored.
        * INPUTS: None.
        * OUTPUTS: The path of the playlist file.
        */
        static std::string getPlaylistFilePath();

        /**
        * PURPOSE: Checks the song title against all the titles in the
        *          music library to determine if it is a duplicate.
        * INPUTS: The title of the song as a juce string.
        * OUTPUTS: A boolean. True if the song is a duplicate and false if it isn't.
        */
        bool songIsDuplicate(juce::String songTitle);


        /** DATA MEMBERS */

        juce::TextButton addToLibraryBtn{ "+ ADD TO LIBRARY" };
        juce::TextEditor searchBar { "Search", 0 };
        std::vector<Track> tracksToDisplay;
        std::vector<Track> allTracks;

        juce::TableListBox tableComponent;
        juce::AudioFormatManager& formatManager;

        DJAudioPlayer* player;
        DeckGUI* leftDeck;
        DeckGUI* rightDeck;
        PlaylistFileProcessor fileProcessor;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};