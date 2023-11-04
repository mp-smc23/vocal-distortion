#pragma once
#include <JuceHeader.h>


class HighPass
{
public:
	HighPass() = default;
	~HighPass() = default;

	void setFrequency(const float newFrequency);
	void setSampleRate(const double newSampleRate);
	void setBlockSize(const int newBlockSize);
	void process(juce::AudioBuffer<float>& buffer);

private:
	void update();

	using FilterBand = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
	FilterBand filter;

	juce::dsp::ProcessSpec processSpec{44100, 2084, 2};

	float frequency{1000.f};
	const float Q{0.707};
};


