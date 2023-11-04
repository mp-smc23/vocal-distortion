#pragma once
#include <JuceHeader.h>

class Roughness {
public:
	Roughness() = default;
	~Roughness() = default;

	void setAmp(const float newHAmp) { this->hAmp = newHAmp; }
	void setSampleRate(const float newSampleRate);
	void setFundamentalFrequency(const float newFrequency);
	void setSubHarmonics(const int newSubHarmonics);

	void process(juce::AudioBuffer<float>& buffer);

private:
	void update();

	float sampleRate{44100.f};

	float f0{440.f}; // fundamental frequency
	float hAmp{1.f}; // amplitude of cos

	int subHarmonics{1};

	float curPhase[5] = {0.f, 0.f, 0.f, 0.f, 0.f}; // current lfo phase
	float phaseStep[5] = {0.f, 0.f, 0.f, 0.f, 0.f}; // lfo step

	float k[5] = {2.f, 4.f, 8.f, 16.f, 32.f};
};
