#pragma once
#include <JuceHeader.h>

class Roughness {
public:
	Roughness() = default;
	~Roughness() = default;

	void setAmp(const float newHAmp) { this->hAmp = newHAmp; }
	void setSampleRate(const float newSampleRate);
	void setFundamentalFrequency(const float newFrequency);

	void process(juce::AudioBuffer<float>& buffer);

private:
	void update(){
		phaseStep = (f0 / k) / sampleRate ;
	}

	float sampleRate{44100.f};

	float f0{440.f}; // fundamental frequency
	float hAmp{1.f}; // amplitude of cos

	float curPhase{0.f}; // current lfo phase
	float phaseStep{0.f}; // lfo step
	
	/* TODO 
	 add params
		- number of harmonies
		- amplitude (H)
	 split into cpp and h files
	 */

	float k{2.f};
};
