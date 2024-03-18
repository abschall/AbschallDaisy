#include "daisy_seed.h"
#include "daisysp.h"

#include "DSP/classicFilters.h"
#include "cv_scaler.h"
#include "patch.h"
#include "ui.h"

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

DaisySeed hardware;
CvScaler cvScaler(hardware);
ClassicFilters filter;
Switch filterSelectButton;
Ui ui(hardware);
Patch patch;
string filterState;
float samplerate;
const int UINT_16_MAX = 65536;

/// @brief DaisySeed hardware initialization  
void Init()
{
		// Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Configure();
    hardware.Init();
    hardware.SetAudioBlockSize(4);
	samplerate = hardware.AudioSampleRate();

	//set filter select button
	filterSelectButton.Init(A2,samplerate / 48.0f);
	// Here set the rest of the hardware 
	ui.Init(&patch);
}

/// @brief process the filterSelectButton's function, switches the filter Type when pressed 
// void processFilterSelectButton()
// {
// 	static unsigned int buttonState = 0; 
// 	filterSelectButton.Debounce();

// 	if (filterSelectButton.RisingEdge())
// 	{
// 		++buttonState;
// 		if(buttonState == 0)
// 			filterState = "LPF1";
// 		else if(buttonState == 1)
// 			filterState = "LPF2";
// 		else if(buttonState == 2)
// 			filterState = "HPF1";
// 		else if(buttonState == 3)
// 			filterState = "HPF2";
// 		else
// 		{
// 			buttonState = 0;
// 			filterState = "LPF1";
// 		}
// 	}
// }

/// @brief prepares the DSP algorithm parametric parameters
/// @param samplerate 
void prepareToPlay(float samplerate)
{
	float initalFrequency = 10.0f;
	filter.setFilterType((FilterType)TYPE_LPF1);
	filter.setCoefficients(initalFrequency,1.0,samplerate);
}

/// @brief processes the audio block
/// @param in 
/// @param out 
/// @param size 
void processAudioBlock(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	// processFilterSelectButton(); 
	// filter.setFilterType(filterState);
	cvScaler.Read(&patch); // read pots ADC values
	ui.processUi();
	filter.setFilterType(patch.filterType);
	filter.setCoefficients(patch.cutoffFreq,patch.qFactor,samplerate); // update filter  coefficients 
	
	//process audio block's samples 
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = filter.processAudioSample( in[0][i]);
		out[1][i] = filter.processAudioSample( in[1][i]);
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
