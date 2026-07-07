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
#include "DspFilters/ChebyshevII.h"

namespace Dsp {

namespace ChebyshevII {

// "Chebyshev Filter Properties"
// http://cnx.org/content/m16906/latest/

AnalogLowPass::AnalogLowPass ()
  : m_numPoles (-1)
{
  setNormal (0, 1);
}

void AnalogLowPass::design (int numPoles,
                            double stopBandDb)
{
  if (m_numPoles != numPoles ||
      m_stopBandDb != stopBandDb)
  {
    m_numPoles = numPoles;
    m_stopBandDb = stopBandDb;

    reset ();

    const double eps = std::sqrt (1. / (std::exp (stopBandDb * 0.1 * doubleLn10) - 1));
    const double v0 = asinh (1 / eps) / numPoles;
    const double sinh_v0 = -sinh (v0);
    const double cosh_v0 = cosh (v0);
    const double fn = doublePi / (2 * numPoles);

    int k = 1;
    for (int i = numPoles / 2; --i >= 0; k+=2)
    {
      const double a = sinh_v0 * cos ((k - numPoles) * fn);
      const double b = cosh_v0 * sin ((k - numPoles) * fn);
      const double d2 = a * a + b * b;
      const double im = 1 / cos (k * fn);
      addPoleZeroConjugatePairs (complex_t (a / d2, b / d2),
                                       complex_t (0, im));
    }

    if (numPoles & 1)
    {
      add (1 / sinh_v0, infinity());
    }
  }
}

//------------------------------------------------------------------------------

//
// Chebyshev Type I low pass shelf prototype
// From "High-Order Digital Parametric Equalizer Design"
// Sophocles J. Orfanidis
// http://www.ece.rutgers.edu/~orfanidi/ece521/hpeq.pdf
//

AnalogLowShelf::AnalogLowShelf ()
  : m_numPoles (-1)
{
  setNormal (doublePi, 1);
}

void AnalogLowShelf::design (int numPoles,
                             double gainDb,
                             double stopBandDb)
{
  if (m_numPoles != numPoles ||
      m_stopBandDb != stopBandDb ||
      m_gainDb != gainDb)
  {
    m_numPoles = numPoles;
    m_stopBandDb = stopBandDb;
    m_gainDb = gainDb;

    reset ();

    gainDb = -gainDb;

    if (stopBandDb >= abs(gainDb))
      stopBandDb = abs (gainDb);
    if (gainDb<0)
      stopBandDb = -stopBandDb;

    const double G  = std::pow (10., gainDb / 20.0 );
    const double Gb = std::pow (10., (gainDb - stopBandDb) / 20.0);
    const double G0 = 1;
    const double g0 = pow (G0 , 1. / numPoles);

    double eps;
    if (Gb != G0 )
      eps = sqrt((G*G-Gb*Gb)/(Gb*Gb-G0*G0));
    else
      eps = G-1; // This is surely wrong

    const double b = pow (G/eps+Gb*sqrt(1+1/(eps*eps)), 1./numPoles);
    const double u = log (b / g0);
    const double v = log (pow (1. / eps+sqrt(1+1/(eps*eps)),1./numPoles));
    
    const double sinh_u = sinh (u);
    const double sinh_v = sinh (v);
    const double cosh_u = cosh (u);
    const double cosh_v = cosh (v);
    const double n2 = 2 * numPoles;
    const int pairs = numPoles / 2;
    for (int i = 1; i <= pairs; ++i)
    {
      const double a = doublePi * (2 * i - 1) / n2;
      const double sn = sin (a);
      const double cs = cos (a);
      addPoleZeroConjugatePairs (complex_t (-sn * sinh_u, cs * cosh_u),
                                       complex_t (-sn * sinh_v, cs * cosh_v));
    }

    if (numPoles & 1)
      add (-sinh_u, -sinh_v);
  }
}

//------------------------------------------------------------------------------

void LowPassBase::setup (int order,
                         double sampleRate,
                         double cutoffFrequency,
                         double stopBandDb)
{
  m_analogProto.design (order, stopBandDb);

  LowPassTransform (cutoffFrequency / sampleRate,
                    m_digitalProto,
                    m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void HighPassBase::setup (int order,
                          double sampleRate,
                          double cutoffFrequency,
                          double stopBandDb)
{
  m_analogProto.design (order, stopBandDb);

  HighPassTransform (cutoffFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void BandPassBase::setup (int order,
                          double sampleRate,
                          double centerFrequency,
                          double widthFrequency,
                          double stopBandDb)
{
  m_analogProto.design (order, stopBandDb);

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
                          double stopBandDb)
{
  m_analogProto.design (order, stopBandDb);

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
                          double stopBandDb)
{
  m_analogProto.design (order, gainDb, stopBandDb);

  LowPassTransform (cutoffFrequency / sampleRate,
                    m_digitalProto,
                    m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void HighShelfBase::setup (int order,
                           double sampleRate,
                           double cutoffFrequency,
                           double gainDb,
                           double stopBandDb)
{
  m_analogProto.design (order, gainDb, stopBandDb);

  HighPassTransform (cutoffFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void BandShelfBase::setup (int order,
                           double sampleRate,
                           double centerFrequency,
                           double widthFrequency,
                           double gainDb,
                           double stopBandDb)
{
  m_analogProto.design (order, gainDb, stopBandDb);

  BandPassTransform (centerFrequency / sampleRate,
                     widthFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  m_digitalProto.setNormal (((centerFrequency/sampleRate) < 0.25) ? doublePi : 0, 1);

  Cascade::setLayout (m_digitalProto);
}

}

}
