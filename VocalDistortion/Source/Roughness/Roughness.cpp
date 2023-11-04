#include "Roughness.h"

// y = xc * xm (where xc = input_signal)
// xm = 1 + h cos(ωmt)
void Roughness::process(juce::AudioBuffer<float>& buffer){
	auto* leftChannel = buffer.getWritePointer(0);
	auto* rightChannel = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : leftChannel;

	for(auto i = 0; i < buffer.getNumSamples(); i++){
		curPhase += phaseStep;
		if(curPhase > 1.f){
			curPhase -= 1.f;
		}

		auto xm = 1 + hAmp * cos(juce::MathConstants<float>::twoPi * curPhase);

		leftChannel[i] *= xm;
		rightChannel[i] *= xm;
	}

}

void Roughness::setFundamentalFrequency(const float newFrequency) {
	if(juce::approximatelyEqual(f0, newFrequency)) return;

	f0 = newFrequency;
	update();
}

void Roughness::setSampleRate(const float newSampleRate) {
	if(juce::approximatelyEqual(sampleRate, newSampleRate)) return;

	sampleRate = newSampleRate;
	update();
}
