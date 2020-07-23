#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent():  frequency(440), phase(0), waveTableSize(1024), amplitude(0.25), maxAmplitude(11.0)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    frequencySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    frequencySlider.setRange(50.0f, 2000, 0.001);
    frequencySlider.setValue(frequency);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 40);
    frequencySlider.addListener(this);
    addAndMakeVisible(frequencySlider);
    
    
    amplitudeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    amplitudeSlider.setValue(amplitude * maxAmplitude);
    amplitudeSlider.setRange(0, maxAmplitude, 0.001);
    amplitudeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 40);
    amplitudeSlider.addListener(this);
   
    addAndMakeVisible(amplitudeSlider);


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
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}



//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    currentSampleRate = sampleRate;

    for (int i = 0; i < waveTableSize; i++)
    {
        waveTable.insert(i, sin(2.0 * juce::MathConstants<double>::pi * i / waveTableSize));
    }

}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    float* const leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    float* const rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (int sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        float aSampleValue = waveTable[(int)phase] * amplitude;
        leftChannel[sample] = aSampleValue;
        rightChannel[sample] = aSampleValue;

    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &frequencySlider)
    {
        updateFrequency();
    }
    else if (slider == &amplitudeSlider)
    {
        updateAmplitude();
    }

}

void MainComponent::updateFrequency()

{
    frequency = frequencySlider.getValue();
    increment = frequency * waveTableSize / currentSampleRate;
    phase = fmod((phase + increment), waveTableSize);
}

void MainComponent::updateAmplitude()
{
    amplitude = amplitudeSlider.getValue() / maxAmplitude;
}
//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    const float border = 50;
    frequencySlider.setBounds(0, border, 300, 300);
    amplitudeSlider.setBounds(getWidth()/2,  border, 300, 300);
}

