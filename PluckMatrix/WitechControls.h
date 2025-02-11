#pragma once

//#if IPLUG_EDITOR
#include "IControls.h"
//#endif

////////////////////////////////////////////////////////////////////////////////////////
// SeqNoteBtnControl
// A button control that can take a message from the DSP
////////////////////////////////////////////////////////////////////////////////////////
class SeqNoteBtnControl : public IBSwitchControl
{
public:
  SeqNoteBtnControl(float x, float y, const IBitmap &bitmap, int paramIdx);
  static void SetSequencerButtons(std::array<bool, kNumberOfSeqButtons> sequencer, IGraphics *ui);
  void OnMsgFromDelegate(int msgTag, int dataSize, const void *pData) override;

protected:
  int mParamIdx;
};

////////////////////////////////////////////////////////////////////////////////////////
// GroupBtnControl
// A button that turns off all the other buttons in the group when selected
////////////////////////////////////////////////////////////////////////////////////////
class GroupBtnControl : public IBSwitchControl
{
public:
  GroupBtnControl(float x,
                  float y,
                  const IBitmap &bitmap,
                  int paramIdx,
                  int &selectedGroupTag,
                  int thisTag,
                  int groupStart,
                  int nrOfGroupMembers);
  void OnMsgFromDelegate(int msgTag, int dataSize, const void *pData) override;
  void OnMouseDown(float x, float y, const IMouseMod &mod) override;

protected:
  int &mSelectedGroupTag;
  int mThisTag;
  int mGroupStart;
  int mNrOfGroupMembers;
};

////////////////////////////////////////////////////////////////////////////////////////
// BounceBtnControl
////////////////////////////////////////////////////////////////////////////////////////
class BounceBtnControl : public IBSwitchControl
{
public:
  BounceBtnControl(float x, float y, const IBitmap &bitmap, int paramIdx);
  void OnMouseDown(float x, float y, const IMouseMod &mod) override;
  void OnMouseUp(float x, float y, const IMouseMod &mod) override;
};

////////////////////////////////////////////////////////////////////////////////////////
// SoloMuteBtnControl
// A button control that can take a message from the DSP
////////////////////////////////////////////////////////////////////////////////////////
class SoloMuteBtnControl : public IBSwitchControl
{
public:
  SoloMuteBtnControl(float x, float y, const IBitmap &bitmap, int paramIdx);
  void OnMsgFromDelegate(int msgTag, int dataSize, const void *pData) override;
};

////////////////////////////////////////////////////////////////////////////////////////
// TDSKnobControl
////////////////////////////////////////////////////////////////////////////////////////
class TDSKnobControl : public IBKnobControl
{
public:
  TDSKnobControl(float x, float y, const IBitmap &bitmap, int paramIdx, int thisTag);
  void OnMsgFromDelegate(int msgTag, int dataSize, const void *pData) override;

protected:
  int mThisTag;
};
