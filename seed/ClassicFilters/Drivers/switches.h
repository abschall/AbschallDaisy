// Driver for scanning switches 

#ifndef DRIVERS_SWITCHES_H
#define DRIVERS_SWITCHES_H

#include "daisy_seed.h"
#include "daisysp.h"
using namespace daisy;
using namespace daisy::seed;

enum SwitchEnum{
    SWITCH_MODE_SELECT,
    NUM_SWITCHES
};

class Switches
{
public:
    Switches(DaisySeed& h): hardware(h){}
    ~Switches(){}
    void Init()
    {
       switches[SWITCH_MODE_SELECT].Init(A2, hardware.AudioSampleRate() / 48.0f);
    }

    /// @brief debounce the switches 
    void Debounce()
    {
        for (auto i = 0; i < NUM_SWITCHES ; ++i)
            switches[i].Debounce();

    }
    
    /// @brief check if desired switch was pressed 
    /// @param s 
    /// @return 
    unsigned int justPressed(SwitchEnum s)
    {
        return (switches[s].RisingEdge() == true);
    }

private:
    DaisySeed& hardware;
    unsigned int switchStates[NUM_SWITCHES];
    daisy::Switch switches[NUM_SWITCHES];
};

#endif