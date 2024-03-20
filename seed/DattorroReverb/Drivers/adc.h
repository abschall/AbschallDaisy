
// Driver for ADC used for scanning pots.

#ifndef DRIVERS_ADC_H_
#define DRIVERS_ADC_H_

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisy::seed;

enum AdcChannel {
    ADC_CHANNEL_MIX,
    ADC_CHANNEL_DAMPEN,
    ADC_CHANNEL_DECAY,
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
        adcConfig[ADC_CHANNEL_MIX].InitSingle(A0);
        adcConfig[ADC_CHANNEL_DAMPEN].InitSingle(A1);
        adcConfig[ADC_CHANNEL_DECAY].InitSingle(A2);
        hardware.adc.Init(adcConfig, NUM_ADC_CHANNELS);
    }

private:
    AdcChannelConfig adcConfig[NUM_ADC_CHANNELS];
    DaisySeed& hardware;
};

#endif

