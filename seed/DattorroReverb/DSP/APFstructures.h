#pragma once

#include "lfo.h"		
#include "daisysp.h"
#include "circularBuffer.h"

using namespace daisy;
using namespace daisysp;


struct delayLineParameters
{
	float delayTime_ms = 0.0;
	bool enableDelay = false;
	float delayTime_samples = 0.0;
};

/// <summary>
/// simple delay Line, Z^(-D) 
/// </summary>
// class delayLine : private CircularBuffer<float>
// {
// public:
// 	/// <summary>
// /// sets the delay time in ms and enables/ disables  the comb filter 
// /// </summary>
// /// <param name="pParameters"></param>
// 	void setParameters(delayLineParameters pParameters)
// 	{
// 		parameters.delayTime_ms = pParameters.delayTime_ms;
// 		parameters.enableDelay = pParameters.enableDelay;
// 		parameters.delayTime_samples = (unsigned int) parameters.delayTime_ms * samplesPerMsec + 1;
// 	}
// 	/// <summary>
// 	/// creates the Delay Line's Delay Buffer (bufferLength = delay time),
// 	/// also sets the delay time in number of samples according to the sample rate 
// 	/// </summary>
// 	/// <param name="pSampleRate"></param>
// 	void createDelayBuffer(float pSampleRate) 
// 	{
// 		currentSampleRate = pSampleRate;
// 		samplesPerMsec = currentSampleRate / 1000.0;
// 		auto bufferLength = (unsigned int)(parameters.delayTime_ms * samplesPerMsec) + 1;
// 		parameters.delayTime_samples = bufferLength;
// 		// del.Init();
// 		// del.SetDelay(parameters.delayTime_samples);
// 		delayBuffer.createBuffer(bufferLength);
// 		delayBuffer.flush();

// 	}
// 	/// <summary>
// 	/// processes the incoming audio sample, output is full wet 
// 	/// </summary>
// 	/// <param name="inputXn"></param>
// 	/// <returns></returns>
// 	virtual float processAudioSample(float inputXn)
// 	{
// 		// full wet signal processing 

// 		if (parameters.enableDelay == true)
// 		{			
// 			// del.Write(inputXn);
// 			// return del.Read(parameters.delayTime_samples);
// 			delayBuffer.writeBuffer(inputXn);
// 			return  delayBuffer.readBuffer(parameters.delayTime_samples);
// 		}
// 		else
// 		{
// 			return inputXn;
// 		}
// 	}
// 	/// <summary>
// 	/// reads the delay Line at a specific sample time. Delay TIme should be given in samples,
// 	/// in order to avoid superfluous ms to sample conversion 
// 	/// </summary>
// 	/// <param name="pDelayTime_samples"></param>
// 	/// <returns></returns>
// 	float readDelayLine(unsigned int pDelayTime_samples)
// 	{	
// 		//return del.Read(pDelayTime_samples); // update this function with an unsigned int one
// 		//return delayBuffer.readBuffer((unsigned int)pDelayTime_samples);
// 		return delayBuffer.readBuffer( pDelayTime_samples);
// 	}
	
// protected:
// 	delayLineParameters parameters;
// 	float currentSampleRate;
// 	CircularBuffer delayBuffer;
// 	//DelayLine<float, 1024> del;
// 	float samplesPerMsec;
// };

