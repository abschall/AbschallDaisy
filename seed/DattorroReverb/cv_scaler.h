
// Scaling ADC input values and input calibration (if any)
#ifndef CV_SCALER_H_
#define CV_SCALER_H_

#include "Drivers/adc.h"
#include "patch.h"

#include "daisy_seed.h"
#include "daisysp.h"

using namespace std;
using namespace daisy;

enum Law {
    LAW_QUADRATIC,
    LAW_DECIBELS,
    LAW_LINEAR,
    LAW_QUADRATIC_BIPOLAR,
    LAW_QUARTIC_BIPOLAR
};

struct ChannelSettings{
    Law law;
    vector<float> range;
    bool bipolar;
};

class CvScaler
{
public:

    CvScaler(DaisySeed& h) : hardware(h), adc_(h) {
     }
    ~CvScaler() { }
    
    void Init()
    {
        adc_.Init();
        for (size_t i = 0; i < NUM_ADC_CHANNELS; ++i)
            {
                analogControl_[i].Init(hardware.adc.GetPtr(i), hardware.AudioSampleRate());
            }
    }

    /// @brief convert 0.0f - 1.0f range to a desired float range, specified by minValue and maxValue
    /// @param x 
    /// @param minValue 
    /// @param maxValue 
    /// @return 
    float rangeConversion(float x, float minValue, float maxValue)
    {
        auto increment = (maxValue - minValue);
        return (minValue + x * increment);
    }

    /// @brief processes the adc values from VC and pots to the desired range 
    /// @param patch 
    void Read(Patch* patch)
    {
        for (size_t i = 0; i < NUM_ADC_CHANNELS; ++i) 
        {
            ChannelSettings& settings = channel_settings_[i];
            float value = analogControl_[i].Process();

            switch (settings.law) {
            case LAW_QUADRATIC:
                {
                    value = value * value * value * value;
                }
                break;
            case LAW_DECIBELS:
                {
                    value = daisysp::pow10f(2*(value -1));
                }
                break;
            case LAW_QUADRATIC_BIPOLAR:
                {
                value = value - 0.5f;
                float value2 = value * value * 4.0f * 3.3f;
                value = value < 0.0f ? -value2 : value2;
                }
                break;

            case LAW_QUARTIC_BIPOLAR:
                {
                value = value - 0.5f;
                float value2 = value * value * 4.0f;
                float value4 = value2 * value2 * 3.3f;
                value = value < 0.0f ? -value4 : value4;
                }
                break;

            default:
                break;
            }

            // apply value according to settings
            rangedValues[i] = rangeConversion(value,settings.range[0], settings.range[1]); // do some filtering here, rather than assigning the value directly

            
        }

        patch->mix = rangedValues[ADC_CHANNEL_MIX];
        patch->dampen = rangedValues[ADC_CHANNEL_DAMPEN];
        patch->decay = rangedValues[ADC_CHANNEL_DECAY];
    }

private:
    DaisySeed &hardware;
    Adc adc_;
    float rangedValues[NUM_ADC_CHANNELS]; // filtered adc values
    //static ChannelSettings channel_settings_[NUM_ADC_CHANNELS];
    AnalogControl analogControl_[NUM_ADC_CHANNELS];
    ChannelSettings channel_settings_[NUM_ADC_CHANNELS] =
    {

    {LAW_LINEAR, {0.0f, 1.0f}, false},   // ADC_CHANNEL_MIX
    {LAW_LINEAR, {10.0f,15000.0f},  false},   // ADC_CHANNEL_DAMPEN
    {LAW_LINEAR, {0.0f, 1.0f},  false},   // ADC_CHANNEL_DECAY
    };

};


#endif