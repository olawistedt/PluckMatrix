#ifndef __PLUCKMATRIX_H__
#define __PLUCKMATRIX_H__

#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"
#include "Machine.h"
#include "Oscillator.h"
#include "Patterns.h"

const int kNumPresets = 1;
const int kNumberOfStepsInSequence = 32;
const int kNumberOfNoteBtns = kNumberOfStepsInSequence * kNrOfNotes;
const int kNumberOfPropBtns = kNumberOfStepsInSequence * kNrOfProps;
const int kNumberOfSeqButtons = kNumberOfNoteBtns + kNumberOfPropBtns;

enum EParams
{
  kParamLedBtn0 = 0,
  kParamNoteBtn0 = kParamLedBtn0 + kNumberOfStepsInSequence,
  kParamPropBtn0 = kParamNoteBtn0 + kNumberOfNoteBtns,
  kNumParams = kParamPropBtn0 + kNumberOfPropBtns
};

enum EControlTags
{
  kCtrlTagLedSeq0 = 0,
  kCtrlTagNote0 = kCtrlTagLedSeq0 + kNumberOfStepsInSequence,
  kCtrlTagProp0 = kCtrlTagNote0 + kNumberOfNoteBtns,
  kCtrlTagKeyboard = kCtrlTagProp0 + kNumberOfPropBtns,
  kCtrlTagBender,
  kNumCtrlTags
};

using namespace iplug;
using namespace igraphics;

class PluckMatrix final : public Plugin
{
public:
  PluckMatrix(const InstanceInfo &info);

#if IPLUG_DSP
public:
  void ProcessBlock(sample **inputs, sample **outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg &msg) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void OnParamChangeUI(int paramIdx, EParamSource source) override;
  void OnIdle() override;

private:
  std::array<bool, kNumberOfSeqButtons> CollectSequenceButtons(int patternNr = -1);
  Machine mMachine;
#endif  // IPLUG_DSP
private:
  ISender<1, 1, int> mLedSeqSender;
  ISender<1, 1, std::array<bool, kNumberOfSeqButtons>> mSequencerSender;
  float mPlugUIScale;
  Patterns mPatterns;
  IMidiQueue mMidiQueue;
  Oscillator mOscillator;
  int mCurrentLed;
  int mSelectedPattern;
};

#endif  // __PLUCKMATRIX_H__