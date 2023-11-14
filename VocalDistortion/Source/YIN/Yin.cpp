#include "Yin.h"


float Yin::getPitch(const juce::AudioBuffer<float>& buffer)
{
	const auto x = buffer.getReadPointer(0);
	const auto bufferSize = buffer.getNumSamples();
	
	auto sum = 0.f;
	auto minimumValue = std::numeric_limits<float>::max();
	auto minTau = 1;

	
	dt[0] = 1; // the first value of dt' is 1 
	for(auto tau = 1; tau < bufferSize; tau++){
		auto diffFunc = 0.f;
		for(auto j = 1; j + tau < bufferSize; j++){
			const auto correlation = (x[j] - x[j+tau]); // STEP 1 - Correlation
			diffFunc += correlation * correlation; // STEP 2 - Difference Function
		}
		sum += diffFunc; // Summation from Mean Normalized Difference
		dt[tau] = diffFunc / (sum / static_cast<float>(tau)); // STEP 3 - Mean Normalized Difference
	}
	
	// STEP 4 - Absolute threshold
	for(auto tau = 1; tau < bufferSize; tau++){
		if(dt[tau] < minimumValue){ // found value lower then current minimum
			minimumValue = dt[tau]; // save the value
			minTau = tau;			// save the index

			// if we have found value lower than threshold and it is a local minimum we have found the answer -> break!
			if((tau == bufferSize - 1 || dt[tau] < dt[tau + 1]) && minimumValue < threshold) break;
		}
	}

	// STEP 5 - Parabolic Interpolation
	const auto result = parabolicInterpolation(dt, minTau, bufferSize);

	if(result > 0.f){ 
		currentPitch = sampleRate / result;
	}

	DBG("Current Pitch in Hz =");
	DBG(currentPitch);

	return currentPitch;
}

float Yin::parabolicInterpolation (const float* dt, const int index, const int dtSize)
{
	if(index == 0 || index == dtSize - 1) return static_cast<float>(index); // last or first element, can't do anything

    const float y0 = dt[index - 1];
	const float y1 = dt[index];
	const float y2 = dt[index + 1];

	// Caluclate parabolic interpolation (shift from middle point) according to equation
	// x = [(y0 - y2)/2]/(2*y1-y0-y2)
	// and add it to the index
	return static_cast<float>(index) - ((y0 - y2) * 0.5f) / (2.f * y1 - y0 - y2);
}


