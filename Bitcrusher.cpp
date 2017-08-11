/******************************************************************************
 *	Author:			Justin Abbott
 *	Email:			justin.abbott90@gmail.com
 *	File:			Bitcrusher.cpp
 *****************************************************************************/

#include "Bitcrusher.h"

Bitcrusher::Bitcrusher()
	:	bitDepth(4),
		bitRate(4096),
		sampleRate(0),
		holdCounter(0),
		holdLength(0),
		holdSample(0)
{
}

Bitcrusher::~Bitcrusher()
{
}

void Bitcrusher::UpdateHoldLength()
{
	holdLength = bitRate >= sampleRate ? 1.0f : static_cast<float>(sampleRate) / static_cast<float>(bitRate);
}

void Bitcrusher::PrepareToPlay(const int sampleRate)
{
	this->sampleRate = sampleRate;
	UpdateHoldLength();
	holdCounter = holdLength;
}

float Bitcrusher::ProcessSample(const float input)
{
	if (holdCounter < holdLength)
	{
		++holdCounter;
		return holdSample;
	}

	holdCounter -= holdLength;

	// Lower the bit resolution using the bit depth
	float stepSize = 1.0f / powf(2.0f, static_cast<float>(bitDepth));
	holdSample = MidTreadQuantize(input, stepSize);
	return holdSample;
}

void Bitcrusher::SetBitRate(const int bitRate)
{
	this->bitRate = bitRate;
	UpdateHoldLength();
}
