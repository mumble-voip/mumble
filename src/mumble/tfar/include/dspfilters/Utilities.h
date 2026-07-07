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

#ifndef DSPFILTERS_UTILITIES_H
#define DSPFILTERS_UTILITIES_H

#include "DspFilters/Common.h"

namespace Dsp {

/*
 * Utilities
 *
 * These routines are handy for manipulating buffers of samples.
 *
 */

//------------------------------------------------------------------------------

// Add src samples to dest, without clip or overflow checking.
template <class Td,
          class Ts>
void add (int samples,
          Td* dest,
          Ts const* src,
          int destSkip = 0,
          int srcSkip = 0)
{
  if (srcSkip !=0 || destSkip != 0)
  {
    ++srcSkip;
    ++destSkip;
    while (--samples >= 0)
    {
      *dest = static_cast<Td>(*src);
      dest += destSkip;
      src += srcSkip;
    }
  }
  else
  {
    while (--samples >= 0)
      *dest++ += static_cast<Td>(*src++);
  }
}

// Multichannel add
template <typename Td,
          typename Ts>
void add (int channels,
          int samples,
          Td* const* dest,
          Ts const* const* src)
{
  for (int i = channels; --i >= 0;)
    add (samples, dest[i], src[i]);
}

//--------------------------------------------------------------------------

// Copy samples from src to dest, which may not overlap. Performs an implicit
// type conversion if Ts and Td are different (for example, float to double).
template <typename Td,
          typename Ts>
void copy (int samples,
           Td* dest,
           Ts const* src,
           int destSkip = 0,
           int srcSkip = 0)
{
  if (srcSkip != 0)
  {
    if (destSkip != 0)
    {
      ++srcSkip;
      ++destSkip;
      while (--samples >= 0)
      {
        *dest++ = *src++;
        dest += destSkip;
        src += srcSkip;
      }
    }
    else
    {
      ++srcSkip;
      while (--samples >= 0)
      {
        *dest++ = *src++;
        src += srcSkip;
      }
    }
  }
  else if (destSkip != 0)
  {
    ++destSkip;
    while (--samples >= 0)
    {
      *dest = *src++;
      dest += destSkip;
    }
  }
  else
  {
    while (--samples >= 0)
      *dest++ = *src++;
  }
}

// Wrapper that uses memcpy if there is no skip and the types are the same
template <typename Ty>
void copy (int samples,
           Ty* dest,
           Ty const* src,
           int destSkip = 0,
           int srcSkip = 0)
{
  if (destSkip != 0 || srcSkip != 0)
    copy<Ty,Ty> (samples, dest, src, destSkip, srcSkip);
  else
    ::memcpy (dest, src, samples * sizeof(src[0]));
}

// Copy a set of channels from src to dest, with implicit type conversion.
template <typename Td,
          typename Ts>
void copy (int channels,
           int samples,
           Td* const* dest,
           Ts const* const* src,
           int destSkip = 0,
           int srcSkip = 0)
{
  for (int i = channels; --i >= 0;)
    copy (samples, dest[i], src[i], destSkip, srcSkip);
}

//--------------------------------------------------------------------------

// Deinterleave channels. Performs implicit type conversion.
template <typename Td, typename Ts>
void deinterleave (int channels,
                   int samples,
                   Td* const* dest,
                   Ts const* src)
{
  assert (channels > 1);

  switch (channels)
  {
  case 2:
    {
      Td* l = dest[0];
      Td* r = dest[1];
	    int n = (samples + 7) / 8;
	    switch (samples % 8)
      {
	    case 0: do
              {	
                *l++ = *src++; *r++ = *src++;
	    case 7:		*l++ = *src++; *r++ = *src++;
	    case 6:		*l++ = *src++; *r++ = *src++;
	    case 5:		*l++ = *src++; *r++ = *src++;
	    case 4:		*l++ = *src++; *r++ = *src++;
	    case 3:		*l++ = *src++; *r++ = *src++;
	    case 2:		*l++ = *src++; *r++ = *src++;
	    case 1:		*l++ = *src++; *r++ = *src++;
		          }
              while (--n > 0);
	    }
    }
    break;

  default:
    {
      for (int i = channels; --i >= 0;)
        copy (samples, dest[i], src + i, 0, channels - 1);
    }
    break;
  };
}

// Convenience for a stereo pair of channels
template <typename Td,
          typename Ts>
void deinterleave (int samples,
                   Td* left,
                   Td* right,
                   Ts const* src)
{
  Td* dest[2];
  dest[0] = left;
  dest[1] = right;
  deinterleave (2, samples, dest, src);
}

//--------------------------------------------------------------------------

// Fade dest
template <typename Td,
          typename Ty>
void fade (int samples,
           Td* dest,
           Ty start = 0,
           Ty end = 1)
{
  Ty t = start;
  Ty dt = (end - start) / samples;

  while (--samples >= 0)
  {
    *dest++ *= t;
    t += dt;
  }
}

// Fade dest cannels
template <typename Td,
          typename Ty>
void fade (int channels,
           int samples,
           Td* const* dest,
           Ty start = 0,
           Ty end = 1)
{
  for (int i = channels; --i >= 0;)
    fade (samples, dest[i], start, end);
}

// Fade src into dest
template <typename Td,
          typename Ts,
          typename Ty>
void fade (int samples,
           Td* dest,
           Ts const* src,
           Ty start = 0,
           Ty end = 1)
{
  Ty t = start;
  Ty dt = (end - start) / samples;

  while (--samples >= 0)
  {
    *dest++ = static_cast<Td>(*dest + t * (*src++ - *dest));
    t += dt;
  }
}

// Fade src channels into dest channels
template <typename Td,
          typename Ts,
          typename Ty>
void fade (int channels,
           int samples,
           Td* const* dest,
           Ts const* const* src,
           Ty start = 0,
           Ty end = 1)
{
  for (int i = channels; --i >= 0;)
    fade (samples, dest[i], src[i], start, end);
}

//--------------------------------------------------------------------------

// Interleave separate channels from source pointers to destination
// (Destination requires channels*frames samples of storage). Performs
// implicit type conversion.
template <typename Td,
          typename Ts>
void interleave (int channels,
                 size_t samples,
                 Td* dest,
                 Ts const* const* src)
{
  assert (channels>1);

  if (samples==0)
    return;

  switch (channels)
  {
  case 2:
    {
      const Ts* l = src[0];
      const Ts* r = src[1];

      // note that Duff's Device only works when samples>0
      int n = (samples + 7) / 8;
	    switch (samples % 8)
      {
	    case 0: do
              {	
                *dest++ = *l++; *dest++ = *r++;
	    case 7:		*dest++ = *l++; *dest++ = *r++;
	    case 6:		*dest++ = *l++; *dest++ = *r++;
	    case 5:		*dest++ = *l++; *dest++ = *r++;
	    case 4:		*dest++ = *l++; *dest++ = *r++;
	    case 3:		*dest++ = *l++; *dest++ = *r++;
	    case 2:		*dest++ = *l++; *dest++ = *r++;
	    case 1:		*dest++ = *l++; *dest++ = *r++;
		          }
              while (--n > 0);
	    }
    }
    break;

  default:
    {
      for (int i = channels; --i >= 0;)
        copy (samples, dest + i, src[i], channels - 1, 0);
    }
    break;
  };
}

//--------------------------------------------------------------------------

// Convenience for a stereo channel pair
template <typename Td,
          typename Ts>
void interleave (int samples,
                 Td* dest,
                 Ts const* left,
                 Ts const* right)
{
  const Ts* src[2];
  src[0] = left;
  src[1] = right;
  interleave (2, samples, dest, src);
}

//--------------------------------------------------------------------------

// Multiply samples by a constant, without clip or overflow checking.
template <typename Td,
          typename Ty>
void multiply (int samples,
               Td* dest,
               Ty factor,
               int destSkip = 0)
{
  if (destSkip != 0)
  {
    ++destSkip;
    while (--samples >= 0)
    {
      *dest = static_cast<Td>(*dest * factor);
      dest += destSkip;
    }
  }
  else
  {
    while (--samples >= 0)
      *dest++ = static_cast<Td>(*dest * factor);
  }
}

// Multiply a set of channels by a constant.
template <typename Td,
          typename Ty>
void multiply (int channels,
               int samples,
               Td* const* dest,
               Ty factor,
               int destSkip = 0)
{
  for (int i = channels; --i >= 0;)
    multiply (samples, dest[i], factor, destSkip);
}

//--------------------------------------------------------------------------

// Copy samples from src to dest in reversed order. Performs implicit
// type conversion. src and dest may not overlap.
template <typename Td,
          typename Ts>
void reverse (int samples,
              Td* dest,
              Ts const* src,
              int destSkip = 0,
              int srcSkip = 0 )
{
  src += (srcSkip + 1) * samples;

  if (srcSkip != 0 || destSkip == 0)
  {
    ++srcSkip;
    ++destSkip;
    while (--samples >= 0)
    {
      src -= srcSkip;
      *dest = *src;
      dest += destSkip;
    }
  }
  else
  {
    while (--samples >= 0)
      *dest++ = *--src;
  }
}

template <typename Td, typename Ts>
void reverse (int channels, size_t frames, Td* const* dest, const Ts* const* src)
{
  for (int i = channels; --i >= 0;)
    reverse (frames, dest[i], src[i]);
}

//--------------------------------------------------------------------------

template <typename Tn>
void to_mono (int samples, Tn* dest, Tn const* left, Tn const* right)
{
#if 1
  while (samples-- > 0)
    *dest++ = (*left++ + *right++) * Tn(0.70710678118654752440084436210485);
#else
  while (samples-- > 0)
    *dest++ = (*left++ + *right++) * Tn(0.5);
#endif
}

//--------------------------------------------------------------------------

template <typename T>
void validate (int numChannels, int numSamples, T const* const* src)
{
  for (int i = 0; i < numChannels; ++i)
  {
    T const* p = src [i];
    for (int j = numSamples; j > 0; --j)
    {
      T v = *p++;
      assert (v < 2 && v > -2);
    }
  }
}

//--------------------------------------------------------------------------

#if 0
/*
 * this stuff all depends on is_pod which is not always available
 *
 */
namespace detail {

template <typename Ty,
          bool isPod>
struct zero
{
  static void process (int samples,
                       Ty* dest,
                       int destSkip)
  {
    if (destSkip != 0)
    {
      ++destSkip;
      while (--samples >= 0)
      {
        *dest = Ty();
        dest += destSkip;
      }
    }
    else
    {
      std::fill (dest, dest + samples, Ty());
    }
  }
};

template <typename Ty>
struct zero<Ty, true>
{
  static void process (int samples,
                       Ty* dest,
                       int destSkip)
  {
    if (destSkip != 0)
      zero<Ty,false>::process (samples, dest, destSkip);
    else
      ::memset (dest, 0, samples * sizeof(dest[0]));
  }
};

}

// Fill a channel with zeros. This works even if Ty is not a basic type.
template <typename Ty>
void zero (int samples,
           Ty* dest,
           int destSkip = 0)
{
  detail::zero<Ty, tr1::is_pod<Ty>::value>::process (samples, dest, destSkip );
}

#else
// Fill a channel with zeros. This works even if Ty is not a basic type.
template <typename Ty>
void zero (int samples,
           Ty* dest,
           int destSkip = 0)
{
  if (destSkip != 0)
  {
    ++destSkip;
    while (--samples >= 0)
    {
      *dest = Ty();
      dest += destSkip;
    }
  }
  else
  {
    std::fill (dest, dest + samples, Ty());
  }
}

#endif

// Fill a set of channels with zero.
template <typename Ty>
void zero (int channels,
           int samples,
           Ty* const* dest,
           int destSkip = 0)
{
  for (int i = channels; --i >= 0;)
    zero (samples, dest[i], destSkip);
}

//------------------------------------------------------------------------------

// Implementation of Brent's Method provided by
// John D. Cook (http://www.johndcook.com/)
// The return value of Minimize is the minimum of the function f.
// The location where f takes its minimum is returned in the variable minLoc.
// Notation and implementation based on Chapter 5 of Richard Brent's book
// "Algorithms for Minimization Without Derivatives".
//
// Reference:
// http://www.codeproject.com/KB/recipes/one_variable_optimize.aspx?msg=2779038

template <class TFunction>
double BrentMinimize
(
  TFunction& f,	// [in] objective function to minimize
  double leftEnd,	// [in] smaller value of bracketing interval
  double rightEnd,	// [in] larger value of bracketing interval
  double epsilon,	// [in] stopping tolerance
  double& minLoc	// [out] location of minimum
)
{
  double d, e, m, p, q, r, tol, t2, u, v, w, fu, fv, fw, fx;
  static const double c = 0.5*(3.0 - ::std::sqrt(5.0));
  static const double SQRT_DBL_EPSILON = ::std::sqrt(DBL_EPSILON);

  double& a = leftEnd;
  double& b = rightEnd;
  double& x = minLoc;

  v = w = x = a + c*(b - a);
  d = e = 0.0;
  fv = fw = fx = f(x);
  int counter = 0;
loop:
  counter++;
  m = 0.5*(a + b);
  tol = SQRT_DBL_EPSILON*::fabs(x) + epsilon;
  t2 = 2.0*tol;
  // Check stopping criteria
  if (::fabs(x - m) > t2 - 0.5*(b - a))
  {
    p = q = r = 0.0;
    if (::fabs(e) > tol)
    {
      // fit parabola
      r = (x - w)*(fx - fv);
      q = (x - v)*(fx - fw);
      p = (x - v)*q - (x - w)*r;
      q = 2.0*(q - r);
      (q > 0.0) ? p = -p : q = -q;
      r = e;
      e = d;
    }
    if (::fabs(p) < ::fabs(0.5*q*r) && p < q*(a - x) && p < q*(b - x))
    {
      // A parabolic interpolation step
      d = p/q;
      u = x + d;
      // f must not be evaluated too close to a or b
      if (u - a < t2 || b - u < t2)
        d = (x < m) ? tol : -tol;
    }
    else
    {
      // A golden section step
      e = (x < m) ? b : a;
      e -= x;
      d = c*e;
    }
    // f must not be evaluated too close to x
    if (::fabs(d) >= tol)
      u = x + d;
    else if (d > 0.0)
      u = x + tol;
    else
      u = x - tol;
    fu = f(u);
    // Update a, b, v, w, and x
    if (fu <= fx)
    {
      (u < x) ? b = x : a = x;
      v = w;
      fv = fw;
      w = x;
      fw = fx;
      x = u;
      fx = fu;
    }
    else
    {
      (u < x) ? a = u : b = u;
      if (fu <= fw || w == x)
      {
        v = w;
        fv = fw;
        w = u;
        fw = fu;
      }
      else if (fu <= fv || v == x || v == w)
      {
        v = u;
        fv = fu;
      }
    }
    goto loop;  // Yes, the dreaded goto statement. But the code
    // here is faithful to Brent's orginal pseudocode.
  }
  return  fx;
}

//------------------------------------------------------------------------------

// Tracks the peaks in the signal stream using the attack and release parameters
template <int Channels=2, typename Value=float>
class EnvelopeFollower
{
public:
  EnvelopeFollower()
  {
    for (int i = 0; i < Channels; i++)
      m_env[i]=0;
  }

  Value operator[] (int channel) const
  {
    return m_env[channel];
  }

  void Setup (int sampleRate, double attackMs, double releaseMs)
  {
    m_a = pow (0.01, 1.0 / (attackMs * sampleRate * 0.001));
    m_r = pow (0.01, 1.0 / (releaseMs * sampleRate * 0.001));
  }

  void Process (size_t samples, const Value** src)
  {
    for( int i=0; i<Channels; i++ )
    {
      const Value* cur = src[i];

      double e = m_env[i];
      for (int n = samples; n; n--)
      {
        double v = ::fabs(*cur++);
        if (v > e)
          e = m_a * (e - v) + v;
        else
          e = m_r * (e - v) + v;
      }
      m_env[i]=e;
    }
  }

  double m_env[Channels];

protected:
  double m_a;
  double m_r;
};

}

#endif
