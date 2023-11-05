#include "Roughness.h"

// y = xc * xm (where xc = input_signal)
// xm = 1 + h cos(ωmt)
void Roughness::process(juce::AudioBuffer<float>& buffer){
	auto* leftChannel = buffer.getWritePointer(0);
	auto* rightChannel = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : leftChannel;

	for(auto i = 0; i < buffer.getNumSamples(); i++){	
		f0 = f0Smoothing.getNextValue();
		update();

		auto xm = 1;

		for(auto j = 0; j < subHarmonics; j++){
			curPhase[j] += phaseStep[j];
			if(curPhase[j] > 1.f){
				curPhase[j] -= 1.f;
			}
			xm += hAmp * h[j] * cos(juce::MathConstants<float>::twoPi * curPhase[j]);
		}

		leftChannel[i] *= xm;
		rightChannel[i] *= xm;
	}

}

void Roughness::setFundamentalFrequency(const float newFrequency) {
	if(juce::approximatelyEqual(f0, newFrequency)) return;
	f0Smoothing.setTargetValue(newFrequency);
}

void Roughness::setSampleRate(const float newSampleRate) {
	if(juce::approximatelyEqual(sampleRate, newSampleRate)) return;

	f0Smoothing.reset(newSampleRate, rampLengthInSeconds);

	sampleRate = newSampleRate;
	update();
}

void Roughness::setSubHarmonics(const int newSubHarmonics) {
	subHarmonics = newSubHarmonics;
	update();
}

void Roughness::update(){
	for(auto i = 0; i < subHarmonics; i++){
		phaseStep[i] = (f0 / k[i]) / sampleRate;
	}
}
