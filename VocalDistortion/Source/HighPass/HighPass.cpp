#include "HighPass.h"


void HighPass::setFrequency(const float newFrequency) {
	if(juce::approximatelyEqual(newFrequency, frequency)) return;

	this->frequency = newFrequency;
	*filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(processSpec.sampleRate, newFrequency, Q);

	update();
}

void HighPass::setSampleRate(const double newSampleRate){
	if(juce::approximatelyEqual(newSampleRate, processSpec.sampleRate)) return;
	processSpec.sampleRate = newSampleRate;
	update();
}

void HighPass::setBlockSize(const int newBlockSize){
	if(newBlockSize == processSpec.maximumBlockSize) return;
	processSpec.maximumBlockSize = newBlockSize;
	update();
}

void HighPass::process(juce::AudioBuffer<float>& buffer)
{
	juce::dsp::AudioBlock<float> block(buffer);
	filter.process(juce::dsp::ProcessContextReplacing<float>(block));
}


void HighPass::update()
{
	filter.prepare(processSpec);
}

