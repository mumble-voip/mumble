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
#include "DspFilters/MathSupplement.h"
#include "DspFilters/Biquad.h"

namespace Dsp {

BiquadPoleState::BiquadPoleState (const BiquadBase& s)
{
  const double a0 = s.getA0 ();
  const double a1 = s.getA1 ();
  const double a2 = s.getA2 ();
  const double b0 = s.getB0 ();
  const double b1 = s.getB1 ();
  const double b2 = s.getB2 ();

  if (a2 == 0 && b2 == 0)
  {
    // single pole
    poles.first = -a1;
    zeros.first = -b0 / b1;
    poles.second = 0;
    zeros.second = 0;
  }
  else
  {
    {
      const complex_t c = sqrt (complex_t (a1 * a1 - 4 * a0 * a2, 0));
      double d = 2. * a0;
      poles.first = -(a1 + c) / d;
      poles.second =  (c - a1) / d;
      assert (!poles.is_nan());
    }

    {
      const complex_t c = sqrt (complex_t (
        b1 * b1 - 4 * b0 * b2, 0));
      double d = 2. * b0;
      zeros.first = -(b1 + c) / d;
      zeros.second =  (c - b1) / d;
      assert (!zeros.is_nan());
    }
  }

  gain = b0 / a0;
}

//------------------------------------------------------------------------------

complex_t BiquadBase::response (double normalizedFrequency) const
{
  const double a0 = getA0 ();
  const double a1 = getA1 ();
  const double a2 = getA2 ();
  const double b0 = getB0 ();
  const double b1 = getB1 ();
  const double b2 = getB2 ();

  const double w = 2 * doublePi * normalizedFrequency;
  const complex_t czn1 = std::polar (1., -w);
  const complex_t czn2 = std::polar (1., -2 * w);
  complex_t ch (1);
  complex_t cbot (1);

  complex_t ct (b0/a0);
  complex_t cb (1);
  ct = addmul (ct, b1/a0, czn1);
  ct = addmul (ct, b2/a0, czn2);
  cb = addmul (cb, a1/a0, czn1);
  cb = addmul (cb, a2/a0, czn2);
  ch   *= ct;
  cbot *= cb;

  return ch / cbot;
}

std::vector<PoleZeroPair> BiquadBase::getPoleZeros () const
{
  std::vector<PoleZeroPair> vpz;
  BiquadPoleState bps (*this);
  vpz.push_back (bps);
  return vpz;
}

void BiquadBase::setCoefficients (double a0, double a1, double a2,
                                  double b0, double b1, double b2)
{
  assert (!Dsp::is_nan (a0) && !Dsp::is_nan (a1) && !Dsp::is_nan (a2) &&
          !Dsp::is_nan (b0) && !Dsp::is_nan (b1) && !Dsp::is_nan (b2));

  m_a0 = a0;
  m_a1 = a1/a0;
  m_a2 = a2/a0;
  m_b0 = b0/a0;
  m_b1 = b1/a0;
  m_b2 = b2/a0;
}

void BiquadBase::setOnePole (complex_t pole, complex_t zero)
{
#if 0
  pole = adjust_imag (pole);
  zero = adjust_imag (zero);
#else
  assert (pole.imag() == 0); 
  assert (zero.imag() == 0);
#endif
  
  const double a0 = 1;
  const double a1 = -pole.real();
  const double a2 = 0;
  const double b0 = -zero.real();
  const double b1 = 1;
  const double b2 = 0;

  setCoefficients (a0, a1, a2, b0, b1, b2);
}

void BiquadBase::setTwoPole (complex_t pole1, complex_t zero1,
                             complex_t pole2, complex_t zero2)
{
#if 0
  pole1 = adjust_imag (pole1);
  pole2 = adjust_imag (pole2);
  zero1 = adjust_imag (zero1);
  zero2 = adjust_imag (zero2);
#endif

  const double a0 = 1;
  double a1;
  double a2;

  if (pole1.imag() != 0)
  {
    assert (pole2 == std::conj (pole1));

    a1 = -2 * pole1.real();
    a2 = std::norm (pole1);
  }
  else
  {
    assert (pole2.imag() == 0);

    a1 = -(pole1.real() + pole2.real());
    a2 =   pole1.real() * pole2.real();
  }

  const double b0 = 1;
  double b1;
  double b2;

  if (zero1.imag() != 0)
  {
    assert (zero2 == std::conj (zero1));

    b1 = -2 * zero1.real();
    b2 = std::norm (zero1);
  }
  else
  {
    assert (zero2.imag() == 0);

    b1 = -(zero1.real() + zero2.real());
    b2 =   zero1.real() * zero2.real();
  }

  setCoefficients (a0, a1, a2, b0, b1, b2);
}

void BiquadBase::setPoleZeroForm (const BiquadPoleState& bps)
{
  setPoleZeroPair (bps);
  applyScale (bps.gain);
}

void BiquadBase::setIdentity ()
{
  setCoefficients (1, 0, 0, 1, 0, 0);
}

void BiquadBase::applyScale (double scale)
{
  m_b0 *= scale;
  m_b1 *= scale;
  m_b2 *= scale;
}

//------------------------------------------------------------------------------

Biquad::Biquad ()
{
}

// Construct a second order section from a pair of poles and zeroes
Biquad::Biquad (const BiquadPoleState& bps)
{
  setPoleZeroForm (bps);
}

//------------------------------------------------------------------------------

}
