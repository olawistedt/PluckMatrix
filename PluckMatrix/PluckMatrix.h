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
  kParamGain = kParamPropBtn0 + kNumberOfProperties,
  kParamNoteGlideTime,
  kParamAttack,
  kParamDecay,
  kParamSustain,
  kParamRelease,
  kParamLFOShape,
  kParamLFORateHz,
  kParamLFORateTempo,
  kParamLFORateMode,
  kParamLFODepth,
  kNumParams
};

#if IPLUG_DSP
// will use EParams in PluckMatrix_DSP.h
#include "PluckMatrix_DSP.h"
#endif

enum EControlTags
{
  kCtrlTagLedSeq0 = 0,
  kCtrlTagNote0 = kCtrlTagLedSeq0 + kNumberOfStepsInSequence,
  kCtrlTagProp0 = kCtrlTagNote0 + kNumberOfNotes,
  kCtrlTagMeter = kCtrlTagProp0 + kNumberOfProperties,
  kCtrlTagLFOVis,
  kCtrlTagScope,
  kCtrlTagRTText,
  kCtrlTagKeyboard,
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
  bool OnMessage(int msgTag, int ctrlTag, int dataSize, const void *pData) override;

private:
  std::array<bool, kNumberOfSeqButtons> CollectSequenceButtons(int patternNr = -1);

  PluckMatrixDSP<sample> mDSP{ 16 };
  IPeakAvgSender<2> mMeterSender;
  ISender<1> mLFOVisSender;
  ISender<1, 1, int> mLedSeqSender;
  ISender<1, 1, std::array<bool, kNumberOfSeqButtons>> mSequencerSender;
  Machine mMachine;
#endif  // IPLUG_DSP
private:
  Patterns mPatterns;
  IMidiQueue mMidiQueue;
  Oscillator mOscillator;
  int mCurrentLed;
  int mSelectedPattern;
};
