
#ifndef DRIVERS_CV_SCALER_H_
#define DRIVERS_CV_SCALER_H_

#include "adc.h"
#include "daisy_seed.h"
#include "daisysp.h"
#include "../patch.h"
#include <vector>

using namespace std;
// Scaling ADC input values and input calibration (if any)

enum Law {
  LAW_LINEAR,
  LAW_QUADRATIC_BIPOLAR,
  LAW_QUARTIC_BIPOLAR
};

struct ChannelSettings{
    Law law;
    vector<float> range;
    bool bipolar;
}
class CvScaler
{
public:
    CvScaler() { }
    ~CvScaler() { }
    
    void Init()
    {
        adc_.Init();
        for (size_t i = 0; i < NUM_ADC_CHANNELS; ++i)
        {
            analogControl_[i].Init(adc_.hardware.adc.GetPtr(i), adc_.hardware.AudioSampleRate);
            if (channel_settings_[i].bipolar == true)
            {
                analogControl_[i].InitBipolarCv(adc_.hardware.adc.GetPtr(i),adc_.hardware.AudioSampleRate);
            }
        }
    }

    void rangeConversion(int x, float maxValue, float minValue)
    {
        auto increment = (maxValue - minValue);
        return minValue + x * increment;
    }

    void Read(Patch* patch)
    // Process all CVs / pots
    {
        for (size_t i = 0; i < NUM_ADC_CHANNELS; ++i) 
        {
            ChannelSettings& settings = channel_settings_[i];
            float value = analogControl_[i].Process();
            //float value = analogControl_[i].GetRawFloat();

            switch (settings.law) {
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
        
        patch->cutoffFreq = rangedValues[ADC_CHANNEL_FREQ_KNOB];
        patch->qFactor = rangedValues[ADC_CHANNEL_q_KNOB];

        // !!! maybe advance / read ADC value for next function call !!!

    }

private:
    Adc adc_;
    float rangedValues[NUM_ADC_CHANNELS]; // filtered adc values
    DaisySeed hardware;
    static ChannelSettings channel_settings_[NUM_ADC_CHANNELS];
    AnalogControl analogControl_[NUM_ADC_CHANNELS];
}

ChannelSettings CvScaler::channel_settings_[NUM_ADC_CHANNELS] =
{
    {LAW_QUADRATIC_BIPOLAR, {50, 20000}, false};    // ADC_CHANNEL_FREQ_KNOB
    {LAW_QUADRATIC_BIPOLAR, {0.01, 30},  false};     // ADC_CHANNEL_q_KNOB
}

#endif