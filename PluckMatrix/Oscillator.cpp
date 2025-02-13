#include "Oscillator.h"
#include <cmath>

Oscillator::Oscillator() : mCurrent(0), mIsNoteOn(false)
{
  // Build a notes to frequences table.
  for (int i = 0; i < 140; i++)
  {
    mNote2FreqTable[i] = 440.0 * pow(2.0, (i - 69) / 12.0);
  }
}

// Returns a sawtooth wave
double
Oscillator::ProcessSample()
{
  if (!mIsNoteOn)
  {
    return 0.0;
  }
  double output =
      (double)(((double)(mCurrent % mPeriodLength)) * (2.0 / ((double)mPeriodLength)) - 1.0);
  mCurrent++;
  return output;
}

void
Oscillator::SetSampleRate(double sampleRate)
{
  mSampleRate = sampleRate;
  mPeriodLength = static_cast<unsigned long>(mSampleRate / mFrequency);
}

void
Oscillator::NoteOn(int noteValue)
{
  mFrequency = mNote2FreqTable[noteValue];
  mPeriodLength = static_cast<unsigned long>(mSampleRate / mFrequency);
  mIsNoteOn = true;
}

void
Oscillator::NoteOff()
{
  mIsNoteOn = false;
}
