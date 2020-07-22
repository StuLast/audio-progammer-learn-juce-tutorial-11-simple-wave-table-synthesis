#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    juce::Array<float> waveTable;
    double waveTableSize;
    float frequency;
    double phase;
    double increment;
    float amplitude;
    float currentSampleRate;
    double maxAmplitude;

    juce::Slider frequencySlider, amplitudeSlider;
    juce::Label frequencyLabel, amplitudeLabel;

    void sliderValueChanged(juce::Slider* slider) override;
    void updateFrequency();
    void updateAmplitude();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
