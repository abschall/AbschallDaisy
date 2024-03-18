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
const int UINT_16_MAX = 65536;

enum AdcChannel {
    ADC_CHANNEL_FREQ_KNOB,
    ADC_CHANNEL_Q_KNOB,
    NUM_ADC_CHANNELS
}

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
    AdcChannelConfig adcConfig[NUM_ADC_CHANNELS];
	// set cut-off frequency and Q-factor  pots
    adcConfig[ADC_CHANNEL_FREQ_KNOB].InitSingle(A0);
    adcConfig[ADC_CHANNEL_Q_KNOB].InitSingle(A1);
    hardware.adc.Init(&adcConfig,NUM_ADC_CHANNELS);   

    //set filter select button
	filterSelectButton.Init(A2,samplerate / 48.0f);
	// Here set the rest of the hardware 

}

float rangeConversion(int x, float maxValue, float minValue, int numBits = UINT_16_MAX)
{
	auto increment = (maxValue - minValue);
	return minValue + x * increment;
}


/// @brief process the filterSelectButton's function, switches the filter Type when pressed 
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

/// @brief prepares the DSP algorithm parametric parameters
/// @param samplerate 
void prepareToPlay(float samplerate)
{
	float initalFrequency = rangeConversion(0, 20000.0f, 50.0f);
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
	filter.setCoefficients(rangeConversion(hardware.adc.GetFloat(ADC_CHANNEL_FREQ_KNOB), 20000.0f, 50.0f),
	 rangeConversion(hardware.adc.GetFloat(ADC_CHANNEL_Q_KNOB), 20000.0f, 50.0f),
	 samplerate);

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
