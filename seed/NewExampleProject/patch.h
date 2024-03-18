// Patch parameters

#ifndef PATCH_H_
#define PATCH_H_

/// @brief main parameters used by the DSP algorithm 
enum FilterType{
    TYPE_LPF1,
    TYPE_LPF2,
    TYPE_HPF1,
    TYPE_HPF2,
    NONE
};

struct Patch
{
    float cutoffFreq;
    float qFactor;
    FilterType filterType;
};

#endif 