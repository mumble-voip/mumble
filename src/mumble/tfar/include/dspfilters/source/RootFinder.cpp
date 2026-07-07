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
#include "DspFilters/RootFinder.h"
#include <stdexcept>
#include <algorithm>

namespace Dsp {

void RootFinderBase::solve (int degree,
                            bool polish,
                            bool doSort)
{
  assert (degree <= m_maxdegree);

  const double EPS = 1.0e-30;

  int its;
  complex_t x, b, c;

  int m = degree;

  // copy coefficients
  for (int j = 0; j <= m; ++j)
    m_ad[j] = m_a[j];

  // for each root
  for (int j = m - 1; j >= 0; --j)
  {
    // initial guess at 0
    x = 0.0;
    laguerre (j + 1, m_ad, x, its);
    
    if (fabs (std::imag(x)) <= 2.0 * EPS * fabs (std::real(x)))
      x = complex_t (std::real(x), 0.0);

    m_root[j] = x;

    // deflate
    b = m_ad[j+1];
    for (int jj = j; jj >= 0; --jj)
    {
      c = m_ad[jj];
      m_ad[jj] = b;
      b = x * b + c;
    }
  }

  if (polish)
    for (int j = 0; j < m; ++j)
      laguerre (degree, m_a, m_root[j], its);

  if (doSort)
    sort (degree);
}

void RootFinderBase::sort (int degree)
{
  for (int j = 1; j < degree; ++j)
  {
    complex_t x = m_root[j];

    int i;
    for (i = j - 1; i >= 0; --i )
    {
      if (m_root[i].imag() >= x.imag())
        break;

      m_root[i+1] = m_root[i];
    }

    m_root[i+1] = x;
  }
}

//------------------------------------------------------------------------------

void RootFinderBase::laguerre (int degree,
                           complex_t a[],
                           complex_t& x,
                           int& its)
{
  const int MR = 8, MT = 10, MAXIT = MT * MR;
  const double EPS = std::numeric_limits<double>::epsilon();

  static const double frac[MR + 1] =
    {0.0, 0.5, 0.25, 0.75, 0.13, 0.38, 0.62, 0.88, 1.0};

  complex_t dx, x1, b, d, f, g, h, sq, gp, gm, g2;

  int m = degree;
  for (int iter = 1; iter <= MAXIT; ++iter)
  {
    its = iter;
    b = a[m];
    double err = std::abs(b);
    d = f = 0.0;
    double abx = std::abs(x);
    for (int j = m - 1; j >= 0; --j)
    {
      f = x * f + d;
      d = x * d + b;
      b = x * b + a[j];
      err = std::abs(b) + abx * err;
    }
    err *= EPS;
    if (std::abs(b) <= err)
      return;
    g  = d / b;
    g2 = g * g;
    h  = g2 - 2.0 * f / b;
    
    sq = sqrt (double(m - 1) * (double(m) * h - g2));
    gp = g + sq;
    gm = g - sq;
    
    double abp = std::abs (gp);
    double abm = std::abs (gm);
    if (abp < abm)
      gp = gm;
    dx = std::max(abp, abm) > 0.0 ? double(m) / gp : std::polar (1 + abx, double(iter));
    x1 = x - dx;
    if (x == x1)
      return;
    if (iter % MT != 0)
      x = x1;
    else
      x -= frac[iter / MT] * dx;
  }

  throw std::logic_error ("laguerre failed");
}

//------------------------------------------------------------------------------

complex_t RootFinderBase::eval (int degree,
                            const complex_t& x )
{
  complex_t y;

  if (x != 0.)
  {
    for (int i = 0; i <= degree; ++i)
      y += m_a[i] * pow (x, double(i));
  }
  else
  {
    y = m_a[0];
  }

  return y;
}


}
