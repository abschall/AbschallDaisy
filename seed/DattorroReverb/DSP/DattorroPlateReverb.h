#pragma once
#include "APFstructures.h"
#include "classicFilters.h"
#include "../patch.h"
struct ReverbControlParameters
{
	float mix = 1.0f;
	float predelay = 50.0f;
	float inputDiffusion1 = 0.75f;
	float inputDiffusion2 = 0.625f;

	float decayDiffusion1 = 0.7f;
	float decayDiffusion2 = 0.5f;
	float decay = 1.0f;

	float damping = 15000.0f;
	float bandwidth = 8000.0f;
};

struct ReverbStructureParameters
{
	// predelay
	delayLineParameters predelayParam = { 50.0, true };

	// input diffusers
	APFParameters inputDiffuser1Param = { 4.93, -0.75, true };
	APFParameters inputDiffuser2Param = { 3.6, -0.75, true };
	APFParameters inputDiffuser3Param = { 12.7, -0.625, true };
	APFParameters inputDiffuser4Param = { 9.3, -0.625, true };
	
	//modulated APF
	APF_modulationParameters modulatedAPF1_lfoParam = { 1, 0.536, true };
	APF_modulationParameters modulatedAPF2_lfoParam = { 1, 0.536, true };
	APFParameters modulatedAPF1Param = { 22.55, 0.7, true };
	APFParameters modulatedAPF2Param = { 30.46, 0.7, true };

	// simple delay lines
	delayLineParameters delayLine1Param = { 149.4,true };
	delayLineParameters delayLine2Param = { 124.8,true };
	delayLineParameters delayLine3Param = { 141.5,true };
	delayLineParameters delayLine4Param = { 105.2,true };

	// simple alternate APF
	APFParameters alternateAPF5Param = { 60.4, 0.50, true };
	APFParameters alternateAPF6Param = { 89.12, 0.50, true };

};

class DattorroPlateReverb
{
public:
	void setParameters(ReverbControlParameters pControlParameters)
	{
		controlParameters.predelay = pControlParameters.predelay;
		controlParameters.mix = pControlParameters.mix;
		controlParameters.inputDiffusion1 = pControlParameters.inputDiffusion1;
		controlParameters.inputDiffusion2 = pControlParameters.inputDiffusion2;

		controlParameters.decayDiffusion1 = pControlParameters.decayDiffusion1;
		controlParameters.decayDiffusion2 = pControlParameters.decayDiffusion2;
		controlParameters.decay = pControlParameters.decay;

		controlParameters.damping = pControlParameters.damping;
		controlParameters.bandwidth = pControlParameters.bandwidth;

		// add additionnal control parameters here 
	}

	void updateParameters( Patch *patch)
	{
		// mix
		controlParameters.mix = patch->mix;

		// dampen 
		controlParameters.damping = patch->dampen;

		// decay
		controlParameters.decay = patch->decay;

		dampingLPF1.setCoefficients(controlParameters.damping, 1.0, sampleRate);
		dampingLPF2.setCoefficients(controlParameters.damping, 1.0, sampleRate);
	}
	void updateParameters(ReverbControlParameters pControlParameters)
	{
		modulatedAPF1.reset(sampleRate);
		modulatedAPF2.reset(sampleRate);

		setParameters(pControlParameters);

		// update predelay delay time
		structureParameters.predelayParam.delayTime_ms = controlParameters.predelay;
		predelayLine.setParameters(structureParameters.predelayParam);

		// update inputDiffusers inputDiffusion 1 and 2
		structureParameters.inputDiffuser1Param.feedbackGain = -controlParameters.inputDiffusion1;
		structureParameters.inputDiffuser2Param.feedbackGain = -controlParameters.inputDiffusion1;
		structureParameters.inputDiffuser3Param.feedbackGain = -controlParameters.inputDiffusion2;
		structureParameters.inputDiffuser4Param.feedbackGain = -controlParameters.inputDiffusion2;
		
		inputDiffuser1.setParameters(structureParameters.inputDiffuser1Param);
		inputDiffuser2.setParameters(structureParameters.inputDiffuser2Param);
		inputDiffuser3.setParameters(structureParameters.inputDiffuser3Param);
		inputDiffuser4.setParameters(structureParameters.inputDiffuser4Param);
		// update decayDiffusion1 and 2
		structureParameters.modulatedAPF1Param.feedbackGain = controlParameters.decayDiffusion1;
		structureParameters.modulatedAPF2Param.feedbackGain = controlParameters.decayDiffusion1;
		structureParameters.alternateAPF5Param.feedbackGain = controlParameters.decayDiffusion2;
		structureParameters.alternateAPF6Param.feedbackGain = controlParameters.decayDiffusion2;
		
		modulatedAPF1.setParameters(structureParameters.modulatedAPF1Param,structureParameters.modulatedAPF1_lfoParam);
		modulatedAPF2.setParameters(structureParameters.modulatedAPF2Param,structureParameters.modulatedAPF2_lfoParam);
		alternateAPF5.setParameters(structureParameters.alternateAPF5Param);
		alternateAPF6.setParameters(structureParameters.alternateAPF6Param);
		// update bandwidth and damping 
		bandwidthLPF.setCoefficients(controlParameters.bandwidth, 1.0, sampleRate);
		dampingLPF1.setCoefficients(controlParameters.damping, 1.0, sampleRate);
		dampingLPF2.setCoefficients(controlParameters.damping, 1.0, sampleRate);
	}