class delayLine : private CircularBuffer<float>
{
public:
	/// <summary>
/// sets the delay time in ms and enables/ disables  the comb filter 
/// </summary>
/// <param name="pParameters"></param>
	void setParameters(delayLineParameters pParameters)
	{
		parameters.delayTime_ms = pParameters.delayTime_ms;
		parameters.enableDelay = pParameters.enableDelay;
		parameters.delayTime_samples = (unsigned int) parameters.delayTime_ms * samplesPerMsec + 1;
	}
	/// <summary>
	/// creates the Delay Line's Delay Buffer (bufferLength = delay time),
	/// also sets the delay time in number of samples according to the sample rate 
	/// </summary>
	/// <param name="pSampleRate"></param>
	void createDelayBuffer(float pSampleRate) 
	{
		currentSampleRate = pSampleRate;
		samplesPerMsec = currentSampleRate / 1000.0;
		auto bufferLength = (unsigned int)(parameters.delayTime_ms * samplesPerMsec) + 1;
		parameters.delayTime_samples = bufferLength;
		del.Init();
	}
	/// <summary>
	/// processes the incoming audio sample, output is full wet 
	/// </summary>
	/// <param name="inputXn"></param>
	/// <returns></returns>
	virtual float processAudioSample(float inputXn)
	{
		// full wet signal processing 
		if (parameters.enableDelay == true)
		{			
			del.Write(inputXn);
			return del.Read(parameters.delayTime_samples);
		}
		else
		{
			return inputXn;
		}
	}
	/// <summary>
	/// reads the delay Line at a specific sample time. Delay TIme should be given in samples,
	/// in order to avoid superfluous ms to sample conversion 
	/// </summary>
	/// <param name="pDelayTime_samples"></param>
	/// <returns></returns>
	float readDelayLine(unsigned int pDelayTime_samples)
	{	
		return del.Read(pDelayTime_samples); // update this function with an unsigned int one

	}
	
protected:
	delayLineParameters parameters;
	float currentSampleRate;
	DelayLine<float, 8192> del;
	float samplesPerMsec;
};


struct CombFilterParameters
{
	float delayTime_ms = 0.0;
	float feedbackGain = 0.0;
	bool enableComb = false;
	float delayTime_samples = 0.0;
};

/// <summary>
/// simple recirculating Comb Filter class, the comb filters have fixed delay times and buffer length
/// !!! This should be modified to a common max buffer length ! The class as is (3/08/2024) does not 
/// lend itself to delay time modulation 
/// </summary>
class CombFilter /*: private CircularBuffer<float>*/
{
public:
	/// <summary>
	/// sets the delay time in ms, feedback loop gain, and enables/ disables  the comb filter 
	/// </summary>
	/// <param name="pParameters"></param>
	void setParameters(CombFilterParameters pParameters)
	{
		parameters.delayTime_ms = pParameters.delayTime_ms;
		parameters.feedbackGain = pParameters.feedbackGain;
		parameters.enableComb = pParameters.enableComb;
		parameters.delayTime_samples = (unsigned int)parameters.delayTime_ms * samplesPerMsec + 1;
	}

	/// <summary>
	/// creates the Comb Filter's Delay Buffer (bufferLength = delay time),
	/// also sets the delay time in number of samples according to the sample rate 
	/// </summary>
	/// <param name="pSampleRate"></param>
	void createDelayBuffer(float pSampleRate)
	{
		currentSampleRate = pSampleRate;
		samplesPerMsec = currentSampleRate / 1000.0;
		//auto bufferLength = (unsigned int)(parameters.delayTime_ms * samplesPerMsec) + 1;
		parameters.delayTime_samples = (unsigned int) parameters.delayTime_ms * samplesPerMsec + 1;
		del.Init();
		//delayBuffer.createBuffer(bufferLength);
		// delayBuffer.flush();
	}

