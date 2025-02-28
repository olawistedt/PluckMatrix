#include "Machine.h"
#include <algorithm>
#include <assert.h>
#include "PluckMatrix.h"

//
// The machine has only one task and that is to keep track of where the sequencer are.
//
Machine::Machine() :
  mCurrentStep(0),
  mNextStep(0),
  mDriftError(0.0),
  mCountDown(-1),
  mRunning(true),
  mSamplesPer16th(0)
{
}

void
Machine::setStep(int inStep, int samplesLeftInStep)
{
  mNextStep = inStep;
  mCountDown = samplesLeftInStep;
}

void
Machine::setSamplesPer16th(double sampleRate)
{
  double secondsPerBeat = 60.0 / mBpm;
  double secondsPer16th = secondsPerBeat / 4.0;
  mSamplesPer16th = static_cast<int>(sampleRate * secondsPer16th);
}

int
Machine::getStep()
{
  return mCurrentStep;
}

int  // Returns true if on a beginning of a new 16th
Machine::StepSequencerOneSample()
{
  if (mRunning == false)
  {
    return kNoEvent;
  }
  if (mCountDown > 0)
  {
    mCountDown--;

    if (static_cast<float>(mCountDown) / static_cast<float>(mSamplesPer16th) < 0.1f)
    {
      return kNoteOff;
    }
    return kNoEvent;
  }
  else  // mCountDown <= 0, a new step begins
  {
    double secondsToNextStep = (60.0 / mBpm) * 0.25;
    double samplesToNextStep = secondsToNextStep * mSampleRate;
    mCountDown = roundToInt(samplesToNextStep);

    // keep track of accumulating error due to rounding and compensate when the accumulated error
    // exceeds half a sample:
    mDriftError += mCountDown - samplesToNextStep;
    if (mDriftError < -0.5)  // negative errors indicate that we are too early
    {
      mDriftError += 1.0;
      mCountDown += 1;
    }
    else if (mDriftError >= 0.5)
    {
      mDriftError -= 1.0;
      mCountDown -= 1;
    }
    mCurrentStep = mNextStep;
    mNextStep = (mCurrentStep + 1) % kNumberOfStepsInSequence;
    return kNewStep;
  }
}


/** Assuming, that the FPU is in 'to nearest even integer' rounding mode (which is the default),
  this function rounds to the nearest integer using upward rounding when the argument is exactly
  halfway between two integers (instead of returning the nearest even integer in this case).
  Argument x must satify (INT_MIN/2)�1.0 < x < (std::numeric_limits<int>::max()/2)+1.0.  */

int
Machine::roundToInt(double x)
{
  double xFloor = floor(x);
  double xFrac = x - xFloor;
  if (xFrac >= 0.5)
    return (int)xFloor + 1;
  else
    return (int)xFloor;
}

void
Machine::start()
{
  mRunning = true;
}

void
Machine::stop()
{
  mRunning = false;
}
