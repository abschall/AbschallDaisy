// User Interface 
#ifndef UI_H_
#define UI_H_

#include "Drivers/leds.h"
#include "Drivers/switches.h"
#include "patch.h"

/// @brief basic state structure, to be completed with more options 
struct filterStates {
    FilterType filterSelectState;
};

// enum FilterType{
//     TYPE_LPF1,
//     TYPE_LPF2,
//     TYPE_HPF1,
//     TYPE_HPF2,
//     NONE
// };
class Ui
{
public:
    Ui(DaisySeed& h) : hardware(h),leds_(h),switches_(h) {}
    ~Ui(){}

    void Init(Patch *patch)
    {
        leds_.Init();
        switches_.Init();
        states_.filterSelectState = TYPE_LPF1;
        patch_ = patch; 
    }

    filterStates& state()
    {
        return states_;
    }
    
    void processUi()
    {
        switches_.Debounce();
        processFilterSelectButton();

    }
private:
    /// @brief process the filterSelectButton's function, switches the filter Type when pressed 
    void processFilterSelectButton()
    {
        static unsigned int buttonState = 0; 
        if (switches_.justPressed(SWITCH_MODE_SELECT))
        {
            ++buttonState;
            if(buttonState == 0)
            {
                states_.filterSelectState = TYPE_LPF1;
            }
            else if(buttonState == 1)
            {
                states_.filterSelectState = TYPE_LPF2;
            }
            else if(buttonState == 2)
            {
                states_.filterSelectState = TYPE_HPF1;
            }
            else if(buttonState == 3)
            {
                states_.filterSelectState = TYPE_HPF2;            
            }
            else
            {
                buttonState = 0;
                states_.filterSelectState = TYPE_LPF1;
            }
            leds_.LedWrite(buttonState);
            patch_->filterType = states_.filterSelectState;
        }
    }

    DaisySeed& hardware; 
    Leds leds_;
    Switches switches_;
    filterStates states_;
    Patch *patch_;
};

#endif
