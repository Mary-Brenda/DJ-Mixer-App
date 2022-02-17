/*
  ==============================================================================

    PlaylistFileProcessor.h
    Created: 28 Feb 2021 10:10:38am
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Track.h"

class PlaylistFileProcessor
{
    public:
        /**
        * PURPOSE: Creates the PlaylistFileProcessor object.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        PlaylistFileProcessor();

        /**
        * PURPOSE: Destroys the PlaylistFileProcessor object and closes the fstream.
        * INPUTS: None.
        * OUTPUTS: None.
        */
        ~PlaylistFileProcessor();

        /**
        * PURPOSE: Creates a playlist file and appends the first song data to it.
        * INPUTS: The name of the playlist file.
        * OUTPUTS: None.
        */
        void createPlaylistFile(std::string fileName);

        /**
        * PURPOSE: Separates each line in the playlist file into tokens/parts based on the sent separator.
        * INPUTS: The line in the file to be separated and the separator to use as the delimiter.
        * OUTPUTS: A vector of strings representing the tokens.
        */
        std::vector<Track> loadData(std::string filePath);

        /**
        * PURPOSE: Appends track data to the playlist file.
        * INPUTS: The title, length and file path of the track to be appended.
        * OUTPUTS: None.
        */
        void appendData(juce::String title, juce::String length, juce::String path);

        /**
        * PURPOSE: Deletes the track data from the playlist file as indicated by the line number.
        * INPUTS: The line number of the track to be deleted.
        * OUTPUTS: None.
        */
        void deleteData(int lineNumber);

    private:
        /**
        * PURPOSE: Separates each line in the playlist file into tokens/parts based on the sent separator.
        * INPUTS: The line in the file to be separated and the separator to use as the delimiter.
        * OUTPUTS: A vector of strings representing the tokens.
        */
        static std::vector<std::string> tokenise(std::string line, char separator);

        /**
        * PURPOSE: Converts the vector of token strings to a Track data structure.
        * INPUTS: A vector of strings representing the tokens.
        * OUTPUTS: A track data structure (created from the Track class).
        */
        static Track stringsToTrack(std::vector<std::string> tokens);


        /** DATA MEMBERS */
        std::ofstream mStream;
        std::string playlistFilePath;
};