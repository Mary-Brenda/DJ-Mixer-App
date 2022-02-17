/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Jan 2021 6:44:48pm
    Author:  Mary-Brenda Akoda

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                 juce::AudioFormatManager & formatManagerToUse,
                 juce::AudioThumbnailCache & cacheToUse,
                 juce::Colour& colourToUse,
                 juce::TooltipWindow* _tooltipWindow
                ) : player(_player), 
                    formatManager(formatManagerToUse),
                    waveformDisplay(formatManagerToUse, cacheToUse, colourToUse),
                    accentColour(colourToUse),
                    tooltipWindow(_tooltipWindow),
                    isLoaded(false),
                    userExperienceLevel(0)
{
    // Get disc record image to display.
    disc = getImageFromResources("disc-record-resized-207.png");
    
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);

    playButton.addListener(this);
    pauseButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    posSlider.addListener(this);
  
    posSlider.setRange(0.0, 1.0);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    double layoutH = (double) (getHeight() / 3);
    double rowH = (double) (getHeight() / 8);
    double buttonW = (double) (getWidth() / 10);

    // Fill background.
    g.fillAll(juce::Colour::fromRGBA(40, 40, 40, 255));
    
    g.setColour(juce::Colours::yellowgreen);
    
    // Style buttons and change appearance on click.
    g.drawRoundedRectangle(buttonW - 8, rowH * 7.1 - 2, buttonW * 2, rowH / 1.2 + 2, 6, 0.5);
    if (playButton.getToggleState())
    {
        g.fillRoundedRectangle(buttonW - 8, rowH * 7.1 - 2, buttonW * 2, rowH / 1.2 + 2, 6);
    }

    g.drawRoundedRectangle(buttonW * 3, rowH * 7.1 - 2, buttonW * 2, rowH / 1.2 + 2, 6, 0.5);
    if (pauseButton.getToggleState())
    {
        g.fillRoundedRectangle(buttonW * 3, rowH * 7.1 - 2, buttonW * 2, rowH / 1.2 + 2, 6);
    }
    
    g.drawRoundedRectangle(buttonW * 5 + 8, rowH * 7.1 - 2, buttonW * 2, rowH / 1.2 + 2, 6, 0.5);
    if (stopButton.getToggleState())
    {
        g.fillRoundedRectangle(buttonW * 5 + 8, rowH * 7.1 - 2, buttonW * 2, rowH / 1.2 + 2, 6);
    }
    
    g.drawRoundedRectangle(buttonW * 7 + 16, rowH * 7.1 - 2, buttonW * 2, rowH / 1.2 + 2, 6, 0.5);
    if (loadButton.getToggleState())
    {
        g.fillRoundedRectangle(buttonW * 7 + 16, rowH * 7.1 - 2, buttonW * 2, rowH / 1.2 + 2, 6);
    }

    // Set position and transform for disc image.
    g.setOrigin(((getWidth() / static_cast<double>(4)) - rowH / 2) + ((getWidth() / 1.5) - 5)/2, 
                (layoutH - 10) + ((getHeight() - (layoutH - rowH / 2)) / 1.5)/2);

    // Position origin of disc rotation.
    juce::AffineTransform transform(AffineTransform::translation((float)(disc.getWidth() / -2),
                                                           (float)(disc.getHeight() / -2)));

    // Draw disc rotation on file load and play.
    transform = transform.followedBy(getTransform());

    // Draw the disc image transformation.
    g.drawImageTransformed(disc, transform, false);

    repaint();
}

void DeckGUI::resized()
{
    double rowH = (double) (getHeight() / 8);
    double rowW = (double) (getWidth() / 8);
    double buttonW = (double) (getWidth() / 10);
    double layoutH = (double) (getHeight() / 3);

    styleButton(playButton, buttonW - 8);
    styleButton(pauseButton, buttonW * 3);
    styleButton(stopButton, buttonW * 5 + 8);
    styleButton(loadButton, buttonW * 7 + 16);

    posSlider.setBounds(rowW, rowH * 2, rowW * 6, rowH * 5);
    posSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    posSlider.setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    posSlider.setColour(juce::Slider::ColourIds::thumbColourId, accentColour);
    posSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::yellowgreen);
    posSlider.setRotaryParameters(juce::MathConstants<float>::pi,
                                  juce::MathConstants<float>::twoPi + juce::MathConstants<float>::pi, 
                                  true);

    waveformDisplay.setBounds(0, 0, getWidth(), layoutH - rowH / 2);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        player->start();

        if (isLoaded)
        {
            playButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            pauseButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            stopButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            loadButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
    }

    if (button == &pauseButton)
    {
        player->pause();
        
        if (isLoaded)
        {
            playButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            pauseButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            stopButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            loadButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
    }
    
    if (button == &stopButton)
    {
        player->stop();
        
        if (isLoaded)
        {
            playButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            pauseButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            stopButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            loadButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        }
    }
    
    if (button == &loadButton)
    {
        if (isLoaded)
        {
            playButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            pauseButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            stopButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            loadButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        }

        juce::FileChooser chooser{"Select a music file..."};

        if (chooser.browseForFileToOpen())
        {
            juce::File trackFile = chooser.getResult();
            juce::String trackName = getSongTitle(trackFile);
            juce::String trackLength = getSongLength(trackFile);
            juce::URL trackPath = juce::URL{ trackFile };

            player->loadURL(trackPath);
            waveformDisplay.loadFile(trackName, trackLength, trackPath);
            
            isLoaded = true;
            playButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            pauseButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            stopButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            loadButton.setToggleState(false, juce::NotificationType::dontSendNotification);
            
            userExperienceLevel++;
        }
    }
}

