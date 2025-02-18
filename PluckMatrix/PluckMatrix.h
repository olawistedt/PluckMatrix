#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"
#include "Machine.h"
#include "Oscillator.h"
#include "Patterns.h"

const int kNumPresets = 1;
const int kNumberOfStepsInSequence = 32;
const int kNumberOfProperties = 32 * 2;
const int kNumberOfSeqButtons = kNumberOfNotes + kNumberOfProperties;

enum EParams
{
  kParamLedBtn0 = 0,
  kParamNoteBtn0 = kParamLedBtn0 + kNumberOfStepsInSequence,
  kParamPropBtn0 = kParamNoteBtn0 + kNumberOfNotes,
  kNumParams = kParamPropBtn0 + kNumberOfProperties
};

enum EControlTags
{
  kCtrlTagLedSeq0 = 0,
  kCtrlTagNote0 = kCtrlTagLedSeq0 + kNumberOfStepsInSequence,
  kCtrlTagProp0 = kCtrlTagNote0 + kNumberOfNotes,
  kCtrlTagKeyboard = kCtrlTagProp0 + kNumberOfProperties,
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
  ISender<1, 1, int> mLedSeqSender;
  ISender<1, 1, std::array<bool, kNumberOfSeqButtons>> mSequencerSender;
  float mPlugUIScale;
  Machine mMachine;
#endif  // IPLUG_DSP
private:
  Patterns mPatterns;
  IMidiQueue mMidiQueue;
  Oscillator mOscillator;
  int mCurrentLed;
  int mSelectedPattern;
};
