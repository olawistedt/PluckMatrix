#include "TemplateProject.h"
#include "IPlug_include_in_plug_src.h"

#if IPLUG_EDITOR
#include "IControls.h"
#endif

TemplateProject::TemplateProject(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamGain)->InitDouble("Gain", 0., 0., 100.0, 0.01, "%");

  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS);
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    const IRECT bounds = pGraphics->GetBounds();
    const IRECT innerBounds = bounds.GetPadded(-10.f);
    const IRECT sliderBounds = innerBounds.GetCentredInside(200).GetVShifted(-200);
    const IRECT versionBounds = innerBounds.GetFromTRHC(300, 20);
    const IRECT titleBounds = innerBounds.GetCentredInside(200, 50);

    if (pGraphics->NControls()) {
      pGraphics->GetBackgroundControl()->SetTargetAndDrawRECTs(bounds);
      pGraphics->GetControlWithTag(kCtrlTagSlider)->SetTargetAndDrawRECTs(sliderBounds);
      pGraphics->GetControlWithTag(kCtrlTagTitle)->SetTargetAndDrawRECTs(titleBounds);
//      pGraphics->GetControlWithTag(kCtrlTagVersionNumber)->SetTargetAndDrawRECTs(versionBounds);
      return;
    }

    pGraphics->SetLayoutOnResize(true);
    pGraphics->AttachCornerResizer(EUIResizerMode::Size, true);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
//    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->AttachPanelBackground(IPattern::CreateLinearGradient(bounds, EDirection::Vertical, {{COLOR_MID_GRAY, 0.f},{COLOR_GRAY, 1.f}}));
    pGraphics->AttachControl(new IVSliderControl(sliderBounds, kParamGain), kCtrlTagSlider);
    pGraphics->AttachControl(new ITextControl(titleBounds, "TemplateProject", IText(30, COLOR_WHITE)), kCtrlTagTitle);
    WDL_String buildInfoStr;
  };
}

void TemplateProject::OnParentWindowResize(int width, int height)
{
  if (GetUI())
    GetUI()->Resize(width, height, 1.f, false);
}

void TemplateProject::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{ 
  const int nChans = NOutChansConnected();
  const double gain = GetParam(kParamGain)->Value() / 100.;
  
//  for (int s = 0; s < nFrames; s++) {
//    for (int c = 0; c < nChans; c++) {
//      outputs[c][s] = inputs[c][s] * gain;
//    }
//  }
}
