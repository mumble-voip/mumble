/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vincent Falco

Official project location:
http://code.google.com/p/dspfilterscpp/

See Documentation.cpp for contact information, notes, and bibliography.

--------------------------------------------------------------------------------

License: MIT License (http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2009 by Vincent Falco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*******************************************************************************/

#include "DspFilters/Common.h"
#include "DspFilters/Bessel.h"
#include "DspFilters/RootFinder.h"

namespace Dsp {

namespace Bessel {

// returns fact(n) = n!
static double fact (int n)
{
  if (n == 0)
    return 1;

  double y = n;
  for (double m = n; --m;)
    y *= m;

  return y;
}

// returns the k-th zero based coefficient of the reverse bessel polynomial of degree n
static double reversebessel (int k, int n)
{
  return fact (2 * n - k) / 
    ((fact (n - k) * fact(k)) * pow(2., n - k));
}

//------------------------------------------------------------------------------

AnalogLowPass::AnalogLowPass ()
  : m_numPoles (-1)
{
  setNormal (0, 1);
}

void AnalogLowPass::design (int numPoles,
                            WorkspaceBase* w)
{
  if (m_numPoles != numPoles)
  {
    m_numPoles = numPoles;

    reset ();

    RootFinderBase& solver (w->roots);
    for (int i = 0; i < numPoles + 1; ++i)
      solver.coef()[i] = reversebessel (i, numPoles);
    solver.solve (numPoles);

    const int pairs = numPoles / 2;
    for (int i = 0; i < pairs; ++i)
    {
      complex_t c = solver.root()[i];
      addPoleZeroConjugatePairs (c, infinity());
    }

    if (numPoles & 1)
      add (solver.root()[pairs].real(), infinity());
  }
}

//------------------------------------------------------------------------------

AnalogLowShelf::AnalogLowShelf ()
  : m_numPoles (-1)
{
  setNormal (doublePi, 1);
}

void AnalogLowShelf::design (int numPoles,
                             double gainDb,
                             WorkspaceBase* w)
{
  if (m_numPoles != numPoles ||
      m_gainDb != gainDb)
  {
    m_numPoles = numPoles;
    m_gainDb = gainDb;

    reset ();

    const double G = pow (10., gainDb / 20) - 1;

    RootFinderBase& poles (w->roots);
    for (int i = 0; i < numPoles + 1; ++i)
      poles.coef()[i] = reversebessel (i, numPoles);
    poles.solve (numPoles);

    RootFinder<50> zeros;
    for (int i = 0; i < numPoles + 1; ++i)
      zeros.coef()[i] = reversebessel (i, numPoles);
    double a0 = reversebessel (0, numPoles);
    zeros.coef()[0] += G * a0;
    zeros.solve (numPoles);

    const int pairs = numPoles / 2;
    for (int i = 0; i < pairs; ++i)
    {
      complex_t p = poles.root()[i];
      complex_t z = zeros.root()[i];
      addPoleZeroConjugatePairs (p, z);
    }

    if (numPoles & 1)
      add (poles.root()[pairs].real(), zeros.root()[pairs].real());
  }
}

//------------------------------------------------------------------------------

void LowPassBase::setup (int order,
                         double sampleRate,
                         double cutoffFrequency,
                         WorkspaceBase* w)
{
  m_analogProto.design (order, w);

  LowPassTransform (cutoffFrequency / sampleRate,
                    m_digitalProto,
                    m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void HighPassBase::setup (int order,
                          double sampleRate,
                          double cutoffFrequency,
                          WorkspaceBase* w)
{
  m_analogProto.design (order, w);

  HighPassTransform (cutoffFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void BandPassBase::setup (int order,
                          double sampleRate,
                          double centerFrequency,
                          double widthFrequency,
                          WorkspaceBase* w)
{
  m_analogProto.design (order, w);

  BandPassTransform (centerFrequency / sampleRate,
                     widthFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void BandStopBase::setup (int order,
                          double sampleRate,
                          double centerFrequency,
                          double widthFrequency,
                          WorkspaceBase* w)
{
  m_analogProto.design (order, w);

  BandStopTransform (centerFrequency / sampleRate,
                     widthFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void LowShelfBase::setup (int order,
                          double sampleRate,
                          double cutoffFrequency,
                          double gainDb,
                          WorkspaceBase* w)
{
  m_analogProto.design (order, gainDb, w);

  LowPassTransform (cutoffFrequency / sampleRate,
                    m_digitalProto,
                    m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

}

}