	void reset(float pSampleRate)
	{
		sampleRate = pSampleRate;

		// reset LFOs
		modulatedAPF1.reset(sampleRate); 
		modulatedAPF2.reset(sampleRate);

		// set parameters
		predelayLine.setParameters(structureParameters.predelayParam);
		modulatedAPF1.setParameters(structureParameters.modulatedAPF1Param, structureParameters.modulatedAPF1_lfoParam);
		modulatedAPF2.setParameters(structureParameters.modulatedAPF2Param, structureParameters.modulatedAPF2_lfoParam);

		inputDiffuser1.setParameters(structureParameters.inputDiffuser1Param);
		inputDiffuser2.setParameters(structureParameters.inputDiffuser2Param);
		inputDiffuser3.setParameters(structureParameters.inputDiffuser3Param);
		inputDiffuser4.setParameters(structureParameters.inputDiffuser4Param);
		
		delayLine1.setParameters(structureParameters.delayLine1Param);
		delayLine2.setParameters(structureParameters.delayLine2Param);
		delayLine3.setParameters(structureParameters.delayLine3Param);
		delayLine4.setParameters(structureParameters.delayLine4Param);
		alternateAPF5.setParameters(structureParameters.alternateAPF5Param);
		alternateAPF6.setParameters(structureParameters.alternateAPF6Param);

		bandwidthLPF.setFilterType(TYPE_LPF1);
		dampingLPF1.setFilterType(TYPE_LPF1);
		dampingLPF2.setFilterType(TYPE_LPF1);
		bandwidthLPF.	setCoefficients(8000.0, 1.0, sampleRate);
		dampingLPF1.	setCoefficients(10000.0, 1.0, sampleRate);
		dampingLPF2.	setCoefficients(10000.0, 1.0, sampleRate);


		// create buffers
		predelayLine.createDelayBuffer(sampleRate);
		modulatedAPF1.createDelayBuffer(sampleRate);
		modulatedAPF2.createDelayBuffer(sampleRate);

		inputDiffuser1.createDelayBuffer(sampleRate);
		inputDiffuser2.createDelayBuffer(sampleRate);
		inputDiffuser3.createDelayBuffer(sampleRate);
		inputDiffuser4.createDelayBuffer(sampleRate);

		delayLine1.createDelayBuffer(sampleRate);
		delayLine2.createDelayBuffer(sampleRate);
		delayLine3.createDelayBuffer(sampleRate);
		delayLine4.createDelayBuffer(sampleRate);

		alternateAPF5.createDelayBuffer(sampleRate);
		alternateAPF6.createDelayBuffer(sampleRate);
	}

	vector<float> processAudioSample(vector<float> inputXn)
	{

		float input = 0.5f * (float)(inputXn[0] + inputXn[1]);
		float output = 0.0f;
		static float tank1_wet = 0.0f, tank2_wet = 0.0f;

		output = predelayLine.processAudioSample(input);

		output = bandwidthLPF.	processAudioSample(input);

		output = inputDiffuser1.processAudioSample(output);
		output = inputDiffuser2.processAudioSample(output);
		output = inputDiffuser3.processAudioSample(output);
		output = inputDiffuser4.processAudioSample(output);


		//delay needs to be read !
		//tank1_wet = delayLine2.readDelayLine(4640);
		//tank2_wet = delayLine4.readDelayLine(5503);
		float tank1 = output + tank2_wet;
		float tank2 = output + tank1_wet;

		// process tank 1
		tank1 = modulatedAPF1.	processAudioSample(tank1);
		tank1 = delayLine1.		processAudioSample(tank1);
		tank1 = dampingLPF1.	processAudioSample(tank1);
		tank1 = alternateAPF5.	processAudioSample(tank1);
		tank1_wet = delayLine2.	processAudioSample(tank1) * controlParameters.decay;

		// process tank 2
		tank2 = modulatedAPF2.	processAudioSample(tank2);
		tank2 = delayLine3.		processAudioSample(tank2);
		tank2 = dampingLPF2.	processAudioSample(tank2);
		tank2 = alternateAPF6.	processAudioSample(tank2);
		tank2_wet = delayLine4.	processAudioSample(tank2) * controlParameters.decay;


		vector<float> outputYn = readOutputTaps();

		outputYn[0] = (1 - controlParameters.mix) * inputXn[0] + (controlParameters.mix) * outputYn[0];
		outputYn[1] = (1 - controlParameters.mix) * inputXn[1] + (controlParameters.mix) * outputYn[1];
		return outputYn;
	}

private:
	vector<float> readOutputTaps()
	{
		float yL = delayLine1.readDelayLine(429);
		yL += delayLine1.readDelayLine(4790);
		yL -= alternateAPF5.readDelayLine(3081);
		yL += delayLine2.readDelayLine(3213);
		yL -= delayLine3.readDelayLine(3205);
		yL -= alternateAPF6.readDelayLine(301);
		yL -= delayLine4.readDelayLine(1717);

		float yR = delayLine3.readDelayLine(568);
		yR += delayLine3.readDelayLine(5842);
		yR -= alternateAPF6.readDelayLine(1977);
		yR += delayLine4.readDelayLine(4305);
		yR -= delayLine1.readDelayLine(3400);
		yR -= alternateAPF5.readDelayLine(540);
		yR -= delayLine2.readDelayLine(192);

		vector<float> output = { yL, yR };

		return output;
	}

	float sampleRate;
	ReverbControlParameters controlParameters;
	ReverbStructureParameters structureParameters;

	alternateAllPassFilter inputDiffuser1, inputDiffuser2, inputDiffuser3, inputDiffuser4;
	alternateAllPassFilter_modulated  modulatedAPF1, modulatedAPF2;
	delayLine predelayLine;
	delayLine delayLine1, delayLine2, delayLine3, delayLine4;
	alternateAllPassFilter alternateAPF5, alternateAPF6;
	ClassicFilters bandwidthLPF, dampingLPF1, dampingLPF2;
};