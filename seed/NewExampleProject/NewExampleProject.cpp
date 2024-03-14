#include "daisy_seed.h"
#include "daisysp.h"
#include "../../dsp_fv/classicFilters.h"
using namespace daisy;
using namespace daisysp;

DaisySeed hardware;
ClassicFilters filter;
Switch filterSelectButton;

string filterState;
float samplerate;


/// @brief prepares the Hardware (pots, buttons, adc and dac)
void prepareHardware()
{
	// Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Configure();
    hardware.Init();
    hardware.SetAudioBlockSize(4);
	samplerate = hardware.AudioSampleRate();

	// Create an ADC configuration
    AdcChannelConfig adcConfig[2];
	//Add pin 21 as an analog input in this config. We'll use this to read the knob
	// set cut-off frequency and Q-factor  pots
	adcConfig[0].InitSingle(hardware.GetPin(21));
	adcConfig[1].InitSingle(hardware.GetPin(17));
    //set filter select button
	filterSelectButton.Init(hardware.GetPin(19),samplerate / 48.0f);
	hardware.adc.Init(adcConfig, 2);

}

/// @brief prepares the DSP algorithm parametric parameters
/// @param samplerate 

void processFilterSelectButton()
{
	static unsigned int buttonState = 0; 
	filterSelectButton.Debounce();

	if (filterSelectButton.RisingEdge())
	{
		++buttonState;
		if(buttonState == 0)
			filterState = "LPF1";
		else if(buttonState == 1)
			filterState = "LPF2";
		else if(buttonState == 2)
			filterState = "HPF1";
		else if(buttonState == 3)
			filterState = "HPF2";
		else
		{
			buttonState = 0;
			filterState = "LPF1";
		}
	}
}

void prepareToPlay(float samplerate)
{
	float initalFrequency = 10.0f;
	filter.setFilterType(filterState);
	filter.setCoefficients(initalFrequency,1.0,samplerate);
}


/// @brief processes the audio block
/// @param in 
/// @param out 
/// @param size 
void processAudioBlock(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	processFilterSelectButton();
	filter.setFilterType(filterState);
	filter.setCoefficients(hardware.adc.GetFloat(0)*20000 + 50 , hardware.adc.GetFloat(1)*20+0.5, samplerate);

	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = filter.processAudioSample( in[0][i]);
		out[1][i] = filter.processAudioSample( in[1][i]);
	}
}


int main(void)
{
	prepareHardware();
	prepareToPlay(samplerate);
	
	hardware.adc.Start();
	hardware.StartAudio(processAudioBlock);


	while(1) {}
}
