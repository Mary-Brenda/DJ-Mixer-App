/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 17 Feb 2021 3:44:24pm
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager,
                                     DeckGUI* deckGUI1, 
                                     DeckGUI* deckGUI2)
                                    : formatManager(_formatManager),
                                      leftDeck(deckGUI1),
                                      rightDeck(deckGUI2)
{
    if (playlistFileExists())
    {
        std::string playlistFilePath = getPlaylistFilePath();
        tracksToDisplay = fileProcessor.loadData(playlistFilePath);
        allTracks = fileProcessor.loadData(playlistFilePath);
        tableComponent.updateContent();
    }

    tableComponent.getHeader().addColumn("Track title", 1, 
                                         510, 200, 900,
                                         juce::TableHeaderComponent::defaultFlags);
    tableComponent.getHeader().addColumn("Length", 2,
                                         200, 150, 450,
                                         juce::TableHeaderComponent::defaultFlags);
    tableComponent.getHeader().addColumn("L", 3, 
                                         40, 20, 70,
                                         juce::TableHeaderComponent::defaultFlags, 0);
    tableComponent.getHeader().addColumn("R", 4,
                                         40, 20, 70,
                                         juce::TableHeaderComponent::defaultFlags, 1);
    tableComponent.getHeader().addColumn("Delete", 5,
                                         50, 20, 70,
                                         juce::TableHeaderComponent::defaultFlags);

    tableComponent.setModel(this);

    addAndMakeVisible(addToLibraryBtn);
    addAndMakeVisible(searchBar);
    addAndMakeVisible(tableComponent);

    addToLibraryBtn.addListener(this);
    searchBar.addListener(this);

    searchBar.setTextToShowWhenEmpty("Search Tracks", juce::Colours::white);
}

PlaylistComponent::~PlaylistComponent()
{
} 

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGBA(55, 56, 60, 255));
}

void PlaylistComponent::resized()
{
    double rowH = getHeight() / 8.0;
    double rowW = getWidth() / 8.0;
    
    searchBar.setBounds(5, 5, (rowW * 6) - 5, rowH);
    searchBar.setJustification(juce::Justification::centredLeft);
    addToLibraryBtn.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addToLibraryBtn.setColour(juce::TextButton::ColourIds::buttonColourId, 
                              juce::Colour::fromRGBA(102, 94, 199, 255));
    addToLibraryBtn.setBounds((rowW * 6) + 5, 5, (rowW * 2) - 10, rowH);
    tableComponent.setBounds(0, rowH + 10, getWidth(), getHeight() - (rowH + 10));
}

