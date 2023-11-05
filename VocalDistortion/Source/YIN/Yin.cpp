#include "Yin.h"


float Yin::getPitch(const juce::AudioBuffer<float>& buffer)
{
	const auto x = buffer.getReadPointer(0);
	const auto bufferSize = buffer.getNumSamples();
	
	auto sum = 0.f;
	auto minimumValue = MAXFLOAT;
	auto minTau = 1;

	// Difference function
	dt[0] = 1;
	for(auto tau = 1; tau < bufferSize; tau++){
		auto correlation = 0.f;
		for(auto j = 1; j + tau < bufferSize; j++){
			const auto difference = (x[j] - x[j+tau]);
			correlation += difference * difference;
		}
		sum += correlation;
		dt[tau] = correlation / (sum / tau); // Mean Normalized Difference
	}

	// Absolute threshold
	for(auto tau = 1; tau < bufferSize; tau++){
		if(dt[tau] < minimumValue){
			minimumValue = dt[tau];
			minTau = tau;
			if((tau == bufferSize - 1 || dt[tau] < dt[tau + 1]) && minimumValue < threshold) break;
		}
	}

	// Parabolic Interpolation
	auto result = parabolicInterpolation(dt, minTau, bufferSize);

	if(result > 0.f){
		currentPitch = sampleRate / result;
	}

	DBG("Current Pitch in Hz =");
	DBG(currentPitch);

	return currentPitch;
}

float Yin::parabolicInterpolation (const float* dt, const int index, const int dtSize)
{
	if(index == 0 || index == dtSize - 1) return index; // last or first element, can't do anything

	// Get values for the nearest 3 points
	float y0, y1, y2;
	y0 = dt[index - 1];
	y1 = dt[index];
	y2 = dt[index + 1];

	// Caluclate parabolic interpolation (shift from middle point) according to equation
	// x = [(y0 - y2)/2]/(2*y1-y0-y2)
	// and add it to the index
	return static_cast<float>(index) - ((y0 - y2) * 0.5f) / (2.f * y1 - y0 - y2);
}


