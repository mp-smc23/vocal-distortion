#pragma once
#include <JuceHeader.h>

class Roughness {
public:
	Roughness() = default;
	~Roughness() = default;

	void setAmp(const float newHAmp) { this->hAmp = newHAmp; }
	
	void setSampleRate(const float newSampleRate) {
		if(juce::approximatelyEqual(sampleRate, newSampleRate)) return;

		sampleRate = newSampleRate;
		update();
	}

	void setFundamentalFrequency(const float newFrequency) {
		if(juce::approximatelyEqual(f0, newFrequency)) return;

		f0 = newFrequency;
		update();
	}

	// y = xc * xm (where xc = input_signal)
	// xm = 1 + h cos(ωmt)
	void process(juce::AudioBuffer<float>& buffer){
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

private:
	void update(){
		phaseStep = (f0 / k) / sampleRate ;
	}

	float sampleRate{44100.f};

	float f0{440.f}; // fundamental frequency
	float hAmp{1.f}; // amplitude of cos

	float curPhase{0.f}; // current lfo phase
	float phaseStep{0.f}; // lfo step

	float k{2.f};
};
