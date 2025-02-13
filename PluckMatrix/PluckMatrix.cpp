#include "PluckMatrix.h"
#include "WitechControls.h"
#include "IPlug_include_in_plug_src.h"
#include "LFO.h"

PluckMatrix::PluckMatrix(const InstanceInfo &info) :
  iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamGain)->InitDouble("Gain", 100., 0., 100.0, 0.01, "%");
  GetParam(kParamNoteGlideTime)->InitMilliseconds("Note Glide Time", 0., 0.0, 30.);
  GetParam(kParamAttack)
      ->InitDouble("Attack",
                   10.,
                   1.,
                   1000.,
                   0.1,
                   "ms",
                   IParam::kFlagsNone,
                   "ADSR",
                   IParam::ShapePowCurve(3.));
  GetParam(kParamDecay)
      ->InitDouble("Decay",
                   10.,
                   1.,
                   1000.,
                   0.1,
                   "ms",
                   IParam::kFlagsNone,
                   "ADSR",
                   IParam::ShapePowCurve(3.));
  GetParam(kParamSustain)->InitDouble("Sustain", 50., 0., 100., 1, "%", IParam::kFlagsNone, "ADSR");
  GetParam(kParamRelease)
      ->InitDouble("Release", 10., 2., 1000., 0.1, "ms", IParam::kFlagsNone, "ADSR");
  GetParam(kParamLFOShape)->InitEnum("LFO Shape", LFO<>::kTriangle, { LFO_SHAPE_VALIST });
  GetParam(kParamLFORateHz)->InitFrequency("LFO Rate", 1., 0.01, 40.);
  GetParam(kParamLFORateTempo)->InitEnum("LFO Rate", LFO<>::k1, { LFO_TEMPODIV_VALIST });
  GetParam(kParamLFORateMode)->InitBool("LFO Sync", true);
  GetParam(kParamLFODepth)->InitPercentage("LFO Depth");

  // Led buttons. We don't want them to be able to automate.
  for (int i = kParamLedBtn0; i < kParamLedBtn0 + 32; ++i)
  {
    GetParam(i)->InitBool(("Led button " + std::to_string(i - kParamLedBtn0)).c_str(),
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
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->EnableMouseOver(true);
    pGraphics->EnableMultiTouch(true);

#ifdef OS_WEB
    pGraphics->AttachPopupMenuControl();
#endif

    //    pGraphics->EnableLiveEdit(true);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds().GetPadded(-20.f);
    const IRECT lfoPanel = b.GetFromLeft(300.f).GetFromTop(200.f);
    IRECT keyboardBounds = b.GetFromBottom(300);
    IRECT wheelsBounds = keyboardBounds.ReduceFromLeft(100.f).GetPadded(-10.f);


    // Led buttons
    const IBitmap ledBtnBitmap = pGraphics->LoadBitmap(PNGBTNLED_FN, 2, true);
    for (int i = 0; i < 32; ++i)
    {
      pGraphics->AttachControl(new GroupBtnControl(50.f + i * ledBtnBitmap.W() / 2,
                                                   200.f + ledBtnBitmap.H(),
                                                   ledBtnBitmap,
                                                   kParamLedBtn0 + i,
                                                   mCurrentLed,
                                                   i,
                                                   kCtrlTagLedSeq0,
                                                   32),
                               kCtrlTagLedSeq0 + i);
    }


    pGraphics->AttachControl(new IVKeyboardControl(keyboardBounds), kCtrlTagKeyboard);
    pGraphics->AttachControl(new IWheelControl(wheelsBounds.FracRectHorizontal(0.5)),
                             kCtrlTagBender);
    pGraphics->AttachControl(new IWheelControl(wheelsBounds.FracRectHorizontal(0.5, true),
                                               IMidiMsg::EControlChangeMsg::kModWheel));
    //    pGraphics->AttachControl(new IVMultiSliderControl<4>(b.GetGridCell(0, 2, 2).GetPadded(-30), "", DEFAULT_STYLE, kParamAttack, EDirection::Vertical, 0.f, 1.f));
    const IRECT controls = b.GetGridCell(1, 2, 2);
    pGraphics->AttachControl(
        new IVKnobControl(controls.GetGridCell(0, 2, 6).GetCentredInside(90), kParamGain, "Gain"));
    pGraphics->AttachControl(new IVKnobControl(controls.GetGridCell(1, 2, 6).GetCentredInside(90),
                                               kParamNoteGlideTime,
                                               "Glide"));
    const IRECT sliders = controls.GetGridCell(2, 2, 6)
                              .Union(controls.GetGridCell(3, 2, 6))
                              .Union(controls.GetGridCell(4, 2, 6));
    pGraphics->AttachControl(new IVSliderControl(sliders.GetGridCell(0, 1, 4).GetMidHPadded(30.),
                                                 kParamAttack,
                                                 "Attack"));
    pGraphics->AttachControl(
        new IVSliderControl(sliders.GetGridCell(1, 1, 4).GetMidHPadded(30.), kParamDecay, "Decay"));
    pGraphics->AttachControl(new IVSliderControl(sliders.GetGridCell(2, 1, 4).GetMidHPadded(30.),
                                                 kParamSustain,
                                                 "Sustain"));
    pGraphics->AttachControl(new IVSliderControl(sliders.GetGridCell(3, 1, 4).GetMidHPadded(30.),
                                                 kParamRelease,
                                                 "Release"));
    pGraphics->AttachControl(new IVLEDMeterControl<2>(controls.GetFromRight(100).GetPadded(-30)),
                             kCtrlTagMeter);

    pGraphics
        ->AttachControl(new IVKnobControl(lfoPanel.GetGridCell(0, 0, 2, 3).GetCentredInside(60),
                                          kParamLFORateHz,
                                          "Rate"),
                        kNoTag,
                        "LFO")
        ->Hide(true);
    pGraphics
        ->AttachControl(new IVKnobControl(lfoPanel.GetGridCell(0, 0, 2, 3).GetCentredInside(60),
                                          kParamLFORateTempo,
                                          "Rate"),
                        kNoTag,
                        "LFO")
        ->DisablePrompt(false);
    pGraphics->AttachControl(
        new IVKnobControl(lfoPanel.GetGridCell(0, 1, 2, 3).GetCentredInside(60),
                          kParamLFODepth,
                          "Depth"),
        kNoTag,
        "LFO");
    pGraphics
        ->AttachControl(new IVKnobControl(lfoPanel.GetGridCell(0, 2, 2, 3).GetCentredInside(60),
                                          kParamLFOShape,
                                          "Shape"),
                        kNoTag,
                        "LFO")
        ->DisablePrompt(false);
    pGraphics->AttachControl(
        new IVSlideSwitchControl(lfoPanel.GetGridCell(1, 0, 2, 3).GetFromTop(30).GetMidHPadded(20),
                                 kParamLFORateMode,
                                 "Sync",
                                 DEFAULT_STYLE.WithShowValue(false)
                                     .WithShowLabel(false)
                                     .WithWidgetFrac(0.5f)
                                     .WithDrawShadows(false),
                                 false),
        kNoTag,
        "LFO");
    pGraphics->AttachControl(new IVDisplayControl(lfoPanel.GetGridCell(1, 1, 2, 3)
                                                      .Union(lfoPanel.GetGridCell(1, 2, 2, 3)),
                                                  "",
                                                  DEFAULT_STYLE,
                                                  EDirection::Horizontal,
                                                  0.f,
                                                  1.f,
                                                  0.f,
                                                  1024),
                             kCtrlTagLFOVis,
                             "LFO");

    pGraphics->AttachControl(new IVGroupControl("LFO", "LFO", 10.f, 20.f, 10.f, 10.f));

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
    //#ifdef OS_IOS
    //    if(!IsOOPAuv3AppExtension())
    //    {
    //      pGraphics->AttachControl(new IVButtonControl(b.GetFromTRHC(100, 100), [pGraphics](IControl* pCaller) {
    //                               dynamic_cast<IGraphicsIOS*>(pGraphics)->LaunchBluetoothMidiDialog(pCaller->GetRECT().L, pCaller->GetRECT().MH());
    //                               SplashClickActionFunc(pCaller);
    //                             }, "BTMIDI"));
    //    }
    //#endif

    pGraphics->SetQwertyMidiKeyHandlerFunc(
        [pGraphics](const IMidiMsg &msg)
        {
          pGraphics->GetControlWithTag(kCtrlTagKeyboard)
              ->As<IVKeyboardControl>()
              ->SetNoteFromMidi(msg.NoteNumber(), msg.StatusMsg() == IMidiMsg::kNoteOn);
        });
  };
#endif
}

