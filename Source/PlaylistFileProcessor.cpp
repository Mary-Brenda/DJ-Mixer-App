/*
  ==============================================================================

    PlaylistFileProcessor.cpp
    Created: 28 Feb 2021 10:10:38am
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#include "PlaylistFileProcessor.h"

PlaylistFileProcessor::PlaylistFileProcessor()
{

}

PlaylistFileProcessor::~PlaylistFileProcessor()
{
    mStream.close();
}

void PlaylistFileProcessor::createPlaylistFile(std::string fileName)
{
    auto dir = juce::File::getCurrentWorkingDirectory();
    int numTries = 0;

    while (!dir.getChildFile("Resources").exists() && numTries++ < 15)
    {
        dir = dir.getParentDirectory();
    }

    std::string resourcesDir = (dir.getChildFile("Resources").getFullPathName()).toStdString();
    std::string filePath = resourcesDir + "/" + fileName;
    playlistFilePath = filePath;

    // Open file for writing purposes.
    mStream.open(filePath, std::fstream::app);

    if (mStream.fail())
    {
        // Throw error if file doesn't open.
        throw std::iostream::failure("Cannot open file " + fileName);
    }

    mStream.close();
}

std::vector<Track> PlaylistFileProcessor::loadData(std::string filePath)
{
    playlistFilePath = filePath;

    std::vector<Track> tracks;
    std::ifstream playlistFile{filePath};
    std::string line;

    if (playlistFile.is_open())
    {
        while (std::getline(playlistFile, line))
        {
            try
            {
                Track newTrack = stringsToTrack(tokenise(line, '|'));
                tracks.push_back(newTrack);
            }
            catch (const std::exception& e)
            {
                DBG("PlaylistFileProcessor::loadData bad data");
            }
        }

        playlistFile.close();
    }
    else
    {
        DBG("Playlist file not open.");
    }

    return tracks;
}

void PlaylistFileProcessor::appendData(juce::String title,
                                       juce::String length, 
                                       juce::String path)
{
    mStream.open(playlistFilePath, std::fstream::app);

    if (mStream.fail())
    {
        // Throw error if file doesn't open.
        throw std::iostream::failure("Cannot append to file " + playlistFilePath);
    }

    mStream << title << "|" << length << "|" << path << std::endl;
    mStream.close();
}

std::vector<std::string> PlaylistFileProcessor::tokenise(std::string line, char separator)
{
    std::vector<std::string> tokens;
    signed int start;
    signed int end;
    std::string token;

    start = line.find_first_not_of(separator, 0);
    do {
        end = line.find_first_of(separator, start);

        if (start == line.length() || start == end) break;
        if (end >= 0) token = line.substr(start, end - start);
        else token = line.substr(start, line.length() - start);
        
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}

Track PlaylistFileProcessor::stringsToTrack(std::vector<std::string> tokens)
{
    if (tokens.size() != 3) // bad
    {
        DBG("Bad line ");
        throw std::exception{};
    }

    // We have 3 tokens.
    Track newTrack{tokens[0], tokens[1], tokens[2]};

    return newTrack;
}

void PlaylistFileProcessor::deleteData(int lineNumber)
{
    std::ifstream playlistFile{ playlistFilePath };
    std::string line;
    int lineNum = 0;
    std::vector<std::string> tempPlaylist;

    if (playlistFile.is_open())
    {
        while (std::getline(playlistFile, line))
        {
            if (lineNum != lineNumber)
            {
                tempPlaylist.push_back(line);
            }
            lineNum++;
        }

        playlistFile.close();
    }
    else
    {
        DBG("Playlist file not open.");
    }


    std::ofstream playlistFileToWrite{ playlistFilePath };
    for (const std::string line : tempPlaylist)
    {
        playlistFileToWrite << line << std::endl;
    }

    playlistFileToWrite.close();
}