int PlaylistComponent::getNumRows()
{
    return tracksToDisplay.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected)
{
    auto alternateColour = getLookAndFeel().findColour(juce::ListBox::backgroundColourId)
                            .interpolatedWith(getLookAndFeel().findColour(juce::ListBox::textColourId), 0.03f);

    // Highlight selected row.
    if (rowIsSelected)
    {
        g.fillAll(juce::Colour::fromRGBA(102, 94, 199, 255));
    }
    else if (rowNumber % 2)
    {
        g.fillAll(alternateColour);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected)
{
    g.setColour(getLookAndFeel().findColour(juce::ListBox::textColourId));
    g.setFont(14.0f);

    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(tracksToDisplay[rowNumber].title,
                5, 0,
                width - 4, height,
                juce::Justification::centredLeft,
                true);
        }
        if (columnId == 2)
        {
            g.drawText(tracksToDisplay[rowNumber].length,
            5, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
        }
    }
    g.setColour(getLookAndFeel().findColour(juce::ListBox::backgroundColourId));
    g.fillRect(width - 1, 0, 1, height);
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                            int columnId,
                                                            bool isRowSelected,
                                                            juce::Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        juce::TextButton* loadLeftBtn = (juce::TextButton*)existingComponentToUpdate;
        
        if (loadLeftBtn == 0)
        {
            loadLeftBtn = new juce::TextButton();
        }
        
        juce::String id{ rowNumber };
        loadLeftBtn->setComponentID(id);
        loadLeftBtn->setButtonText("<");
        loadLeftBtn->setMouseCursor(juce::MouseCursor::PointingHandCursor);
        loadLeftBtn->setColour(juce::TextButton::ColourIds::buttonColourId,
                               juce::Colour::fromRGBA(37, 136, 238, 255));
        loadLeftBtn->addListener(this);
        
        return loadLeftBtn;
    }

    if (columnId == 4)
    {
        juce::TextButton* loadRightBtn = (juce::TextButton*)existingComponentToUpdate;
        
        if (loadRightBtn == 0)
        {
            loadRightBtn = new juce::TextButton();
        }
        
        juce::String id{ "0." + std::to_string(rowNumber) };
        loadRightBtn->setComponentID(id);
        loadRightBtn->setButtonText(">");
        loadRightBtn->setMouseCursor(juce::MouseCursor::PointingHandCursor);
        loadRightBtn->setColour(juce::TextButton::ColourIds::buttonColourId,
                                juce::Colour::fromRGBA(146, 14, 27, 255));
        loadRightBtn->addListener(this);

        return loadRightBtn;
    }

    if (columnId == 5)
    {
        juce::TextButton* deleteTrackBtn = (juce::TextButton*)existingComponentToUpdate;

        if (deleteTrackBtn == 0)
        {
            deleteTrackBtn = new juce::TextButton();
        }
        
        juce::String id{ "X" + std::to_string(rowNumber) };
        deleteTrackBtn->setComponentID(id);
        deleteTrackBtn->setButtonText("X");
        deleteTrackBtn->setMouseCursor(juce::MouseCursor::PointingHandCursor);
        deleteTrackBtn->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
        deleteTrackBtn->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::red);
        deleteTrackBtn->addListener(this);

        return deleteTrackBtn;
    }
    else
    {
        return 0;
    }
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &addToLibraryBtn)
    {
        juce::FileChooser chooser{ "Add a music file..." };
        if (chooser.browseForMultipleFilesToOpen())
        {
            for (const auto& result : chooser.getResults())
            {
                juce::File songFile{ result };
                juce::String songTitle = leftDeck->getSongTitle(songFile);
                juce::String songLength = rightDeck->getSongLength(songFile);
                juce::String songPath = juce::URL{ songFile }.toString(false);

                if (playlistFileExists())
                {
                    // Append track data to playlist file and 
                    // append the created track to tracksToDisplay vector.

                    if (songIsDuplicate(songTitle) == false)
                    {
                        Track trackToAdd{ songTitle, songLength, songPath };
                        fileProcessor.appendData(songTitle, songLength, songPath);
                        tracksToDisplay.push_back(trackToAdd);
                        allTracks.push_back(trackToAdd);
                    }
                }
                else
                {
                    // Create file to store tracksToDisplay.
                    fileProcessor.createPlaylistFile("playlist.txt");

                    // Append track data to playlist file and 
                    // append the created track to tracksToDisplay vector.
                    Track trackToAdd{ songTitle, songLength, songPath };
                    fileProcessor.appendData(songTitle, songLength, songPath);
                    tracksToDisplay.push_back(trackToAdd);
                    allTracks.push_back(trackToAdd);
                }

                tableComponent.updateContent();
            }
        }
    }
    else
    {
        std::string componentID = button->getComponentID().toStdString();

        if (componentID.find('.') != std::string::npos) // since it's not equal to no pos, it's found!
        {
            int start = componentID.find_first_of('.', 0);
            int rightBtnId = std::stoi(componentID.substr(start+1));
            juce::URL pathURL{ tracksToDisplay[rightBtnId].path };
            rightDeck->loadTrack(tracksToDisplay[rightBtnId].title, 
                                 tracksToDisplay[rightBtnId].length, 
                                 pathURL);
        }
        if (componentID.find('X') != std::string::npos)
        {
            int start = componentID.find_first_of('X', 0);
            int deleteTrackBtnId = std::stoi(componentID.substr(start + 1));
            tracksToDisplay.erase(tracksToDisplay.begin() + deleteTrackBtnId);
            allTracks.erase(allTracks.begin() + deleteTrackBtnId);
            fileProcessor.deleteData(deleteTrackBtnId);
            tableComponent.updateContent();
        }
        else if (componentID.find('.') == std::string::npos && 
                 componentID.find('X') == std::string::npos)
        {
            int leftBtnId = std::stoi(componentID);
            juce::URL pathURL{ tracksToDisplay[leftBtnId].path };
            leftDeck->loadTrack(tracksToDisplay[leftBtnId].title, 
                      tracksToDisplay[leftBtnId].length, 
                      pathURL);
        }
    }
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor& editor)
{
    if (editor.isEmpty() == false)
    {
        std::vector<Track> results;
        juce::String keyword = editor.getText();

        for (unsigned int i = 0; i < allTracks.size(); ++i)
        {
            juce::String songTitle{ allTracks[i].title };
            songTitle = songTitle.toLowerCase();
            keyword = keyword.toLowerCase();

            if (songTitle.startsWith(keyword))
            {
                results.push_back(allTracks[i]);
            }
        }

        tracksToDisplay = results;
        tableComponent.updateContent();
    }
    else
    {
        tracksToDisplay = allTracks;
        tableComponent.updateContent();
    }
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (const auto& file : files)
    {
        juce::File songFile{ file };
        juce::String songTitle = leftDeck->getSongTitle(songFile);
        juce::String songLength = rightDeck->getSongLength(songFile);
        juce::String songPath = juce::URL{ songFile }.toString(false);

        if (playlistFileExists())
        {
            // Append track data to playlist file and append the created track to tracksToDisplay vector.
            unsigned int songID = allTracks.size();

            if (songIsDuplicate(songTitle) == false)
            {
                Track trackToAdd{ songTitle, songLength, songPath };
                fileProcessor.appendData(songTitle, songLength, songPath);
                tracksToDisplay.push_back(trackToAdd);
                allTracks.push_back(trackToAdd);
            }
        }
        else
        {
            // Create file to store tracksToDisplay.
            fileProcessor.createPlaylistFile("playlist.txt");

            // Append track data to playlist file and append the created track to tracksToDisplay vector.
            unsigned int songID = allTracks.size();
            Track trackToAdd{ songTitle, songLength, songPath };
            fileProcessor.appendData(songTitle, songLength, songPath);
            tracksToDisplay.push_back(trackToAdd);
            allTracks.push_back(trackToAdd);
        }

        tableComponent.updateContent();
    }
}

bool PlaylistComponent::playlistFileExists()
{
    auto dir = juce::File::getCurrentWorkingDirectory();

    int numTries = 0;

    while (!dir.getChildFile("Resources").exists() && numTries++ < 15)
    {
        dir = dir.getParentDirectory();
    }

    auto tableFile = dir.getChildFile("Resources").getChildFile("playlist.txt");

    if (tableFile.exists())
    {
        return true;
    }

    return false;
}

std::string PlaylistComponent::getPlaylistFilePath()
{
    auto dir = juce::File::getCurrentWorkingDirectory();
    int numTries = 0;

    while (!dir.getChildFile("Resources").exists() && numTries++ < 15)
    {
        dir = dir.getParentDirectory();
    }

    std::string resourcesDir = (dir.getChildFile("Resources").getFullPathName()).toStdString();
    std::string filePath = resourcesDir + "/playlist.txt";

    return filePath;
}

bool PlaylistComponent::songIsDuplicate(juce::String songTitle)
{
    for (const auto& song : allTracks)
    {
        if (song.title == songTitle)
        {
            return true;
        }
    }

    return false;
}