void DeckGUI::sliderValueChanged (juce::Slider *slider)
{
    if (slider == &posSlider)
    {
        double sliderValue = slider->getValue();
        
        if (sliderValue >= 1.0)
        {
            sliderValue = 0.0;
            player->setPositionRelative(sliderValue);
            player->start();
        }
        else
        {
            player->setPositionRelative(sliderValue);
        }
    }
}

bool DeckGUI::isInterestedInFileDrag (const juce::StringArray &files)
{
  return true; 
}

void DeckGUI::filesDropped (const juce::StringArray &files, int x, int y)
{
    if (files.size() == 1)
    {
        juce::String trackName = getSongTitle(juce::File{ files[0] });
        juce::String trackLength = getSongLength(juce::File{ files[0] });
        juce::URL trackPath = juce::URL{ juce::File{files[0]} };
        player->loadURL(trackPath);
        waveformDisplay.loadFile(trackName, trackLength, trackPath);

        isLoaded = true;
        playButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        pauseButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        stopButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        loadButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        
        userExperienceLevel++;
    }
}

void DeckGUI::timerCallback()
{
    double relativePosition = player->getPositionRelative();

    if (!isnan(relativePosition))
    {
        waveformDisplay.setPositionRelative(relativePosition);
        waveformDisplay.updateTrackDuration(player->getPosInTrack());
        posSlider.setValue(relativePosition);
    }

    if (userExperienceLevel <= 2)
    {
        posSlider.setTooltip("Click and drag disc to the right or \nleft to change the current position.");
    }
    else
    {
        posSlider.setTooltip("");
    }
}

void DeckGUI::loadTrack(juce::String trackName, juce::String trackLength, juce::URL trackPath)
{
    player->loadURL(trackPath);
    waveformDisplay.loadFile(trackName, trackLength, trackPath);

    isLoaded = true;
    playButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    pauseButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    stopButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    loadButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    
    userExperienceLevel++;
}

juce::String DeckGUI::getSongTitle(juce::File songFile)
{
    juce::String songTitle{ songFile.getFileNameWithoutExtension() };
    return songTitle;
}

juce::String DeckGUI::getSongLength(juce::File songFile)
{
    int numSamples{0};
    double sampleRate{0.0};
    double lengthInSecs{0.0};
    auto* reader = formatManager.createReaderFor(juce::URL{ songFile }.createInputStream(false));

    if (reader != nullptr) // good file!
    {
        numSamples = reader->lengthInSamples;
        sampleRate = reader->sampleRate;
        lengthInSecs = numSamples / sampleRate;
    }

    // Delete reader after use to prevent memory leaks.
    delete reader;

    int minutes = floor(lengthInSecs / 60.0);
    int seconds = floor(lengthInSecs - (minutes * 60));

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

inline std::unique_ptr<juce::InputStream> DeckGUI::createImgFileInputStream(const char* resourcePath)
{
    auto dir = juce::File::getCurrentWorkingDirectory();
    
    int numTries = 0;    
    while (!dir.getChildFile("Resources").exists() && numTries++ < 15)
    {
        dir = dir.getParentDirectory();
    }
    
    auto imageFile = dir.getChildFile("Resources").getChildFile(resourcePath);
    jassert(imageFile.existsAsFile());

    return imageFile.createInputStream();
}

inline juce::Image DeckGUI::getImageFromResources(const char* imageName)
{
    auto hashCode = (juce::String(imageName) + "@otodecks_resources").hashCode64();
    auto img = juce::ImageCache::getFromHashCode(hashCode);

    if (img.isNull())
    {
        std::unique_ptr<juce::InputStream> discImgStream(createImgFileInputStream(imageName));

        if (discImgStream == nullptr)
            return {};

        img = juce::ImageFileFormat::loadFrom(*discImgStream);

        juce::ImageCache::addImageToCache(img, hashCode);
    }

    return img;
}

void DeckGUI::styleButton(juce::TextButton& button, double x)
{
    double rowH = (double) (getHeight() / 8);
    double buttonW = (double) (getWidth() / 10);

    button.setBounds(x, rowH * 7.1 - 2, buttonW * 2, rowH / 1.2);
    button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    button.setColour(juce::TextButton::ColourIds::textColourOffId, accentColour);
    button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

juce::AffineTransform DeckGUI::getTransform()
{
    juce::AffineTransform t;
    t = t.rotated(fmod(0.15 * player->getPosInTrack(), 2.0) * juce::MathConstants<float>::twoPi);

    return t;
}