	/// <summary>
	/// reads the delay Line at a specific sample time. Delay TIme should be given in samples,
	/// in order to avoid superfluous ms to sample conversion 
	/// </summary>
	/// <param name="pDelayTime_samples"></param>
	/// <returns></returns>
	float readDelayLine(unsigned int pDelayTime_samples)
	{
		// return delayBuffer.readBuffer((unsigned int) pDelayTime_samples);
		return del.Read(pDelayTime_samples);
	}
	/// <summary>
	/// processes the incoming audio sample, output is full wet 
	/// </summary>
	/// <param name="inputXn"></param>
	/// <returns></returns>
	virtual float processAudioSample(float inputXn)
	{
		// full wet signal processing 

		if (parameters.enableComb == true)
		{
			//auto ynD = delayBuffer.readBuffer(parameters.delayTime_samples, true);
			float ynD = del.Read(parameters.delayTime_samples);
			
			float ynFullWet = inputXn + parameters.feedbackGain * ynD;
			del.Write(ynFullWet);
			//delayBuffer.writeBuffer(ynFullWet);

			return ynD;
		}
		else
			return inputXn;

	}
protected:
	CombFilterParameters parameters;
	float currentSampleRate;
	float samplesPerMsec;
	DelayLine<float, 4096> del;
	//CircularBuffer delayBuffer;
};

struct APFParameters
{
	float delayTime_ms = 0.0;
	float feedbackGain = 0.0;
	bool enableAPF = false;
	float delayTime_samples;
};

/// <summary>
/// APF class,as described in the MR Schroeder 1961 and 1962 reverberation papers
/// </summary>
class allPassFilter /*: private CircularBuffer<float>*/
{
public:
	/// <summary>
	/// sets the delay time in ms, feedback loop gain, and enables/ disables  the comb filter 
	/// </summary>
	/// <param name="pParameters"></param>
	void setParameters(APFParameters pParameters)
	{
		parameters.delayTime_ms = pParameters.delayTime_ms;
		parameters.feedbackGain = pParameters.feedbackGain;
		parameters.enableAPF = pParameters.enableAPF;
		parameters.delayTime_samples = (unsigned int)parameters.delayTime_ms * samplesPerMsec + 1;
	}
	/// <summary>
	/// creates the Comb Filter's Delay Buffer (bufferLength = delay time),
	/// also sets the delay time in number of samples according to the sample rate 
	/// </summary>
	/// <param name="pSampleRate"></param>
	virtual void createDelayBuffer(float pSampleRate)
	{
		currentSampleRate = pSampleRate;
		samplesPerMsec = currentSampleRate / 1000.0;
		//auto bufferLength = (unsigned int)(parameters.delayTime_ms * samplesPerMsec) + 1;
		parameters.delayTime_samples = (unsigned int)parameters.delayTime_ms * samplesPerMsec + 1;
		del.Init();
		// delayBuffer.createBuffer(bufferLength);
		// delayBuffer.flush();
	}
	/// <summary>
	/// reads the delay Line at a specific sample time. Delay TIme should be given in samples,
	/// in order to avoid superfluous ms to sample conversion 
	/// </summary>
	/// <param name="pDelayTime_samples"></param>
	/// <returns></returns>
	float readDelayLine(unsigned int pDelayTime_samples)
	{
		//return delayBuffer.readBuffer((unsigned int)pDelayTime_samples);
		return del.Read(pDelayTime_samples);
	}
	/// <summary>
	/// processes the incoming audio sample, output is full wet 
	/// </summary>
	/// <param name="inputXn"></param>
	/// <returns></returns>
	virtual float processAudioSample(float inputXn)
	{
		auto ynD = 0.0;
		if (parameters.enableAPF == true)
		{
			ynD = del.Read(parameters.delayTime_samples);
			del.Write(inputXn + ynD * parameters.feedbackGain);
			auto yn = (1 - parameters.feedbackGain * parameters.feedbackGain) * ynD + inputXn * (-parameters.feedbackGain);
			return yn;

			// ynD = delayBuffer.readBuffer(parameters.delayTime_samples);
			// delayBuffer.writeBuffer(inputXn + ynD * parameters.feedbackGain);
			// auto yn = (1 - parameters.feedbackGain * parameters.feedbackGain) * ynD + inputXn * (-parameters.feedbackGain);

			// return yn;
		}
		else
		{
			return inputXn;
		}
	}
protected:
	APFParameters parameters; //change parameters by apfParameters 
	float currentSampleRate;
	float samplesPerMsec;
	//CircularBuffer delayBuffer;
private:
	DelayLine<float, 2048> del;
};

