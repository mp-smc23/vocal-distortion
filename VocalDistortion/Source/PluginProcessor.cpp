/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VocalDistortionAudioProcessor::VocalDistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
	roughness(std::make_unique<Roughness>()),
	yin(std::make_unique<Yin>()),
	highPass(std::make_unique<HighPass>())
#endif
{
	addParameter(subHarmonicsParam = new juce::AudioParameterInt({"voices", 1}, "Subharmonics", 1, 5, 1));
	addParameter(dryWetParam = new juce::AudioParameterFloat({"DryWet", 1}, "Dry/Wet", 0, 1, 0.5f));
	addParameter(hAmpParam = new juce::AudioParameterFloat({"Amplitude", 1}, "Mod Amplitude", 0, 1, 0.5f));
	addParameter(highPassFrequencyParam = new juce::AudioParameterFloat({ "HighPassFreq", 1 }, "High-Pass Frequency", { 16.35f, 7902.13f, 0.f, 0.199f }, 440.f, juce::AudioParameterFloatAttributes().withLabel(juce::String("Hz"))));
}

VocalDistortionAudioProcessor::~VocalDistortionAudioProcessor()
{
}

//==============================================================================
const juce::String VocalDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VocalDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VocalDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VocalDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VocalDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VocalDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VocalDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VocalDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VocalDistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void VocalDistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VocalDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	roughness->setSampleRate(sampleRate);

	yin->setSampleRate(sampleRate);
	yin->setBufferSize(samplesPerBlock);

	highPass->setSampleRate(sampleRate);
	highPass->setBlockSize(samplesPerBlock);

	tmpCopyBuffer.setSize(2, samplesPerBlock);
}

void VocalDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VocalDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void VocalDistortionAudioProcessor::getParametersValues(){
	dryWet = dryWetParam->get();

	roughness->setSubHarmonics(subHarmonicsParam->get());
	roughness->setAmp(hAmpParam->get());

	highPass->setFrequency(highPassFrequencyParam->get());
}

void VocalDistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	getParametersValues();
	
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	const auto numSamples = buffer.getNumSamples();

	tmpCopyBuffer.copyFrom(0, 0, buffer, 0, 0, numSamples);
	tmpCopyBuffer.copyFrom(1, 0, buffer, 1, 0, numSamples);

	const auto pitch = yin->getPitch(buffer); // TODO how to treat stereo

	roughness->setFundamentalFrequency(pitch);
	roughness->process(buffer);

	// Substract original signal from the "mixed" one
	juce::FloatVectorOperations::subtract(buffer.getWritePointer(0), tmpCopyBuffer.getReadPointer(0), numSamples);
	juce::FloatVectorOperations::subtract(buffer.getWritePointer(1), tmpCopyBuffer.getReadPointer(1), numSamples);

	// Filter subharmonics with high pass
//	highPass->process(buffer);

	// Multiply subharmonics by dry wet parameter
	juce::FloatVectorOperations::multiply(buffer.getWritePointer(0), dryWet, numSamples);
	juce::FloatVectorOperations::multiply(buffer.getWritePointer(1), dryWet, numSamples);

	// Sum  original signal with high-pass filtered subharmonics
	juce::FloatVectorOperations::add(buffer.getWritePointer(0), tmpCopyBuffer.getReadPointer(0), numSamples);
	juce::FloatVectorOperations::add(buffer.getWritePointer(1), tmpCopyBuffer.getReadPointer(1), numSamples);

}

//==============================================================================
bool VocalDistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VocalDistortionAudioProcessor::createEditor()
{
    return new VocalDistortionAudioProcessorEditor (*this);
}

//==============================================================================
void VocalDistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VocalDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VocalDistortionAudioProcessor();
}
