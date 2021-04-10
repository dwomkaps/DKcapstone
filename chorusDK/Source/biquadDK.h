#pragma once
#include <math.h>
#include <JuceHeader.h>

class biquadDK {
private:
	juce::Array<float> inSamples;
	juce::Array<float> outSamples;

	double cutoff;
	double q;
	double sampleRate;

public:
	biquadDK::biquadDK() {
		inSamples.resize(3);
		outSamples.resize(2);

		inSamples.fill(0);
		outSamples.fill(0);

		cutoff = 0;
		q = 0;
		sampleRate = 0;
		
	}

	void setCutoff(double newValue) {
		cutoff = newValue;
	}

	void setQ(double newValue) {
		q = newValue;
	}

	void setSampleRate(double newValue) {
		sampleRate = newValue;
	}

	double getCutoff() {
		return cutoff;
	}

	double getQ() {
		return q;
	}

	void push(float input) {
		inSamples.set(0, input);
	}

	//cycle samples through the arrays
	void cycle(float input, float output) {
		for (int i = inSamples.size(); i > 0; i--) {
			inSamples.set(i, inSamples[i - 1]);
		}
		//inSamples.set(0, input);

		for (int j = outSamples.size(); j > 0; j--) {
			outSamples.set(j, outSamples[j - 1]);
		}
		outSamples.set(0, output);
	}

	void prepareToPlay() {
		inSamples.fill(0);
		outSamples.fill(0);
	}

	double lowpass(float input){
		double output, a0, a1, a2, b0, b1, b2, w, alpha;
		double pi = 3.141592653589793238;
		w = (2 * pi) * (cutoff / sampleRate);
		alpha = (sin(w)) / 2 * q;
		a0 = 1 + alpha;
		a1 = -2 * cos(w);
		a2 = 1 - alpha;
		b0 = (1 - cos(w)) / 2;
		b1 = 1 - cos(w);
		b2 = (1 - cos(w)) / 2;

		push(input);

		output = (b0 / a0) * inSamples[0] + (b1 / a0) * inSamples[1] + (b2 / a0) * inSamples[2]
			- (a1 / a0) * outSamples[0] - (a2 / a0) * outSamples[1];

		cycle(input, output);

		return output;
	}

};