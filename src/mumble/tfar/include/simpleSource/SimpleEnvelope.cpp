/*
 *	Simple Envelope Detectors (source)
 *
 *  File		: SimpleEnvelope.cpp
 *	Library		: SimpleSource
 *  Version		: 1.12
 *  Implements	: EnvelopeDetector, AttRelEnvelope
 *
 *	© 2006, ChunkWare Music Software, OPEN-SOURCE
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a
 *	copy of this software and associated documentation files (the "Software"),
 *	to deal in the Software without restriction, including without limitation
 *	the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *	and/or sell copies of the Software, and to permit persons to whom the
 *	Software is furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *	DEALINGS IN THE SOFTWARE.
 */


#include "SimpleEnvelope.h"

namespace chunkware_simple
{
	//-------------------------------------------------------------
	// envelope detector
	//-------------------------------------------------------------
	EnvelopeDetector::EnvelopeDetector( double ms, double sampleRate )
	{
		assert( sampleRate > 0.0 );
		assert( ms > 0.0 );
		sampleRate_ = sampleRate;
		ms_ = ms;
		setCoef();
	}

	//-------------------------------------------------------------
	void EnvelopeDetector::setTc( double ms )
	{
		assert( ms > 0.0 );
		ms_ = ms;
		setCoef();
	}

	//-------------------------------------------------------------
	void EnvelopeDetector::setSampleRate( double sampleRate )
	{
		assert( sampleRate > 0.0 );
		sampleRate_ = sampleRate;
		setCoef();
	}

	//-------------------------------------------------------------
	void EnvelopeDetector::setCoef( void )
	{
		coef_ = exp( -1000.0 / ( ms_ * sampleRate_ ) );
	}

	//-------------------------------------------------------------
	// attack/release envelope
	//-------------------------------------------------------------
	AttRelEnvelope::AttRelEnvelope( double att_ms, double rel_ms, double sampleRate )
		: att_( att_ms, sampleRate )
		, rel_( rel_ms, sampleRate )
	{
	}

	//-------------------------------------------------------------
	void AttRelEnvelope::setAttack( double ms )
	{
		att_.setTc( ms );
	}

	//-------------------------------------------------------------
	void AttRelEnvelope::setRelease( double ms )
	{
		rel_.setTc( ms );
	}

	//-------------------------------------------------------------
	void AttRelEnvelope::setSampleRate( double sampleRate )
	{
		att_.setSampleRate( sampleRate );
		rel_.setSampleRate( sampleRate );
	}

}	// end namespace chunkware_simple
