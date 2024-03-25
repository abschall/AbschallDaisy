#include "daisy_seed.h"
#include "daisysp.h"
#include "DSP/DattorroPlateReverb.h"
#include "cv_scaler.h"
#include "patch.h"
#include "ui.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hardware;
CvScaler cvScaler(hardware);
DattorroPlateReverb reverb;
Ui ui(hardware);
Patch patch;

ReverbControlParameters controlParamsInit; 

float samplerate;
const int UINT_16_MAX = 65536;

void Init()
{
		// Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.

    hardware.Init();
    hardware.SetAudioBlockSize(128);
	samplerate = hardware.AudioSampleRate();

	ui.Init(&patch);
}

/// @brief prepares the DSP algorithm parametric parameters
/// @param samplerate 
void prepareToPlay(float samplerate)
{	
	reverb.reset(samplerate);
	reverb.setParameters(controlParamsInit);
	reverb.updateParameters(&patch);
}

void processAudioBlock(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	cvScaler.Read(&patch); // read pots ADC values
	ui.processUi();
	//reverb.reset(samplerate);
	reverb.updateParameters(&patch);
	vector<float> processedOut = {0.0f, 0.0f};
	vector<float> rawIn = {0.0f, 0.0f};
	
	//process audio block's samples 
	for (size_t i = 0; i < size; i++)
	{
		rawIn = {in[0][i], in[1][i]};
		processedOut =  reverb.processAudioSample(rawIn);
		//processedOut =  rawIn;
		out[0][i] = processedOut[0];
		out[1][i] = processedOut[1];    

	}
}

int main(void)
{
	Init();
	cvScaler.Init();

	prepareToPlay(samplerate);
	
	hardware.adc.Start(); 
	hardware.StartAudio(processAudioBlock);

	while(1) {}
}
