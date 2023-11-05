/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Roughness/Roughness.h"
#include "YIN/Yin.h"
#include "HighPass/HighPass.h"

//==============================================================================
/**
*/
class VocalDistortionAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    VocalDistortionAudioProcessor();
    ~VocalDistortionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
	//==============================================================================

	void getParametersValues();

    //==============================================================================
	juce::AudioBuffer<float> tmpCopyBuffer;

	std::unique_ptr<Roughness> roughness;
	std::unique_ptr<Yin> yin;
	std::unique_ptr<HighPass> highPass;

	juce::AudioParameterInt* subHarmonicsParam;
	juce::AudioParameterFloat* dryWetParam;
	juce::AudioParameterFloat* hAmpParam;
	juce::AudioParameterFloat* highPassFrequencyParam;

	juce::SmoothedValue<float> hAmpSmoothing;
	juce::SmoothedValue<float> highPassFrequencySmoothing;

	float dryWet{1.f};
	float highPassFreq{1000.f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocalDistortionAudioProcessor)
};
