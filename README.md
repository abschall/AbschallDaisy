# AbschallDaisy

> Abschall's Daisy projects
> 
>  


## Daisy Seed projects 
Abschall's Daisy Seed projects: 
- [/ClassicFilters](seed/ClassicFilters) first simple Daisy Seed Project, which is a selection of embedded 2nd order Classic Filters ( based upon the biquad structure)
- [/DattorroReverb](seed/DattorroReverb) embedded Dattorro Reverb algorithm

**/seed** host folder to personnal Daisy Seed projects.  

**/dsp_fv** personal DSP algorithms library. *.h* files have to be added in a **/DSP** folder inside a personal project in the **/seed** folder 

## Daisy Libraries

This repo is home to Daisy projects utilizing *libDaisy* and *DaisySP* libraries. As for now, the used embedded Daisy plateform is the Daisy SEED.

**/libDaisy** libDaisy library

**/DaisySP** DaisySP library

**Compiling the libraries**

To build all the libraries at once, run: `./rebuild_all.sh`

This is a little time  consuming, and more often than not, only one example, or the libraries need to get built frequently.

To build both libraries at once simply run:

`./ci/build_libs.sh`

This is the same as going to each library's directory and running `make`.

## Adding New Project and Updating Old Ones
*taken from the DaisyExamples/README.md on [DaisyExample repository](https://github.com/electro-smith/DaisyExamples/tree/master)*

`helper.py` is a python script in the root directory that can be used for several helpful utilities.

The script does expect that python3 is installed.

One thing that must be kept in mind is that the script does not know where libDaisy/DaisySP
are located. So all copied/created projects are expected to be two directories up from whereever the libraries are  (e.g. `seed/new_project/`).

Adding an argument to specify this is planned, but not yet available.

**Create new example project**

Creates a brand new example project containing a Makefile, compilable source file,
and debug resources for VisualStudio using VisualGDB, and for VS Code using Cortex Debug.

The board option can be any of the following:

field, patch, petal, pod, seed, versio

`./helper.py create pod/MyNewProject --board pod`