#if IPLUG_DSP

void
PluckMatrix::ProcessMidiMsg(const IMidiMsg &msg)
{
  TRACE;
  mMidiQueue.Add(msg);  // Take care of MIDI events in ProcessBlock()
}

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

  //// Old stuff
  //mDSP.ProcessBlock(nullptr, outputs, 2, nFrames, mTimeInfo.mPPQPos, mTimeInfo.mTransportIsRunning);
  //mMeterSender.ProcessBlock(outputs, nFrames, kCtrlTagMeter);
  //mLFOVisSender.PushData({ kCtrlTagLFOVis, { float(mDSP.mLFO.GetLastOutput()) } });


  mMidiQueue.Flush(nFrames);
}

void
PluckMatrix::OnIdle()
{
  mMeterSender.TransmitData(*this);
  mLFOVisSender.TransmitData(*this);
  mLedSeqSender.TransmitData(*this);
}

void
PluckMatrix::OnReset()
{
  mDSP.Reset(GetSampleRate(), GetBlockSize());
  mMeterSender.Reset(GetSampleRate());

  mMachine.mSampleRate = GetSampleRate();
}


void
PluckMatrix::OnParamChange(int paramIdx)
{
  mDSP.SetParam(paramIdx, GetParam(paramIdx)->Value());
}

void
PluckMatrix::OnParamChangeUI(int paramIdx, EParamSource source)
{
#if IPLUG_EDITOR
  if (auto pGraphics = GetUI())
  {
    if (paramIdx == kParamLFORateMode)
    {
      const auto sync = GetParam(kParamLFORateMode)->Bool();
      pGraphics->HideControl(kParamLFORateHz, sync);
      pGraphics->HideControl(kParamLFORateTempo, !sync);
    }
  }
#endif
}

bool
PluckMatrix::OnMessage(int msgTag, int ctrlTag, int dataSize, const void *pData)
{
  if (ctrlTag == kCtrlTagBender && msgTag == IWheelControl::kMessageTagSetPitchBendRange)
  {
    const int bendRange = *static_cast<const int *>(pData);
    mDSP.mSynth.SetPitchBendRange(bendRange);
  }

  return false;
}
#endif
