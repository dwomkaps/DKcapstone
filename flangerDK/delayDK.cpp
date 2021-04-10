#include "delayDK.h"


delayDK::delayDK() : delayBuffer(2, 1) {
	//initialize values
	//delayTime = 0.5;
	//feedback = 0.0;

	delayBufferLength = 1;
	

}

void delayDK::prepareToPlay(double sampleRate) {
	delayBufferLength = ((int)2.0 * sampleRate);
	if (delayBufferLength < 1)
		delayBufferLength = 1;

	delayBuffer.resize(delayBufferLength);
	delayBuffer.clear();
	//delayReadPosition = (int)(delayWritePosition - (delayTime * sampleRate) + delayBufferLength) % delayBufferLength; //this wont come up
}

float delayDK::delayProcess(float input, double feedback)
{
	//float delayData[] = delayBuffer;
	
	long rpi = (long)floor(delayBuffer[dpr]); //linear interpolation
	float wetSignal = delayBuffer[dpr] - (double)rpi; //read portion

	float toWrite = input + (wetSignal * feedback);
	delayBuffer.set(dpw, toWrite);


	if (++dpr >= delayBufferLength) //maybe these but possibly not try without first
		dpr = 0;
	if (++dpw >= delayBufferLength)
		dpw = 0;

	
	return wetSignal;
}

void delayDK::setDprDpw(int delayReadPosition, int delayWritePosition)
{
	dpr = delayReadPosition;
	dpw = delayWritePosition;
}

void delayDK::setDelayTime(double newValue)
{
	delayTime = newValue;
}

void delayDK::setFeedback(double newValue)
{
	feedback = newValue;
}

void delayDK::setDryWet(double newValue)
{
	dryWet = newValue;
}
