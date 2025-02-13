#ifndef __OSCILLATOR_H__
#define __OSCILLATOR_H__

//------------------------------------------------------------------------------------------
// Oscillator
//------------------------------------------------------------------------------------------
class Oscillator
{
public:
  Oscillator();
  double ProcessSample();
  void SetSampleRate(double sampleRate);
  void NoteOn(int note);
  void NoteOff();

private:
  double mNote2FreqTable[140];
  double mSampleRate;
  unsigned long mPeriodLength;  // Period legth in bits.
  unsigned long mCurrent;       // x position in sample curve.
  double mFrequency;
  bool mIsNoteOn;
};

#endif  // __OSCILLATOR_H__