/// <summary>
/// lattice APF structure / alternate APF as described by Jon Dattorro and Will Pirkle 
/// </summary>
class alternateAllPassFilter : public allPassFilter
{
public:
	float processAudioSample(float inputXn) override
	{
		if (parameters.enableAPF == true)
		{
			auto ynD = del.Read(parameters.delayTime_samples);
			auto temp = inputXn + ynD * parameters.feedbackGain;
			del.Write(temp);
			auto yn = -parameters.feedbackGain * temp + ynD;

			// auto ynD = delayBuffer.readBuffer(parameters.delayTime_samples, true);
			// auto temp = inputXn + ynD * parameters.feedbackGain;
			// delayBuffer.writeBuffer(temp);
			// auto yn = -parameters.feedbackGain * temp + ynD;

			return yn;
		}
		else
		{
			return inputXn;
		}


	}
private:
	DelayLine<float, 2096> del;
};

struct APF_modulationParameters
{
	float LFORate_Hz = 0.0;
	float excursion_ms = 0.0;
	bool enableLFO = true;
	float excursion_samples;
};

class alternateAllPassFilter_modulated : public allPassFilter
{
public:
	void reset(float pSampleRate)
	{
		currentSampleRate = pSampleRate;
		lfo.reset(currentSampleRate);
	}

	void setParameters(APFParameters pAPFparameters, APF_modulationParameters pApfModParameters)
	{
		allPassFilter::setParameters(pAPFparameters);
		// delayTime_ms needs to account for the excursion !!!!
		setModulationParameters(pApfModParameters);

		OscillatorParameters lfoParams = { generatorWaveform::kSin,apfModParameters.LFORate_Hz };
		lfo.setParameters(lfoParams);
	}

	void createDelayBuffer(float pSampleRate) override
	{
		currentSampleRate = pSampleRate;
		samplesPerMsec = currentSampleRate / 1000.0;
		//auto bufferLength = (unsigned int)((parameters.delayTime_ms + apfModParameters.excursion_ms) * samplesPerMsec + 1);
		parameters.delayTime_samples = parameters.delayTime_ms * samplesPerMsec;
		apfModParameters.excursion_samples = apfModParameters.excursion_ms * samplesPerMsec;
		del.Init();

		// delayBuffer.createBuffer(bufferLength);
		// delayBuffer.flush();
	}

	float processAudioSample(float inputXn) override
	{
		if (parameters.enableAPF == true)
		{
			float modValue = 0.0;

			if(apfModParameters.enableLFO == true)
			{ 
				auto outLfo = lfo.renderAudioOuput();
				auto modValue = outLfo.normalOutput * apfModParameters.excursion_samples;
			}

			auto ynD = del.Read(parameters.delayTime_samples + modValue);
			auto temp = inputXn + parameters.feedbackGain * ynD;
			del.Write(temp);
			auto yn = -parameters.feedbackGain * temp + ynD;

			// auto ynD = delayBuffer.readBuffer(parameters.delayTime_samples + modValue,true);
			// auto temp = inputXn + parameters.feedbackGain * ynD;
			// delayBuffer.writeBuffer(temp);
			// auto yn = -parameters.feedbackGain * temp + ynD;

			return yn;
		}
		else
		{
			return inputXn;
		}
	}

protected:
	APF_modulationParameters apfModParameters;
	LFO lfo;
	

private:
	void setModulationParameters(APF_modulationParameters pApfModParameters)
	{
		// to be called somewhere !!!
		apfModParameters.enableLFO = pApfModParameters.enableLFO;
		apfModParameters.excursion_ms = pApfModParameters.excursion_ms;
		apfModParameters.LFORate_Hz = pApfModParameters.LFORate_Hz;
	}
	DelayLine<float, 1024> del;
};

