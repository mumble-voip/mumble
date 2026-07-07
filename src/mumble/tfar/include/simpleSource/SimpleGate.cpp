/*
 *	Simple Gate (source)
 *
 *  File		: SimpleGate.cpp
 *	Library		: SimpleSource
 *  Version		: 1.12
 *  Implements	: SimpleGate, SimpleGateRms
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


#include "SimpleGate.h"

namespace chunkware_simple
{
	//-------------------------------------------------------------
	SimpleGate::SimpleGate()
		: AttRelEnvelope( 1.0, 100.0 )
		, threshdB_( 0.0 )
		, thresh_( 1.0 )
		, env_( DC_OFFSET )
	{
	}

	//-------------------------------------------------------------
	void SimpleGate::setThresh( double dB )
	{
		threshdB_ = dB;
		thresh_ = dB2lin( dB );
	}

	//-------------------------------------------------------------
	void SimpleGate::initRuntime( void )
	{
		env_ = DC_OFFSET;
	}

	//-------------------------------------------------------------
	// simple gate with RMS detection
	//-------------------------------------------------------------
	SimpleGateRms::SimpleGateRms()
		: ave_( 5.0 )
		, aveOfSqrs_( DC_OFFSET )
	{
	}

	//-------------------------------------------------------------
	void SimpleGateRms::setSampleRate( double sampleRate )
	{
		SimpleGate::setSampleRate( sampleRate );
		ave_.setSampleRate( sampleRate );
	}

	//-------------------------------------------------------------
	void SimpleGateRms::setWindow( double ms )
	{
		ave_.setTc( ms );
	}

	//-------------------------------------------------------------
	void SimpleGateRms::initRuntime( void )
	{
		SimpleGate::initRuntime();
		aveOfSqrs_ = DC_OFFSET;
	}

}	// end namespace chunkware_simple
