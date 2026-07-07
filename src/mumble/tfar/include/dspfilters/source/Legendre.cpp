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
#include "DspFilters/Legendre.h"
#include "DspFilters/RootFinder.h"

#include <sstream>
#include <iostream>
#include <iomanip>

namespace Dsp {

namespace Legendre {

static inline double m_sqrt2 ()
{
  return 1.41421356237309504880;
}

//  Optimum 'L' Filter algorithm.
//  (C) 2004, C. Bond.
//
//  Based on discussion in Kuo, "Network Analysis and Synthesis",
//  pp. 379-383. Original method due to A.Papoulis."On Monotonic
//  Response Filters", Proc. IRE, 47, Feb. 1959.
//
//  Rewritten by Vincent Falco to change the way temporary
//  storage is allocated
//

//
//  This routine calculates the coefficients of the Legendre polynomial
//  of the 1st kind. It uses a recursion relation. The first few polynomials
//  are hard coded and the rest are found by recursion.
//
//  (n+1)Pn+1 = (2n+1)xPn - nPn-1 	Recursion relation.
//
void PolynomialFinderBase::legendre (double *p, int n)
{
  int i,j;

  if (n == 0) {
    p[0] = 1.0;
    return;
  }
  if (n == 1) {
    p[0] = 0.0;
    p[1] = 1.0;
    return;
  }
  p[0] = -0.5;
  p[1] = 0.0;
  p[2] = 1.5;

  if (n == 2) return;

  for (i=0;i<=n;i++) {
    m_aa[i] = m_bb[i] = 0.0;
  }
  m_bb[1] = 1.0;

  for (i=3;i<=n;i++) {
    for (j=0;j<=i;j++) {
      m_aa[j] = m_bb[j];
      m_bb[j] = p[j];
      p[j] = 0.0;
    }
    for (j=i-2;j>=0;j-=2) {
      p[j] -= (i-1)*m_aa[j]/i;
    }
    for (j=i-1;j>=0;j-=2) {
      p[j+1] += (2*i-1)*m_bb[j]/i;
    }
  }
}

//
//
//  In the following routine n = 2k + 1 for odd 'n' and n = 2k + 2 for
//  even 'n'.
//
//
//      n   k
//      -----
//      1   0
//      2   0
//      3   1
//      4   1
//      5   2
//      6   2
//

void PolynomialFinderBase::solve (int n)
{
  assert (n <= m_maxN);

  double c0,c1;
  int i,j,k;

  k = (n-1)/2;
  //
  //  form vector of 'a' constants
  //
  if (n & 1) {                // odd
    for (i=0;i<=k;i++) {
      m_a[i] = (2.0*i+1.0)/(m_sqrt2()*(k+1.0));
    }
  }                           // even
  else {
    for (i=0;i<k+1;i++) {
      m_a[i] = 0.0;
    }
    if (k & 1) {
      for (i=1;i<=k;i+=2) {
        m_a[i] = (2*i+1)/sqrt(double((k+1)*(k+2)));
      }
    }
    else {
      for (i=0;i<=k;i+=2) {
        m_a[i] = (2*i+1)/sqrt(double((k+1)*(k+2)));
      }
    }
  }
  for (i=0;i<=n;i++){
    m_s[i] = 0.0;
    m_w[i] = 0.0;
  }
  //
  // form s[] = sum of a[i]*P[i]
  //
  m_s[0] = m_a[0];
  m_s[1] = m_a[1];
  for (i=2;i<=k;i++) {
    legendre(m_p,i);
    for (j=0;j<=i;j++) {
      m_s[j] += m_a[i]*m_p[j];
    }
  }
  //
  //  form v[] = square of s[]
  //
  for (i=0;i<=2*k+2;i++) {
    m_v[i] = 0.0;
  }
  for (i=0;i<=k;i++) {
    for (j=0;j<=k;j++) {
      m_v[i+j] += m_s[i]*m_s[j];    
    }
  }
  //
  //  modify integrand for even 'n'
  //
  m_v[2*k+1] = 0.0;
  if ((n & 1) == 0) {
    for (i=n;i>=0;i--) {
      m_v[i+1] += m_v[i];
    }
  }
  //
  //  form integral of v[]
  //
  for (i=n+1;i>=0;i--) {
    m_v[i+1] = m_v[i]/(double)(i+1.0);
  }
  m_v[0] = 0.0;
  //
  // clear s[] for use in computing definite integral
  //
  for (i=0;i<(n+2);i++){ 
    m_s[i] = 0.0;
  }
  m_s[0] = -1.0;
  m_s[1] = 2.0;
  //
  //  calculate definite integral
  //
  for (i=1;i<=n;i++) {
    if (i > 1) {
      c0 = -m_s[0];
      for (j=1;j<i+1;j++) {
        c1 = -m_s[j] + 2.0*m_s[j-1];
        m_s[j-1] = c0;
        c0 = c1;
      }
      c1 = 2.0*m_s[i];
      m_s[i] = c0;
      m_s[i+1] = c1;
    }
    for (j=i;j>0;j--) {
      m_w[j] += (m_v[i]*m_s[j]);
    }
  }
  if ((n & 1) == 0) m_w[1] = 0.0;
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

    PolynomialFinderBase& poly (w->poly);
    RootFinderBase& poles (w->roots);

    poly.solve (numPoles);
    int degree = numPoles * 2;

    poles.coef()[0] = 1 + poly.coef()[0];
    poles.coef()[1] = 0;
    for (int i = 1; i <= degree; ++i)
    {
      poles.coef()[2*i] = poly.coef()[i] * ((i & 1) ? -1 : 1);
      poles.coef()[2*i+1] = 0;
    }
    poles.solve (degree);

    int j = 0;
    for (int i = 0; i < degree; ++i)
      if (poles.root()[i].real() <= 0)
        poles.root()[j++] = poles.root()[i];
    // sort descending imag() and cut degree in half
    poles.sort (degree/2);

    const int pairs = numPoles / 2;
    for (int i = 0; i < pairs; ++i)
    {
      complex_t c = poles.root()[i];
      addPoleZeroConjugatePairs (c, infinity());
    }

    if (numPoles & 1)
      add (poles.root()[pairs].real(), infinity());
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

}

}
