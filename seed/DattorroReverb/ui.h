// User Interface 
#ifndef UI_H_
#define UI_H_

// #include "Drivers/leds.h"
// #include "Drivers/switches.h"
#include "patch.h"
#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class Ui
{
public:
    Ui(DaisySeed& h) : hardware(h)/*,leds_(h),switches_(h) */{}
    ~Ui(){}

    void Init(Patch *patch)
    {
        patch_ = patch; 
    }

    void state()
    {

    }
    
    void processUi()
    {


    }
private:
    DaisySeed& hardware; 
    Patch *patch_;
};

#endif
