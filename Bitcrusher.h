/******************************************************************************
 *	Author:			Justin Abbott
 *	Email:			justin.abbott90@gmail.com
 *	File:			Bitcrusher.h
 *****************************************************************************/

#pragma once

#include <math.h>

class Bitcrusher
{
	/**************************************************************************
	 *	PRIVATE MEMBER VARIABLES
	 *************************************************************************/

private:
	// User variables
	int bitDepth;
	int bitRate;

	// Internal variables
	int sampleRate;
	float holdCounter;
	float holdLength;
	float holdSample;

	/**************************************************************************
	 *	CONSTRUCTOR AND DESTRUCTOR
	 *************************************************************************/

public:
	Bitcrusher();
	~Bitcrusher();

	/**************************************************************************
	 *	PRIVATE METHODS
	 *************************************************************************/

private:
	inline float MidTreadQuantize(const float value, const float stepSize) { return stepSize * floorf((value / stepSize) + 0.5f); };
	void UpdateHoldLength();

	/**************************************************************************
	 *	PUBLIC METHODS
	 *************************************************************************/

public:
	void PrepareToPlay(const int sampleRate);
	float ProcessSample(const float input);
	void SetBitDepth(const int bitDepth) { this->bitDepth = bitDepth; };
	void SetBitRate(const int bitRate);
};