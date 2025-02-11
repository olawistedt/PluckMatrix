#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"
#include "Machine.h"

const int kNumPresets = 1;
const int kNumberOfStepsInSequence = 32;
const int kNumberOfSeqButtons = 13 * 32;

enum EParams
{
  kParamLedBtn0 = 0,
  kParamGain = kParamLedBtn0 + kNumberOfStepsInSequence,
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
  kCtrlTagMeter = kCtrlTagLedSeq0 + kNumberOfStepsInSequence,
  kCtrlTagBtnSeq0,
  kCtrlTagLFOVis = kCtrlTagBtnSeq0 + kNumberOfSeqButtons,
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

#if IPLUG_DSP  // http://bit.ly/2S64BDd
public:
  void ProcessBlock(sample **inputs, sample **outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg &msg) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void OnParamChangeUI(int paramIdx, EParamSource source) override;
  void OnIdle() override;
  bool OnMessage(int msgTag, int ctrlTag, int dataSize, const void *pData) override;

private:
  PluckMatrixDSP<sample> mDSP{ 16 };
  IPeakAvgSender<2> mMeterSender;
  ISender<1> mLFOVisSender;
  int mCurrentLed;
  ISender<1, 1, int> mLedSeqSender;
  Machine mMachine;

#endif
};
