# OtoDecks DJ Mixer App
A DJ Mixer Desktop Application developed using C++ and JUCE.
<br><br>
Video Tutorial: https://youtu.be/247qGKv7T10 
<br>
Download for Windows (with instructions): https://github.com/Mary-Brenda/DJ-Mixer-App/releases/tag/v1.0.0 
<br>

Some features of the app include:
- Persistent Music Library which displays metadata such as artist name, track title and track length, and allows users to add, delete, search for and load tracks to the left or right deck.
- Basic controls such as Play, Pause, Stop and Load buttons, as well as Volume and Speed sliders.
- 360 degrees rotary position slider which moves to reflect the current position in the track.
- Disc/turntable that rotates as the track plays (as a function of the current position in the track) and, together with the position slider, can be held and dragged to the right or left to move to a specific position in the track.
- Extra controls such as Loop buttons to play the last X seconds repeatedly and Hot Cues buttons to save/set points in the track to later jump to during mixing (Click sets a hot cue button while CTRL + click resets it).
- Waveform of the track to be played, with the artist name, track title and remaining duration of the track displayed above the waveform.
- Automatic reset of all controls by loading a new track or reloading the same track.
- Automatic replay/looping immediately a track ends, except the track is paused or stopped.
