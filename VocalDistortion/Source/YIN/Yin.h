#pragma once
#include <JuceHeader.h>

class Yin
{

public:
	Yin () = default;
	
	~Yin() {
		if(dt) delete(dt);
	};

	Yin(const Yin&) = delete;
	Yin& operator=(const Yin&) = delete;

	float getPitch(const juce::AudioBuffer<float>& buffer);

	/// Should not be called from Audio Thread, allocating memory
	void setBufferSize(const unsigned int newBufferSize)
	{
		if(dt) delete(dt);

		dt = new float[newBufferSize]{};
		bufferSize = newBufferSize;
	}

	void setSampleRate(double newSampleRate)
	{
		sampleRate = newSampleRate;
	}

	void setThreshold(float newThreshold)
	{
		threshold = newThreshold;
	}

private:

	// YIN STEPS to think about
	//void bestLocalEstimate(const juce::AudioBuffer<float>& buffer);

	juce::AudioSampleBuffer differenceFunc;
	float* dt;
	float threshold{0.1f};

	unsigned int bufferSize{2048};
	double sampleRate{44100.f};

	float currentPitch{440.f};

	// Below functions should go in a seperate utilities class

	float parabolicInterpolation (const float *data, unsigned int pos) noexcept
	{
		float s0, s1, s2;
		unsigned int x0, x2;
		if (pos == 0 || pos == bufferSize - 1) return pos;
		x0 = (pos < 1) ? pos : pos - 1;
		x2 = (pos + 1 < bufferSize) ? pos + 1 : pos;
		if (x0 == pos) return (data[pos] <= data[x2]) ? pos : x2;
		if (x2 == pos) return (data[pos] <= data[x0]) ? pos : x0;
		s0 = data[x0];
		s1 = data[pos];
		s2 = data[x2];
		return pos + 0.5 * (s0 - s2 ) / (s0 - 2.* s1 + s2);
	}

};
