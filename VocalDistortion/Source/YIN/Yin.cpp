#include "Yin.h"


float Yin::getPitch(const juce::AudioBuffer<float>& buffer)
{
	auto x = buffer.getReadPointer(0);
	auto sum = 0.f;
	auto minimumValue = 2.f;
	auto minTau = 0;

	//difference function
	dt[0] = 1;
	for(auto tau = 1; tau < bufferSize; tau++){
		auto tmp = 0.f;
		for(auto j = 1; j + tau < bufferSize; j++){
			auto sqr = (x[j]-x[j+tau]);
			tmp += sqr * sqr;
		}
		sum += tmp;
		dt[tau] = tmp / (sum / tau); // Mean Normalized Difference

		if(dt[tau] < minimumValue){
			minimumValue = dt[tau];
			minTau = tau;
			if(minimumValue < threshold) break;
		}
	}

	// TODO parabolic interpolation - see results
	// for now: no parabolic interpolation cause little effect when f0 small in comparisson to SR (voice is up to 3k)
	if(minTau > 0){
		currentPitch = sampleRate / minTau;
	}
	DBG("Current Pitch in Hz =");
	DBG(currentPitch);
	return currentPitch;
}
