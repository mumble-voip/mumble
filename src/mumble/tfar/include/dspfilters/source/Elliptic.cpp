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
#include "DspFilters/Elliptic.h"

namespace Dsp {

namespace Elliptic {

// shit ton of math in here

// approximation to complete elliptic integral of the first kind.
// fast convergence, peak error less than 2e-16.
double Solver::ellipticK (double k)
{
  double m = k*k;
  double a = 1;
  double b = sqrt (1 - m);
  double c = a - b;
  double co;
  do
  {
    co = c;
    c = (a - b) / 2;
    double ao = (a + b) / 2;
    b = sqrt (a*b);
    a = ao;
  }
  while (c<co);
 
  return doublePi / (a + a);
}

//------------------------------------------------------------------------------

AnalogLowPass::AnalogLowPass ()
  : m_numPoles (-1)
{
  setNormal (0, 1);
}

void AnalogLowPass::design (int numPoles,
                            double rippleDb,
                            double rolloff)
{
  if (m_numPoles != numPoles ||
      m_rippleDb != rippleDb ||
      m_rolloff != rolloff)
  {
    m_numPoles = numPoles;
    m_rippleDb = rippleDb;
    m_rolloff = rolloff;

    reset ();

    // calculate
    //const double ep = rippleDb; // passband ripple

    const int n = numPoles;

    double e2=pow(10.,rippleDb/10)-1;
    //double xi = rolloff + 1;
    double xi = 5 * exp (rolloff - 1) + 1;

    m_K = Solver::ellipticK(1/xi);
    m_Kprime = Solver::ellipticK(sqrt(1-1/(xi*xi)));

    int ni = ((n & 1) == 1) ? 0 : 1;
    int i;
    double f[100]; // HACK!!!
    for (i = 1; i <= n/2; i++)
    {
      double u = (2*i-ni)*m_K/n;
      double sn = calcsn(u);
      sn *= 2*doublePi/m_K;
      f[i] = m_zeros[i-1] = 1/sn;
    }
    m_zeros[n/2] = std::numeric_limits<double>::infinity();
    double fb = 1/(2*doublePi);
    m_nin = n % 2;
    m_n2 = n/2;
    for (i = 1; i <= m_n2; i++)
    {
      double x = f[m_n2+1-i];
      m_z1[i] = sqrt(1-1/(x*x));
    }
    double ee = e2;//pow(10., rippleDb/20)-1;
    m_e = sqrt(ee);
    double fbb = fb*fb;
    m_m = m_nin+2*m_n2;
    m_em = 2*(m_m/2);
    double tp = 2*doublePi;
    calcfz();
    calcqz();
    if (m_m > m_em)
      m_c1[2*m_m] = 0;
    for (i = 0; i <= 2*m_m; i += 2)
      m_a1[m_m-i/2] = m_c1[i] + m_d1[i];
    double a0 = findfact(m_m);
    int r = 0;
    while (r < m_em/2)
    {
      r++;
      m_p[r] /= 10;
      m_q1[r] /= 100;
      double d = 1+m_p[r]+m_q1[r];
      m_b1[r] = (1+m_p[r]/2)*fbb/d;
      m_zf1[r] = fb/pow(d, .25);
      m_zq1[r] = 1/sqrt(abs(2*(1-m_b1[r]/(m_zf1[r]*m_zf1[r]))));
      m_zw1[r] = tp*m_zf1[r];

      m_rootR[r] = -.5*m_zw1[r]/m_zq1[r];
      m_rootR[r+m_em/2] = m_rootR[r];
      m_rootI[r] = .5*sqrt(abs(m_zw1[r]*m_zw1[r]/(m_zq1[r]*m_zq1[r]) - 4*m_zw1[r]*m_zw1[r]));
      m_rootI[r+m_em/2] = -m_rootI[r];

      complex_t pole (
        -.5*m_zw1[r]/m_zq1[r],
        .5*sqrt(abs(m_zw1[r]*m_zw1[r]/(m_zq1[r]*m_zq1[r]) - 4*m_zw1[r]*m_zw1[r])));

      complex_t zero (0, m_zeros[r-1]);

      addPoleZeroConjugatePairs (pole, zero);
    }

    if (a0 != 0)
    {
      m_rootR[r+1+m_em/2] = -sqrt(fbb/(.1*a0-1))*tp;
      m_rootI[r+1+m_em/2] = 0;

      add (-sqrt(fbb/(.1*a0-1))*tp, infinity());
    }

    setNormal (0, (numPoles&1) ? 1. : pow (10., -rippleDb / 20.0));
  }
}

// generate the product of (z+s1[i]) for i = 1 .. sn and store it in b1[]
// (i.e. f[z] = b1[0] + b1[1] z + b1[2] z^2 + ... b1[sn] z^sn)
void AnalogLowPass::prodpoly( int sn )
{
  m_b1[0] = m_s1[1];
  m_b1[1] = 1;
  int i, j;
  for (j = 2; j <= sn; j++)
  {
    m_a1[0] = m_s1[j]*m_b1[0];
    for (i = 1; i <= j-1; i++)
      m_a1[i] = m_b1[i-1]+m_s1[j]*m_b1[i];
    for (i = 0; i != j; i++)
      m_b1[i] = m_a1[i];
    m_b1[j] = 1;
  }
}

// determine f(z)^2
void AnalogLowPass::calcfz2( int i )
{
  int ji = 0;
  int jf = 0;
  if (i < m_em+2)
  {
    ji = 0;
    jf = i;
  }
  if (i > m_em)
  {
    ji = i-m_em;
    jf = m_em;
  }
  m_c1[i] = 0;
  int j;
  for(j = ji; j <= jf; j += 2)
    m_c1[i] += m_a1[j]*(m_a1[i-j]*pow(10., m_m-i/2));
}

// calculate f(z)
void AnalogLowPass::calcfz( void )
{
  int i = 1;
  if( m_nin == 1 )
    m_s1[i++] = 1;
  for (; i <= m_nin+m_n2; i++)
    m_s1[i] = m_s1[i+m_n2] = m_z1[i-m_nin];
  prodpoly(m_nin+2*m_n2);
  for (i = 0; i <= m_em; i += 2)
    m_a1[i] = m_e*m_b1[i];
  for (i = 0; i <= 2*m_em; i += 2)
    calcfz2(i);
}

// determine q(z)
void AnalogLowPass::calcqz( void )
{
  int i;
  for (i = 1; i <= m_nin; i++)
    m_s1[i] = -10;
  for (; i <= m_nin+m_n2; i++)
    m_s1[i] = -10*m_z1[i-m_nin]*m_z1[i-m_nin];
  for (; i <= m_nin+2*m_n2; i++)
    m_s1[i] = m_s1[i-m_n2];
  prodpoly(m_m);
  int dd = ((m_nin & 1) == 1) ? -1 : 1;
  for (i = 0; i <= 2*m_m; i += 2)
    m_d1[i] = dd*m_b1[i/2];
}

// compute factors
double AnalogLowPass::findfact(int t)
{
  int i;
  double a = 0;
  for (i = 1; i <= t; i++)
    m_a1[i] /= m_a1[0];
  m_a1[0] = m_b1[0] = m_c1[0] = 1;
  int i1 = 0;
  for(;;)
  {
    if (t <= 2)
      break;
    double p0 = 0, q0 = 0;
    i1++;
    for(;;)
    {
      m_b1[1] = m_a1[1] - p0;
      m_c1[1] = m_b1[1] - p0;
      for (i = 2; i <= t; i++)
        m_b1[i] = m_a1[i]-p0*m_b1[i-1]-q0*m_b1[i-2];
      for (i = 2; i < t; i++)
        m_c1[i] = m_b1[i]-p0*m_c1[i-1]-q0*m_c1[i-2];
      int x1 = t-1;
      int x2 = t-2;
      int x3 = t-3;
      double x4 = m_c1[x2]*m_c1[x2]+m_c1[x3]*(m_b1[x1]-m_c1[x1]);
      if (x4 == 0)
        x4 = 1e-3;
      double ddp = (m_b1[x1]*m_c1[x2]-m_b1[t]*m_c1[x3])/x4;
      p0 += ddp;
      double dq = (m_b1[t]*m_c1[x2]-m_b1[x1]*(m_c1[x1]-m_b1[x1]))/x4;
      q0 += dq;
      if (abs(ddp+dq) < 1e-6)
        break;
    }
    m_p[i1] = p0;
    m_q1[i1] = q0;
    m_a1[1] = m_a1[1]-p0;
    t -= 2;
    for (i = 2; i <= t; i++)
      m_a1[i] -= p0*m_a1[i-1]+q0*m_a1[i-2];
    if (t <= 2)
      break;
  }

  if (t == 2)
  {
    i1++;
    m_p[i1] = m_a1[1];
    m_q1[i1] = m_a1[2];
  }
  if (t == 1)
    a = -m_a1[1];

  return a;
}

double AnalogLowPass::calcsn(double u)
{
  double sn = 0;
  int j;
  // q = modular constant
  double q = exp(-doublePi*m_Kprime/m_K);
  double v = doublePi*.5*u/m_K;
  for (j = 0; ; j++)
  {
    double w = pow(q, j+.5);
    sn += w*sin((2*j+1)*v)/(1-w*w);
    if (w < 1e-7)
      break;
  }
  return sn;
}

//------------------------------------------------------------------------------

void LowPassBase::setup (int order,
                         double sampleRate,
                         double cutoffFrequency,
                         double rippleDb,
                         double rolloff)
{
  m_analogProto.design (order, rippleDb, rolloff);

  LowPassTransform (cutoffFrequency / sampleRate,
                    m_digitalProto,
                    m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void HighPassBase::setup (int order,
                          double sampleRate,
                          double cutoffFrequency,
                          double rippleDb,
                          double rolloff)
{
  m_analogProto.design (order, rippleDb, rolloff);

  HighPassTransform (cutoffFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void BandPassBase::setup (int order,
                          double sampleRate,
                          double centerFrequency,
                          double widthFrequency,
                          double rippleDb,
                          double rolloff)
{
  m_analogProto.design (order, rippleDb, rolloff);

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
                          double rippleDb,
                          double rolloff)
{
  m_analogProto.design (order, rippleDb, rolloff);

  BandStopTransform (centerFrequency / sampleRate,
                     widthFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

}

}
