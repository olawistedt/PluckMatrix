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
  Machine();
  void setStep(int inStep, int samplesLeftInStep);
  int getStep();
  bool StepSequencerOneSample();
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
  double mDriftError;  // to keep track and compensate for accumulating timing error
};
