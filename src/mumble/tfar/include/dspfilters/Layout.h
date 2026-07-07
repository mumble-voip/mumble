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

#ifndef DSPFILTERS_LAYOUT_H
#define DSPFILTERS_LAYOUT_H

#include "DspFilters/Common.h"
#include "DspFilters/MathSupplement.h"

namespace Dsp {

//
// Describes a filter as a collection of poles and zeros along with
// normalization information to achieve a specified gain at a specified
// frequency. The poles and zeros may lie either in the s or the z plane.
//

// Base uses pointers to reduce template instantiations
class LayoutBase
{
public:
  LayoutBase ()
    : m_numPoles (0)
    , m_maxPoles (0)
  {
  }

  LayoutBase (int maxPoles, PoleZeroPair* pairs)
    : m_numPoles (0)
    , m_maxPoles (maxPoles)
    , m_pair (pairs)
  {
  }

  void setStorage (const LayoutBase& other)
  {
    m_numPoles = 0;
    m_maxPoles = other.m_maxPoles;
    m_pair = other.m_pair;
  }

  void reset ()
  {
    m_numPoles = 0;
  }

  int getNumPoles () const
  {
    return m_numPoles;
  }

  int getMaxPoles () const
  {
    return m_maxPoles;
  }

  void add (const complex_t& pole, const complex_t& zero)
  {
    assert (!(m_numPoles&1)); // single comes last
    assert (!Dsp::is_nan (pole));
    m_pair[m_numPoles/2] = PoleZeroPair (pole, zero);
    ++m_numPoles;
  }

  void addPoleZeroConjugatePairs (const complex_t pole,
                                  const complex_t zero)
  {
    assert (!(m_numPoles&1)); // single comes last
    assert (!Dsp::is_nan (pole));
    m_pair[m_numPoles/2] = PoleZeroPair (
      pole, zero, std::conj (pole), std::conj (zero));
    m_numPoles += 2;
  }

  void add (const ComplexPair& poles, const ComplexPair& zeros)
  {
    assert (!(m_numPoles&1)); // single comes last
    assert (poles.isMatchedPair ());
    assert (zeros.isMatchedPair ());
    m_pair[m_numPoles/2] = PoleZeroPair (poles.first, zeros.first,
                                         poles.second, zeros.second);
    m_numPoles += 2;
  }

  const PoleZeroPair& getPair (int pairIndex) const
  {
    assert (pairIndex >= 0 && pairIndex < (m_numPoles+1)/2);
    return m_pair[pairIndex];
  }

  const PoleZeroPair& operator[] (int pairIndex) const
  {
    return getPair (pairIndex);
  }

  double getNormalW () const
  {
    return m_normalW;
  }

  double getNormalGain () const
  {
    return m_normalGain;
  }

  void setNormal (double w, double g)
  {
    m_normalW = w;
    m_normalGain = g;
  }

private:
  int m_numPoles;
  int m_maxPoles;
  PoleZeroPair* m_pair;
  double m_normalW;
  double m_normalGain;
};

//------------------------------------------------------------------------------

// Storage for Layout
template <int MaxPoles>
class Layout
{
public:
  operator LayoutBase ()
  {
    return LayoutBase (MaxPoles, m_pairs);
  }

private:
  PoleZeroPair m_pairs[(MaxPoles+1)/2];
};

}

#endif
