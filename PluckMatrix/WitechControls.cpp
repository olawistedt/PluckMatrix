#include "PluckMatrix.h"
#include "WitechControls.h"

// A button control that can take a message from the DSP
SeqNoteBtnControl::SeqNoteBtnControl(float x, float y, const IBitmap &bitmap, int paramIdx) :
  IBSwitchControl(x, y, bitmap, paramIdx),
  mParamIdx(paramIdx)
{
}

//
// This function updates one the GUI sequencer buttons. It is just in GUI visible things. The actual values
// is stored in Sequencer. So the sender of these notes should have read the values from the Sequencer.
//
void
SeqNoteBtnControl::SetSequencerButtons(std::array<bool, kNumberOfSeqButtons> sequencer,
                                       IGraphics *ui)
{
  Trace(TRACELOC, "");
  for (int i = 0; i < kNumberOfSeqButtons; i++)
  {
    IControl *pControlBtn = ui->GetControlWithTag(kCtrlTagBtnSeq0 + i);
    double before = pControlBtn->GetValue();
    pControlBtn->SetValue(sequencer[i] ? 1.0 : 0.0);
    if (before != pControlBtn->GetValue())
    {
      pControlBtn->SetDirty(true);
    }
  }
}

void
SeqNoteBtnControl::OnMsgFromDelegate(int msgTag, int dataSize, const void *pData)
{
  if (!IsDisabled() && msgTag == ISender<>::kUpdateMessage)
  {
    IByteStream stream(pData, dataSize);
    int pos = 0;
    ISenderData<1, std::array<bool, kNumberOfSeqButtons>> d;
    pos = stream.Get(&d, pos);
    std::array<bool, kNumberOfSeqButtons> sequencer = d.vals[0];

    SeqNoteBtnControl::SetSequencerButtons(sequencer, GetUI());

    SetDirty(false);
  }
}

////////////////////////////////////////////////////////////////////////////////////////
// GroupBtnControl
////////////////////////////////////////////////////////////////////////////////////////
GroupBtnControl::GroupBtnControl(float x,
                                 float y,
                                 const IBitmap &bitmap,
                                 int paramIdx,
                                 int &selectedGroupTag,
                                 int thisTag,
                                 int groupStart,
                                 int nrOfGroupMembers) :
  IBSwitchControl(x, y, bitmap, paramIdx),
  mSelectedGroupTag(selectedGroupTag),
  mThisTag(thisTag),
  mGroupStart(groupStart),
  mNrOfGroupMembers(nrOfGroupMembers)
{
}

void
GroupBtnControl::OnMsgFromDelegate(int msgTag, int dataSize, const void *pData)
{
  if (GetUI())
  {
    if (!IsDisabled() && msgTag == ISender<>::kUpdateMessage)
    {
      IByteStream stream(pData, dataSize);
      int pos = 0;
      ISenderData<1, int> d;
      pos = stream.Get(&d, pos);
      mSelectedGroupTag = d.vals[0];

      for (int i = 0; i < mNrOfGroupMembers; i++)
      {
        IControl *pControlBtn = GetUI()->GetControlWithTag(mGroupStart + i);
        if (i == mSelectedGroupTag)
        {
          pControlBtn->SetValue(1.0);
        }
        else
        {
          pControlBtn->SetValue(0.0);
        }
        pControlBtn->SetDirty(true);
      }
    }
  }
}

void
GroupBtnControl::OnMouseDown(float x, float y, const IMouseMod &mod)
{
  if (GetUI())
  {
    for (int i = 0; i < mNrOfGroupMembers; i++)
    {
      IControl *pControlBtn = GetUI()->GetControlWithTag(mGroupStart + i);
      if (i == mThisTag)
      {
        pControlBtn->SetValue(1.0);
      }
      else
      {
        pControlBtn->SetValue(0.0);
      }
      pControlBtn->SetDirty(true);
    }
  }
  return;
}


////////////////////////////////////////////////////////////////////////////////////////
// BounceBtnControl
////////////////////////////////////////////////////////////////////////////////////////
BounceBtnControl::BounceBtnControl(float x, float y, const IBitmap &bitmap, int paramIdx) :
  IBSwitchControl(x, y, bitmap, paramIdx)
{
}

void
BounceBtnControl::OnMouseDown(float x, float y, const IMouseMod &mod)
{
  SetValue(1.0);
  SetDirty();
}

void
BounceBtnControl::OnMouseUp(float x, float y, const IMouseMod &mod)
{
  SetValue(0.0);
  SetDirty();
}


////////////////////////////////////////////////////////////////////////////////////////
// TDSKnobControl
////////////////////////////////////////////////////////////////////////////////////////
TDSKnobControl::TDSKnobControl(float x, float y, const IBitmap &bitmap, int paramIdx, int thisTag) :
  IBKnobControl(x, y, bitmap, paramIdx),
  mThisTag(thisTag)
{
}

void
TDSKnobControl::OnMsgFromDelegate(int msgTag, int dataSize, const void *pData)
{
  if (!IsDisabled() && msgTag == ISender<>::kUpdateMessage)
  {
    IByteStream stream(pData, dataSize);
    int pos = 0;

    ISenderData<1, double> d;
    pos = stream.Get(&d, pos);
    double v = d.vals[0];
    GetUI()->GetControlWithTag(mThisTag)->SetValue(v);
    GetUI()->GetControlWithTag(mThisTag)->SetDirty(true);
  }
}
