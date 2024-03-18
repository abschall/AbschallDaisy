// Driver for Leds used for scanning pots.

#ifndef DRIVERS_LEDS_H_
#define DRIVERS_LEDS_H_

#include "daisy_seed.h"
#include "daisysp.h"
#include "../../stmlib/stmlib.h"
using namespace daisy;
using namespace daisy::seed;

enum Led
{
    SWITCH_MODE_LED,
    CUTOFF_LED,
    NUMB_LEDS
};

class Leds
{
public:
    Leds(DaisySeed& h): hardware(h){}
    ~Leds(){}

    void Init()
    {
        // led[SWITCH_MODE_LED].Init(D26, false);
        // led[CUTOFF_LED].Init(D27, false);
        // led[SWITCH_MODE_LED].Init(D26,GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);
        // led[CUTOFF_LED].Init(D27,GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);

        // switchModeLED.Init(D18,D19,D20,false);
        // cutoffLED.Init(D26, false);
        led[0].Init(D19, GPIO::Mode::OUTPUT);
        led[1].Init(D20, GPIO::Mode::OUTPUT);
    }

    void LedWrite(unsigned int counter)
    {
        if (counter == 0)
        {
            led[0].Write(false);
            led[1].Write(false);
        }
        if (counter == 1)
        {
            led[0].Write(true);
            led[1].Write(false);
        }
        if (counter == 2)
        {
            led[0].Write(false);
            led[1].Write(true);
        }
        if (counter == 3)
        {
            led[0].Write(true);
            led[1].Write(true);
        }
    }
private:
    DaisySeed& hardware;
   // daisy::Led led[NUMB_LEDS];

    unsigned int number_leds = 2;
    daisy::GPIO led[NUMB_LEDS];
    // daisy::RgbLed switchModeLED;
    // daisy::Led cutoffLED;
};
#endif 