#ifndef CLUNK_SSE_FFT_CONTEXT_H__
#define CLUNK_SSE_FFT_CONTEXT_H__

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
#include <clunk/clunk_assert.h>
#include <clunk/export_clunk.h>

#ifndef CLUNK_USES_SSE
#	error turn on SSE support with CLUNK_USES_SSE macro
#endif

#ifndef _USE_MATH_DEFINES
#	define _USE_MATH_DEFINES
#endif

#include <xmmintrin.h>
#include <complex>

namespace clunk {

struct CLUNKAPI aligned_allocator {
	static void * allocate(size_t size, size_t alignment);
	static void deallocate(void *ptr);
};

template<typename T, int N, int ALIGNMENT = sizeof(T)>
class aligned_array {
	T * data;
public: 
	aligned_array() : data(static_cast<T *>(aligned_allocator::allocate(sizeof(T) * N, ALIGNMENT))) {}
	operator T*() { return data; }
	operator const T*() const { return data; }
	~aligned_array() { aligned_allocator::deallocate(data); }
};

template<int N, typename T>
struct sse_danielson_lanczos {
	typedef __m128 sse_type;
	enum { SSE_DIV = sizeof(sse_type) / sizeof(T) };

	typedef sse_danielson_lanczos<N / 2, T> next_type;
	next_type next;
	
	aligned_array<sse_type, N / 2> angle_re;
	aligned_array<sse_type, N / 2> angle_im;
	
	sse_danielson_lanczos() {
		T a = (T)(-2 * M_PI / N / SSE_DIV);
		T wtemp = sin(a / 2);
		
		std::complex<T> wp (-2 * wtemp * wtemp, sin(a)), w(1, 0);
		for (unsigned i = 0; i < N / 2 ; ++i) {
			T w_re_buf[SSE_DIV], w_im_buf[SSE_DIV];
			for (unsigned k = 0; k < SSE_DIV; ++k) {
				w_re_buf[k] = w.real();
				w_im_buf[k] = w.imag();
				w += w * wp;
			}

			angle_re[i] = _mm_loadu_ps(w_re_buf);
			angle_im[i] = _mm_loadu_ps(w_im_buf);
		}
	}

	template<int SIGN>
	void apply(sse_type * data_re, sse_type * data_im) {
		next.template apply<SIGN>(data_re, data_im);
		next.template apply<SIGN>(data_re + N / 2, data_im + N / 2);
			
		for (unsigned i = 0; i < N / 2 ; ++i) {
			int j = i + N / 2;
			
			sse_type w_re = angle_re[i], w_im = _mm_mul_ps(_mm_set_ps1(SIGN), angle_im[i]);
			
			sse_type temp_re = _mm_sub_ps(_mm_mul_ps(data_re[j], w_re), _mm_mul_ps(data_im[j], w_im)), 
					 temp_im = _mm_add_ps(_mm_mul_ps(data_im[j], w_re), _mm_mul_ps(data_re[j], w_im));

			data_re[j] = _mm_sub_ps(data_re[i], temp_re);
			data_im[j] = _mm_sub_ps(data_im[i], temp_im);
			data_re[i] = _mm_add_ps(data_re[i], temp_re);
			data_im[i] = _mm_add_ps(data_im[i], temp_im);
		}
	}
};

template<typename T>
struct sse_danielson_lanczos<1, T> {
	typedef __m128 sse_type;
	enum { SSE_DIV = sizeof(sse_type) / sizeof(T) };

	typedef danielson_lanczos<SSE_DIV, T> next_type;

	template<int SIGN>
	static void apply(sse_type * data_re, sse_type * data_im) {
		T re[SSE_DIV], im[SSE_DIV]; 
		_mm_storeu_ps(re, *data_re);
		_mm_storeu_ps(im, *data_im);

		std::complex<T> data[SSE_DIV];
		for(unsigned i = 0; i < SSE_DIV; ++i) {
			data[i] = std::complex<T>(re[i], im[i]);
		}
		
		next_type::template apply<SIGN>(data);

		for(unsigned i = 0; i < SSE_DIV; ++i) {
			re[i] = data[i].real();
			im[i] = data[i].imag();
		}
		
		*data_re = _mm_loadu_ps(re);
		*data_im = _mm_loadu_ps(im);
	}
};

template<int BITS>
class fft_context<BITS, float> {
public: 
	typedef __m128 sse_type;
	enum { N = 1 << BITS };
	enum { SSE_DIV = sizeof(sse_type) / sizeof(float) };
	enum { SSE_N = (N - 1) / SSE_DIV + 1 };

	//clunk_static_assert(SSE_DIV == 4);

private:
	aligned_array<sse_type, SSE_N> data_re;
	aligned_array<sse_type, SSE_N> data_im;

public: 

	typedef std::complex<float> value_type;
	value_type data[N];
	
	inline void fft() {
		scramble(data);
		load();
		next.template apply<1>(data_re, data_im);
		save();
	}

	inline void ifft() {
		scramble(data);
		load();
		next.template apply<-1>(data_re, data_im);

		sse_type n = _mm_set_ps1(N);
		for(unsigned i = 0; i < SSE_N; ++i) {
			data_re[i] = _mm_div_ps(data_re[i], n);
			data_im[i] = _mm_div_ps(data_im[i], n);
		}
		save();
	}

private:
	sse_danielson_lanczos<SSE_N, float> next;

	static void scramble(std::complex<float> * data) {
		int j = 0;
		for(int i = 0; i < N; ++i) {
			if (i > j) {
				std::swap(data[i], data[j]);
			}
			int m = N / 2;
			while(j >= m && m >= 2) {
				j -= m;
				m >>= 1;
			}
			j += m;
		}
	}

	void load() {
		for(int i = 0; i < SSE_N; ++i) {
			float buf_re[SSE_DIV], buf_im[SSE_DIV];
			for(int j = 0; j < SSE_DIV; ++j) {
				int idx = i * SSE_DIV + j; 
				buf_re[j] = (idx < N)? data[idx].real(): 0;
				buf_im[j] = (idx < N)? data[idx].imag(): 0;
			}
			data_re[i] = _mm_loadu_ps(buf_re);
			data_im[i] = _mm_loadu_ps(buf_im);
		}
	}

	void save() {
		for(int i = 0; i < SSE_N; ++i) {
			float buf_re[SSE_DIV], buf_im[SSE_DIV];
			_mm_storeu_ps(buf_re, data_re[i]);
			_mm_storeu_ps(buf_im, data_im[i]);
			
			for(int j = 0; j < SSE_DIV; ++j) {
				int idx = i * SSE_DIV + j;
				if (idx >= N)
					break;
				data[idx] = value_type(buf_re[j], buf_im[j]);
			}
		}
	}

};

}

#endif
