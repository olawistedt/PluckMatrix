#include "Synth.h"

Synth::Synth() {}

double
Synth::ProcessSample()
{
  return mOscillator.ProcessSample();
}

void
Synth::SetSampleRate(double sampleRate)
{
  mOscillator.SetSampleRate(sampleRate);
}

void
Synth::NoteOn(int noteValue)
{
  mOscillator.NoteOn(noteValue);
}

void
Synth::NoteOff()
{
  mOscillator.NoteOff();
}
