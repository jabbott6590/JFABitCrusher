/******************************************************************************
 *	Author:			Justin Abbott
 *	Email:			justin.abbott90@gmail.com
 *	File:			JFABitcrusher.h
 *****************************************************************************/

#ifndef __JFABITCRUSHER__
#define __JFABITCRUSHER__

#include "IPlug_include_in_plug_hdr.h"
#include "Bitcrusher.h"

class JFABitCrusher : public IPlug
{
private:
	Bitcrusher leftBitcrusher;
	Bitcrusher rightBitcrusher;

	float wetRatio; // Number in range [0, 1] indicating the ratio of the wet signal with the dry signal

public:
  JFABitCrusher(IPlugInstanceInfo instanceInfo);
  ~JFABitCrusher();

private:
	void CreatePresets();

public:
  void Reset() override;
  void OnParamChange(int paramIdx) override;
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames) override;

};

#endif
