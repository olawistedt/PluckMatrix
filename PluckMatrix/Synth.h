#ifndef __SYNTH_H__
#define __SYNTH_H__

#include "Oscillator.h"

class Synth
{
public:
  Synth();
  void SetSampleRate(double sampleRate);
  void NoteOn(int note);
  void NoteOff();
  double ProcessSample();

private:
  Oscillator mOscillator;
};

#endif  // __SYNTH_H__
