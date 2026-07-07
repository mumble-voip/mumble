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

#ifndef DSPFILTERS_ROOTFINDER_H
#define DSPFILTERS_ROOTFINDER_H

#include "DspFilters/Common.h"
#include "DspFilters/Types.h"

namespace Dsp {

//
// Finds the complex roots of the given polynomial with
// complex-valued coefficients using a numerical method.
//

class RootFinderBase
{
public:
  struct Array
  {
    Array (int max, complex_t* values)
     // : m_max (max)
     // , m_values (values)
    {
    }

    //complex_t& operator[] (int index)
    //{
    //};
  };

  //
  // Find roots of polynomial f(x)=a[0]+a[1]*x+a[2]*x^2...+a[degree]*x^degree
  // The input coefficients are set using coef()[].
  // The solutions are placed in roots.
  //
  void solve (int degree,
              bool polish = true,
              bool doSort = true);

  // Evaluates the polynomial at x
  complex_t eval (int degree,
                  const complex_t& x);

  // Direct access to the input coefficient array of size degree+1.
  complex_t* coef()
  {
    return m_a;
  }

  // Direct access to the resulting roots array of size degree
  complex_t* root()
  {
    return m_root;
  }

  // sort the roots by descending imaginary part
  void sort (int degree);

private:
  // Improves x as a root using Laguerre's method.
  // The input coefficient array has degree+1 elements.
  void laguerre (int degree,
                 complex_t a[],
                 complex_t& x,
                 int& its);

protected:
  int m_maxdegree;
  complex_t* m_a;		// input coefficients (m_maxdegree+1 elements)
  complex_t* m_ad;	// copy of deflating coefficients
  complex_t* m_root; // array of roots (maxdegree elements)
};

//------------------------------------------------------------------------------

template<int maxdegree>
struct RootFinder : RootFinderBase
{
  RootFinder()
  {
    m_maxdegree = maxdegree;
    m_a  = m_a0;
    m_ad = m_ad0;
    m_root = m_r;
  }

private:
  complex_t m_a0 [maxdegree+1];
  complex_t m_ad0[maxdegree+1];
  complex_t m_r  [maxdegree];
};

}

#endif
