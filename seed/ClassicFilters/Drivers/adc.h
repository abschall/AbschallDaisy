
// Driver for ADC used for scanning pots.

#ifndef DRIVERS_ADC_H_
#define DRIVERS_ADC_H_

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisy::seed;

enum AdcChannel {
    ADC_CHANNEL_FREQ_KNOB,
    ADC_CHANNEL_q_KNOB,
    NUM_ADC_CHANNELS
};

class Adc
{
public:
    Adc(DaisySeed& h): hardware(h){ }
    ~Adc(){}

    /// @brief initializes the ADC channels specific for the filterProject 
    void Init()
    {
        adcConfig[ADC_CHANNEL_FREQ_KNOB].InitSingle(A0);
        adcConfig[ADC_CHANNEL_q_KNOB].InitSingle(A1);

        hardware.adc.Init(adcConfig, NUM_ADC_CHANNELS);
    }

private:
    AdcChannelConfig adcConfig[NUM_ADC_CHANNELS];
    DaisySeed& hardware;
};

#endif

