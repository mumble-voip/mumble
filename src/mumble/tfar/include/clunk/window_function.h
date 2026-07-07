#ifndef CLUNK_WINDOW_FUNCTION_H
#define	CLUNK_WINDOW_FUNCTION_H

/* clunk - cross-platform 3D audio API built on top SDL library
 * Copyright (C) 2007-2014 Netive Media Group
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <clunk/types.h>

namespace clunk
{

//window function used in ogg/vorbis
template<int N, typename T>
struct vorbis_window_func : public clunk::window_func_base<N, T, vorbis_window_func<N, T> > {
	inline T operator()(int x) const {
		T s = sin(T(M_PI) * (x + (T)0.5) / N);
		return sin(T(M_PI_2) * s * s);
	}
};

template<int N, typename T>
struct sin_window_func : public clunk::window_func_base<N, T, sin_window_func< N, T> > {
	inline T operator()(int x) const {
		return sin(T(M_PI) * (x + (T)0.5) / N);
	}
};

template<int N, typename T>
struct kbd_window_func {
	T cache[N];

	static T BesselI0(T x) {
		T denominator;
		T numerator;
		T z;

		if (x == 0.0) {
		   return 1.0;
		} else {
		   z = x * x;
		   numerator = (z* (z* (z* (z* (z* (z* (z* (z* (z* (z* (z* (z* (z*
						  (z* 0.210580722890567e-22  + 0.380715242345326e-19 ) +
							  0.479440257548300e-16) + 0.435125971262668e-13 ) +
							  0.300931127112960e-10) + 0.160224679395361e-7  ) +
							  0.654858370096785e-5)  + 0.202591084143397e-2  ) +
							  0.463076284721000e0)   + 0.754337328948189e2   ) +
							  0.830792541809429e4)   + 0.571661130563785e6   ) +
							  0.216415572361227e8)   + 0.356644482244025e9   ) +
							  0.144048298227235e10);

		   denominator = (z*(z*(z-0.307646912682801e4)+
							0.347626332405882e7)-0.144048298227235e10);
		}

		return -numerator/denominator;
	}

	kbd_window_func(int alpha = 4) {
		T sumvalue = 0.0;
		for (int i=0; i< N / 2; ++i) {
		   sumvalue += BesselI0(M_PI * alpha * sqrt(1.0 - pow(4.0 * i / N - 1.0, 2)));
		   cache[i] = sumvalue;
		}

		// need to add one more value to the nomalization factor at size/2:
		sumvalue += BesselI0(M_PI * alpha * sqrt(1.0 - pow(4.0 * (N/2) / N - 1.0, 2)));

		// normalize the window and fill in the righthand side of the window:
		for (int i=0; i< N/2; ++i) {
		   cache[i] = sqrt(cache[i] / sumvalue);
		   cache[N - 1 - i] = cache[i];
		}
	}
};

}

#endif
