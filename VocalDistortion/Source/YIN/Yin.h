#pragma once
#include <JuceHeader.h>

class Yin
{

public:
	Yin () = default;
	
	~Yin() {
        delete(dt);
	};

	Yin(const Yin&) = delete;
	Yin& operator=(const Yin&) = delete;

	float getPitch(const juce::AudioBuffer<float>& buffer);

	/// Should not be called from Audio Thread, allocating memory
	void setBufferSize(const unsigned int newBufferSize)
	{
        delete(dt);

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
	float* dt; // buffer for holding dt'(tau) values
	float threshold{0.1f}; // threshold value for step no 4
	float currentPitch{440.f}; // variable for storing found pitch

	unsigned int bufferSize{2048}; 
	float sampleRate{44100.f};	


	static float parabolicInterpolation (const float* dt, const int index, const int dtSize);
};
