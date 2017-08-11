/******************************************************************************
 *	Author:			Justin Abbott
 *	Email:			justin.abbott90@gmail.com
 *	File:			JFABitcrusher.cpp
 *****************************************************************************/

#include "JFABitCrusher.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "IKnobMultiControlText.h"
#include "resource.h"

const int kNumPrograms = 2;

enum EParams
{
	kBitDepth = 0,
	kBitRate,
	kWetPercent,
	kNumParams
};

enum ELayout
{
	kWidth = GUI_WIDTH,
	kHeight = GUI_HEIGHT,

	kBitDepthX = 16,
	kBitDepthY = 80,
	kBitRateX = 96,
	kBitRateY = 80,
	kWetPercentX = 176,
	kWetPercentY = 80,
	kKnobTextSize = 18,
	kKnobFrames = 128,
	kKnobWidth = 64,
	kKnobHeight = 64
};

JFABitCrusher::JFABitCrusher(IPlugInstanceInfo instanceInfo)
	:	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo),
		wetRatio(1.0f)
{
	TRACE;

	GetParam(kBitDepth)->InitInt("Bit Depth", 8, 2, 32, "bit");
	GetParam(kBitRate)->InitInt("Bit Rate", 4096, 50, 44100, "Hz");
	GetParam(kWetPercent)->InitDouble("Wet / Dry Percent", 100.0, 0.0, 100.0, 0.01, "Wet Percent");

	IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
	pGraphics->AttachBackground(BACKGROUND_ID, BACKGROUND_FN);

	IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);
	IText text = IText(kKnobTextSize);

	pGraphics->AttachControl(new IKnobMultiControlText(this, IRECT(kBitDepthX, kBitDepthY, kBitDepthX + kKnobWidth, kBitDepthY + kKnobHeight + 24), kBitDepth, &knob, &text));
	pGraphics->AttachControl(new IKnobMultiControlText(this, IRECT(kBitRateX, kBitRateY, kBitRateX + kKnobWidth, kBitRateY + kKnobHeight + 24), kBitRate, &knob, &text));
	pGraphics->AttachControl(new IKnobMultiControlText(this, IRECT(kWetPercentX, kWetPercentY, kWetPercentX + kKnobWidth, kWetPercentY + kKnobHeight + 24), kWetPercent, &knob, &text));

	AttachGraphics(pGraphics);

	CreatePresets();
}

JFABitCrusher::~JFABitCrusher() {}

void JFABitCrusher::CreatePresets()
{
	MakePreset("8-Bit", 8, 4096, 100.0);
	MakePreset("16-Bit", 16, 4096, 100.0);
}

void JFABitCrusher::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	// Mutex is already locked for us.

	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
	{
		*out1 = (1 - wetRatio) * *in1 + wetRatio * leftBitcrusher.ProcessSample(*in1);
		*out2 = (1 - wetRatio) * *in2 + wetRatio * rightBitcrusher.ProcessSample(*in2);
	}
}

void JFABitCrusher::Reset()
{
	TRACE;
	IMutexLock lock(this);

	int sampleRate = static_cast<int>(GetSampleRate());
	leftBitcrusher.PrepareToPlay(sampleRate);
	rightBitcrusher.PrepareToPlay(sampleRate);
}

void JFABitCrusher::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	switch (paramIdx)
	{
		case kBitDepth:
		{
			int bitDepth = GetParam(kBitDepth)->Int();
			leftBitcrusher.SetBitDepth(bitDepth);
			rightBitcrusher.SetBitDepth(bitDepth);
			break;
		}
		case kBitRate:
		{
			int bitRate = GetParam(kBitRate)->Int();
			leftBitcrusher.SetBitRate(bitRate);
			rightBitcrusher.SetBitRate(bitRate);
			break;
		}
		case kWetPercent:
			wetRatio = static_cast<float>(GetParam(kWetPercent)->Value() / 100.0);
			break;
		default:
			break;
	}
}
