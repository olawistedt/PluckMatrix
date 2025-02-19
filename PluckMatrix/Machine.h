#pragma once

#include "IPlugConstants.h"

class Note
{
public:
  int mNoteNr;
};

class Machine
{
public:
  enum
  {
    kNoEvent = -1,
    kNoteOff = 80,
    kNewStep = 0
  };
  Machine();
  void setSamplesPer16th(double sampleRate);
  void setStep(int inStep, int samplesLeftInStep);
  int getStep();
  int StepSequencerOneSample();
  int roundToInt(double x);
  void start();
  void stop();


  bool mRunning;
  double mBpm;
  double mSampleRate;

private:
  int mCurrentStep;  // the current step
  int mNextStep;
  int mCountDown;      // a sample-countdown - counts down for the next step to occur
  int mSamplesPer16th;
  double mDriftError;  // to keep track and compensate for accumulating timing error
};
