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
#include "DspFilters/Cascade.h"

namespace Dsp {

Cascade::Cascade ()
  : m_numStages (0)
  , m_maxStages (0)
  , m_stageArray (0)
{
}

void Cascade::setCascadeStorage (const Storage& storage)
{
  m_numStages = 0;
  m_maxStages = storage.maxStages;
  m_stageArray = storage.stageArray;
}

complex_t Cascade::response (double normalizedFrequency) const
{
  double w = 2 * doublePi * normalizedFrequency;
  const complex_t czn1 = std::polar (1., -w);
  const complex_t czn2 = std::polar (1., -2 * w);
  complex_t ch (1);
  complex_t cbot (1);

  const Biquad* stage = m_stageArray;
  for (int i = m_numStages; --i >=0; ++stage)
  {
    complex_t cb (1);
    complex_t ct    (stage->getB0()/stage->getA0());
    ct = addmul (ct, stage->getB1()/stage->getA0(), czn1);
    ct = addmul (ct, stage->getB2()/stage->getA0(), czn2);
    cb = addmul (cb, stage->getA1()/stage->getA0(), czn1);
    cb = addmul (cb, stage->getA2()/stage->getA0(), czn2);
    ch   *= ct;
    cbot *= cb;
  }

  return ch / cbot;
}

std::vector<PoleZeroPair> Cascade::getPoleZeros () const
{
  std::vector<PoleZeroPair> vpz;
  vpz.reserve (m_numStages);

  const Stage* stage = m_stageArray;
  for (int i = m_numStages; --i >=0;)
  {
    BiquadPoleState bps (*stage++);
    assert (!bps.isSinglePole() || i == 0);
    vpz.push_back (bps);
  }

  return vpz;
}

void Cascade::applyScale (double scale)
{
  // For higher order filters it might be helpful
  // to spread this factor between all the stages.
  assert (m_numStages > 0);
  m_stageArray->applyScale (scale);
}

void Cascade::setLayout (const LayoutBase& proto)
{
  const int numPoles = proto.getNumPoles();
  m_numStages = (numPoles + 1)/ 2;
  assert (m_numStages <= m_maxStages);

  Biquad* stage = m_stageArray;
  for (int i = 0; i < m_numStages; ++i, ++stage)
    stage->setPoleZeroPair (proto[i]);
  
  applyScale (proto.getNormalGain() /
              std::abs (response (proto.getNormalW() / (2 * doublePi))));
}

}

