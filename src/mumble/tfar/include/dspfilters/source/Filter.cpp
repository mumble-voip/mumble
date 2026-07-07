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
#include "DspFilters/Filter.h"

namespace Dsp {

Params Filter::getDefaultParams() const
{
  Params params;

  params.clear();

  for (int i = 0; i < getNumParams(); ++i)
    params[i] = getParamInfo(i).getDefaultValue();

  return params;
}

Filter::~Filter()
{
}

int Filter::findParamId (int paramId)
{
  int index = -1;

  for (int i = getNumParams(); --i >= 0;)
  {
    if (getParamInfo (i).getId () == paramId)
    {
      index = i;
      break;
    }
  }

  return index;
}

void Filter::setParamById (int paramId, double nativeValue)
{
  for (int i = getNumParams(); --i >= 0;)
  {
    if (getParamInfo (i).getId () == paramId)
    {
      setParam (i, nativeValue);
      return;
    }
  }
  
  assert (0);
}

void Filter::copyParamsFrom (Dsp::Filter const* other)
{
  // first, set reasonable defaults
  m_params = getDefaultParams ();

  if (other)
  {
    // now loop
    for (int i = 0; i < getNumParams (); ++i)
    {
      const ParamInfo& paramInfo = getParamInfo (i);

      // find a match
      for (int j = 0; j < other->getNumParams(); ++j)
      {
        const ParamInfo& otherParamInfo = other->getParamInfo (j);

        if (paramInfo.getId() == otherParamInfo.getId())
        {
          // match!
          m_params [i] = paramInfo.clamp (other->getParam (j));
          break;
        }
      }
    }
  }

  doSetParams (m_params);
}

}
