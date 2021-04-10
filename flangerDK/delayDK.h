#pragma once

#include <JuceHeader.h>

class delayDK {
public:
	//delay methods
	delayDK::delayDK();
	void prepareToPlay(double sampleRate);
	float delayProcess(float input, double feedback); //fill parameters with what we need for delay
	void setDprDpw(int delayReadPosition, int delayWritePosition);
	void setDelayTime(double newValue);
	void setFeedback(double newValue);
	void setDryWet(double newValue);

	//double delayTime;
	//double feedback;

private:
	//juce::AudioBuffer<float> delayBuffer;
	juce::Array<float> delayBuffer;


	int delayBufferLength;
	int dpr;
	int dpw;

	double delayTime;
	double feedback;
	double dryWet;



};