#include "PluckMatrix.h"
#include "WitechControls.h"
#include "IPlug_include_in_plug_src.h"
#include "LFO.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// DSP
///////////////////////////////////////////////////////////////////////////////////////////////////

#if IPLUG_DSP
void
PluckMatrix::ProcessBlock(sample **inputs, sample **outputs, int nFrames)
{
  mLedSeqSender.PushData({ kCtrlTagLedSeq0, { mMachine.getStep() } });
  mMachine.mBpm = GetTempo();
  mOscillator.SetSampleRate(GetSampleRate());

  for (int offset = 0; offset < nFrames; ++offset)
  {
    // The built in keyboard sends midi messages.
    while (!mMidiQueue.Empty())
    {
      IMidiMsg msg = mMidiQueue.Peek();
      if (msg.mOffset > offset)
        break;

      if (msg.StatusMsg() == IMidiMsg::kNoteOn)
      {
        if (msg.NoteNumber() >= 48 && msg.NoteNumber() <= 71)
        {                          // Octav 2 and octav 3
          mMachine.setStep(0, 0);  // When pattern note is pressed, begin the pattern from start.
          mMachine.start();        // Start the sequencer.
          mOscillator.NoteOn(msg.NoteNumber());  // Play midi notes outside the pattern range.
        }
        else
        {
          mOscillator.NoteOn(msg.NoteNumber());  // Play midi notes outside the pattern range.
        }
      }
      else if (msg.StatusMsg() == IMidiMsg::kNoteOff)
      {
        mMachine.stop();
        mOscillator.NoteOff();
      }

      mMidiQueue.Remove();
    }


    if (mMachine.StepSequencerOneSample())  // Returns true if a new step is entered.
    {
      mCurrentLed = mMachine.getStep();
      mLedSeqSender.PushData({ kCtrlTagLedSeq0, { mCurrentLed } });
    }
    outputs[0][offset] = outputs[1][offset] = mOscillator.ProcessSample();
  }

  mMidiQueue.Flush(nFrames);
}

void
PluckMatrix::ProcessMidiMsg(const IMidiMsg &msg)
{
  TRACE;
  mMidiQueue.Add(msg);  // Take care of MIDI events in ProcessBlock()
}

void
PluckMatrix::OnIdle()
{
  mLedSeqSender.TransmitData(*this);
  mSequencerSender.TransmitData(*this);

  // Update the plugin scale.
  if (GetUI())
  {
    mPlugUIScale = GetUI()->GetDrawScale();
  }
}

void
PluckMatrix::OnReset()
{
  mMachine.mSampleRate = GetSampleRate();
  mSequencerSender.PushData({ kCtrlTagNote0, { CollectSequenceButtons(-1) } });
}


void
PluckMatrix::OnParamChange(int paramIdx)
{
}

void
PluckMatrix::OnParamChangeUI(int paramIdx, EParamSource source)
{
#if IPLUG_EDITOR
#endif  // IPLUG_EDITOR
}

std::array<bool, kNumberOfSeqButtons>
PluckMatrix::CollectSequenceButtons(int patternNr)
{
  std::array<bool, kNumberOfSeqButtons> seq;

  if (patternNr == -1)
  {
    patternNr = mPatterns.mSelectedPattern;
  }

  for (int i = 0; i < kNumberOfNotes; ++i)
  {
    int col = i % kNumberOfStepsInSequence;
    int row = i / kNumberOfStepsInSequence;
    seq[i] = mPatterns.mNotes[patternNr][col] == row;
  }
  return seq;
}

#endif  // IPLUG_DSP


///////////////////////////////////////////////////////////////////////////////////////////////////
// GUI
///////////////////////////////////////////////////////////////////////////////////////////////////

