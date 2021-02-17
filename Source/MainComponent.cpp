#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
    
    // set the background color to be black for the whole app
    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::black);
    
    // reveal both decks
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    // reveal playlist component
    addAndMakeVisible(playlistComponent);
    // register basic formats once ahead of other component creation
    // for passing to audio players
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // register both players with mixerSource for parallel playback
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    // prepare both players for plaback
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // pass incoming audio block to mixerSource
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    
    // release resources from sources and players
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // keep decks within resonable bounds on resize
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() * 0.6);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() * 0.6);
    // keep playlist component within resonable bounds on resize
    playlistComponent.setBounds(0, getHeight() * 0.6, getWidth(), getHeight() * 0.4);
}
