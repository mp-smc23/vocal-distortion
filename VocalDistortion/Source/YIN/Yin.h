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

	void setSampleRate(const double newSampleRate)
	{
		sampleRate = static_cast<float>(newSampleRate);
	}

	void setThreshold(const float newThreshold)
	{
		threshold = newThreshold;
	}

private:
	float* dt;
	float threshold{0.1f};
	float currentPitch{440.f};

	unsigned int bufferSize{2048};
	float sampleRate{44100.f};


	static float parabolicInterpolation (const float* dt, const int index, const int dtSize);
};