PluckMatrix::PluckMatrix(const InstanceInfo &info) :
  iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets)),
  mPlugUIScale(1.0)
{
  // Led buttons. We don't want them to be able to automate.
  for (int i = kParamLedBtn0; i < kParamLedBtn0 + 32; ++i)
  {
    GetParam(i)->InitBool(("Led button " + std::to_string(i - kParamLedBtn0)).c_str(),
                          false,
                          "On/Off",
                          IParam::kFlagCannotAutomate);
  }

  // Note buttons. We don't want them to be able to automate.
  for (int i = kParamNoteBtn0; i < kParamNoteBtn0 + kNumberOfNotes; ++i)
  {
    GetParam(i)->InitBool(("Note button " + std::to_string(i - kParamNoteBtn0)).c_str(),
                          false,
                          "On/Off",
                          IParam::kFlagCannotAutomate);
  }

  // Property buttons. We don't want them to be able to automate.
  for (int i = kParamPropBtn0; i < kParamPropBtn0 + kNumberOfProperties; ++i)
  {
    GetParam(i)->InitBool(("Proerty Note button " + std::to_string(i - kParamPropBtn0)).c_str(),
                          false,
                          "On/Off",
                          IParam::kFlagCannotAutomate);
  }

  // Note buttons and property buttons. We don't want them to be able to automate.
  for (int i = kParamPropBtn0; i < kParamPropBtn0 + kNumberOfProperties; ++i)
  {
    GetParam(i)->InitBool(("Property button " + std::to_string(i - kParamPropBtn0)).c_str(),
                          false,
                          "On/Off",
                          IParam::kFlagCannotAutomate);
  }

#if IPLUG_EDITOR  // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]()
  {
    return MakeGraphics(*this,
                        PLUG_WIDTH,
                        PLUG_HEIGHT,
                        PLUG_FPS,
                        GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics *pGraphics)
  {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);

    const IColor COLOR_PLUCK_MATRIX_BLACK(255, 30, 30, 30);
    pGraphics->AttachPanelBackground(COLOR_PLUCK_MATRIX_BLACK);
    //    pGraphics->LoadBitmap(BACKGROUND_FN, 1, true);
    //    pGraphics->AttachBackground(BACKGROUND_FN);

    pGraphics->EnableMouseOver(true);
    pGraphics->EnableMultiTouch(true);

#ifdef OS_WEB
    pGraphics->AttachPopupMenuControl();
#endif

    //    pGraphics->EnableLiveEdit(true);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    // Note buttons
    const IBitmap noteBtnBitmap = pGraphics->LoadBitmap(PNGBTNNOTE_FN, 2, true);
    for (int i = 0; i < kNumberOfStepsInSequence; i++)
    {
      float xOffset = 50.f + i * (noteBtnBitmap.W() / 2 + 21) + (i / 4) * 12;

      for (int j = 0; j < 12; j++)
      {
        pGraphics->AttachControl(new SeqNoteBtnControl(xOffset,
                                                       250.f + j * (noteBtnBitmap.H() + 1),
                                                       noteBtnBitmap,
                                                       kParamNoteBtn0 +
                                                           kNumberOfStepsInSequence * j + i),
                                 kCtrlTagNote0 + kNumberOfStepsInSequence * j + i,
                                 "Sequencer");
      }
    }

    // Property buttons
    for (int i = 0; i < kNumberOfStepsInSequence; i++)
    {
      float xOffset = 50.f + i * (noteBtnBitmap.W() / 2 + 21) + (i / 4) * 12;

      for (int j = 0; j < 2; j++)
      {
        pGraphics->AttachControl(new SeqNoteBtnControl(xOffset,
                                                       550.f + j * (noteBtnBitmap.H() + 1),
                                                       noteBtnBitmap,
                                                       kParamPropBtn0 +
                                                           kNumberOfStepsInSequence * j + i),
                                 kCtrlTagProp0 + kNumberOfStepsInSequence * j + i,
                                 "Sequencer");
      }
    }
    // Led buttons
    const IBitmap ledBtnBitmap = pGraphics->LoadBitmap(PNGBTNLED_FN, 2, true);
    for (int i = 0; i < kNumberOfStepsInSequence; ++i)
    {
      float xOffset = 50.f + i * (ledBtnBitmap.W() / 2) + (i / 4) * 12;

      pGraphics->AttachControl(new GroupBtnControl(xOffset,
                                                   590.f + ledBtnBitmap.H(),
                                                   ledBtnBitmap,
                                                   kParamLedBtn0 + i,
                                                   mCurrentLed,
                                                   i,
                                                   kCtrlTagLedSeq0,
                                                   32),
                               kCtrlTagLedSeq0 + i);
    }

    IRECT keyboardBounds(20, 750, 1900, 980);
    pGraphics->AttachControl(new IVKeyboardControl(keyboardBounds, 36, 89), kCtrlTagKeyboard);
    pGraphics
        ->AttachControl(new IVButtonControl(
            keyboardBounds.GetFromTRHC(200, 30).GetTranslated(0, -30),
            SplashClickActionFunc,
            "Show/Hide Keyboard",
            DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE).WithLabelText({ 15.f, EVAlign::Middle })))
        ->SetAnimationEndActionFunction(
            [pGraphics](IControl *pCaller)
            {
              static bool hide = false;
              pGraphics->GetControlWithTag(kCtrlTagKeyboard)->Hide(hide = !hide);
              pGraphics->Resize(PLUG_WIDTH,
                                hide ? PLUG_HEIGHT / 2 : PLUG_HEIGHT,
                                pGraphics->GetDrawScale());
            });

    pGraphics->SetQwertyMidiKeyHandlerFunc(
        [pGraphics](const IMidiMsg &msg)
        {
          pGraphics->GetControlWithTag(kCtrlTagKeyboard)
              ->As<IVKeyboardControl>()
              ->SetNoteFromMidi(msg.NoteNumber(), msg.StatusMsg() == IMidiMsg::kNoteOn);
        });

    mPlugUIScale = 0.5f;
    pGraphics->Resize(PLUG_WIDTH, PLUG_HEIGHT, mPlugUIScale, true);
    pGraphics->AttachControl(
        new ITextControl(IRECT(20, 20, 70, 50), "v0.1", IText(30), IColor(255, 255, 255, 255)));
  };
#endif
}
