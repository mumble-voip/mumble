#ifndef MDCT_CONTEXT_H__
#define MDCT_CONTEXT_H__

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


#include <clunk/fft_context.h>
#include <clunk/clunk_assert.h>
#include <string.h>

namespace clunk {

template<int N, typename T, typename impl>
struct window_func_base {
	window_func_base() {
		for(int i = 0; i < N; ++i) {
			cache[i] = static_cast<impl &>(*this)(i);
		}
	}

	T cache[N];
};


template<int BITS, template <int, typename> class window_func_type , typename T = float> 
class mdct_context {
private:
	typedef fft_context<BITS - 2, T> fft_type;
	fft_type fft;

public:
	enum { N = 1 << BITS , M = N / 2, N2 = M, N4 =  fft_type::N };

private:
	const window_func_type<N, T> window_func;

	std::complex<T> angle_cache[N4];
	T sqrt_N;

	clunk_static_assert(N == N4 * 4);

public:
	typedef T value_type;
	typedef std::complex<T> complex_type;

	T data[N];

	mdct_context() : window_func(), sqrt_N((T)sqrt((T)N)), data() {
		for(unsigned t = 0; t < N4; ++t) {
			angle_cache[t] = std::polar<T>(1, 2 * T(M_PI) * (t + T(0.125)) / N);
		}
	}
	
	void mdct() {
		T rotate[N];
		for(unsigned t = 0; t < N4; ++t) {
			rotate[t] = -data[t + 3 * N4];
		}
		for(unsigned t = N4; t < N; ++t) {
			rotate[t] = data[t - N4];
		}

		for(unsigned t = 0; t < N4; ++t) {
			T re = (rotate[t * 2] - rotate[N - 1 - t * 2]) / 2;
			T im = (rotate[M + t * 2] - rotate[M - 1 - t * 2]) / -2;
			const std::complex<T> & a = angle_cache[t];
			fft.data[t] = std::complex<T>(re * a.real() + im * a.imag(), -re * a.imag() + im * a.real());
		}
		fft.fft();

		for(unsigned t = 0; t < N4; ++t) {
			const std::complex<T>& a = angle_cache[t];
			std::complex<T> &f = fft.data[t];
			f = std::complex<T>(
				2 / sqrt_N * (f.real() * a.real() + f.imag() * a.imag()),
				2 / sqrt_N * (-f.real() * a.imag() + f.imag() * a.real())
			);
		}

		for(unsigned t = 0; t < N4; ++t) {
			data[2 * t] = fft.data[t].real();
			data[M - 2 * t - 1] = -fft.data[t].imag();
		}
	}
	
	void imdct() {
		for(unsigned t = 0; t < N4; ++t) {
			T re = data[t * 2] / 2, im = data[M - 1 - t * 2] / 2;
			std::complex<T> a = angle_cache[t];
			fft.data[t] = std::complex<T>(re * a.real() + im * a.imag(), - re * a.imag() + im * a.real());
		}

		fft.fft();

		for(unsigned t = 0; t < N4; ++t) {
			std::complex<T> a = angle_cache[t];
			std::complex<T>& f = fft.data[t];
			fft.data[t] = std::complex<T>(
				8 / sqrt_N * (f.real() * a.real() + f.imag() * a.imag()),
				8 / sqrt_N * (-f.real() * a.imag() + f.imag() * a.real())
			);
		}

		T rotate[N];
		for(unsigned t = 0; t < N4; ++t) {
			const std::complex<T> &f = fft.data[t];
			rotate[2 * t] = f.real();
			rotate[M + 2 * t] = f.imag();
		}
		for(unsigned t = 1; t < N; t += 2) {
			rotate[t] = - rotate[N - t - 1];
		}

		//shift
		for(unsigned t = 0; t < 3 * N4; ++t) {
			data[t] = rotate[t + N4];
		}
		for(unsigned t = 3 * N4; t < N; ++t) {
			data[t] = -rotate[t - 3 * N4];
		}
	}
	
	void apply_window() {
		for(int i = 0; i < N; ++i) {
			data[i] *= window_func.cache[i];
		}
	}
	
	void clear() {
		memset(data, 0, sizeof(data));
	}
};

}

#endif
