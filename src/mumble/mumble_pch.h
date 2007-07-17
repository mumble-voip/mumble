#ifdef WIN32
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#else
#define CopyMemory(dst,ptr,len) memcpy(dst,ptr,len)
#define ZeroMemory(ptr,len) memset(ptr, 0, len)
#define __cdecl
#endif

#define QT_NO_DEBUG_STREAM
#define QT_ASCII_CAST_WARNINGS
#define QT_NO_CAST_TO_ASCII
#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtOpenGL>
#include <QtSql>
#include <QtXml>
#include <speex/speex.h>

#include <math.h>
#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>
#include <speex/speex_callbacks.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

/** Speex echo cancellation state; sync from mdf.c in Speex distribution.
  */

typedef float spx_word16_t;
typedef float spx_word32_t;
typedef float spx_float_t;
typedef spx_word32_t spx_mem_t;
/** Speex echo cancellation state. */
struct CloneSpeexEchoState {
	int frame_size;           /**< Number of samples processed each time */
	int window_size;
	int M;
	int cancel_count;
	int adapted;
	int saturated;
	int screwed_up;
	spx_int32_t sampling_rate;
	spx_word16_t spec_average;
	spx_word16_t beta0;
	spx_word16_t beta_max;
	spx_word32_t sum_adapt;
	spx_word16_t leak_estimate;

	spx_word16_t *e;      /* scratch */
	spx_word16_t *x;      /* Far-end input buffer (2N) */
	spx_word16_t *X;      /* Far-end buffer (M+1 frames) in frequency domain */
	spx_word16_t *input;  /* scratch */
	spx_word16_t *y;      /* scratch */
	spx_word16_t *last_y;
	spx_word16_t *Y;      /* scratch */
	spx_word16_t *E;
	spx_word32_t *PHI;    /* scratch */
	spx_word32_t *W;      /* (Background) filter weights */
	spx_word16_t *foreground; /* Foreground filter weights */
	spx_word32_t  Davg1;  /* 1st recursive average of the residual power difference */
	spx_word32_t  Davg2;  /* 2nd recursive average of the residual power difference */
	spx_float_t   Dvar1;  /* Estimated variance of 1st estimator */
	spx_float_t   Dvar2;  /* Estimated variance of 2nd estimator */
	spx_word32_t *power;  /* Power of the far-end signal */
	spx_float_t  *power_1;/* Inverse power of far-end */
	spx_word16_t *wtmp;   /* scratch */
	spx_word32_t *Rf;     /* scratch */
	spx_word32_t *Yf;     /* scratch */
	spx_word32_t *Xf;     /* scratch */
	spx_word32_t *Eh;
	spx_word32_t *Yh;
	spx_float_t   Pey;
	spx_float_t   Pyy;
	spx_word16_t *window;
	spx_word16_t *prop;
	void *fft_table;
	spx_word16_t memX, memD, memE;
	spx_word16_t preemph;
	spx_word16_t notch_radius;
	spx_mem_t notch_mem[2];
};

/** Speex pre-processor state. */
struct CloneSpeexPreprocessState {
	/* Basic info */
	int    frame_size;        /**< Number of samples processed each time */
	int    ps_size;           /**< Number of points in the power spectrum */
	int    sampling_rate;     /**< Sampling rate of the input/output */
	int    nbands;
	void *bank;
	int    denoise_enabled;
	int    vad_enabled;
	int    dereverb_enabled;
	spx_word16_t  reverb_decay;
	spx_word16_t  reverb_level;
	spx_word16_t speech_prob_start;
	spx_word16_t speech_prob_continue;
	int    noise_suppress;
	int    echo_suppress;
	int    echo_suppress_active;
	CloneSpeexEchoState *echo_state;
	spx_word16_t *frame;      /**< Processing frame (2*ps_size) */
	spx_word16_t *ft;         /**< Processing frame in freq domain (2*ps_size) */
	spx_word32_t *ps;         /**< Current power spectrum */
	spx_word16_t *gain2;      /**< Adjusted gains */
	spx_word16_t *gain_floor; /**< Minimum gain allowed */
	spx_word16_t *window;     /**< Analysis/Synthesis window */
	spx_word32_t *noise;      /**< Noise estimate */
	spx_word32_t *reverb_estimate; /**< Estimate of reverb energy */
	spx_word32_t *old_ps;     /**< Power spectrum for last frame */
	spx_word16_t *gain;       /**< Ephraim Malah gain */
	spx_word16_t *prior;      /**< A-priori SNR */
	spx_word16_t *post;       /**< A-posteriori SNR */

	spx_word32_t *S;          /**< Smoothed power spectrum */
	spx_word32_t *Smin;       /**< See Cohen paper */
	spx_word32_t *Stmp;       /**< See Cohen paper */
	int *update_prob;       /**< Propability of speech presence for noise update */

	spx_word16_t *zeta;       /**< Smoothed a priori SNR */
	spx_word32_t *echo_noise;
	spx_word32_t *residual_echo;

	spx_word16_t *inbuf;      /**< Input buffer (overlapped analysis) */
	spx_word16_t *outbuf;     /**< Output buffer (for overlap and add) */

	int    agc_enabled;
	float  agc_level;
	float  loudness_accum;
	float *loudness_weight;   /**< Perceptual loudness curve */
	float  loudness;          /**< Loudness estimate */
	float  agc_gain;          /**< Current AGC gain */
	int    nb_loudness_adapt; /**< Number of frames used for loudness adaptation so far */
	float  max_gain;          /**< Maximum gain allowed */
	float  max_increase_step; /**< Maximum increase in gain from one frame to another */
	float  max_decrease_step; /**< Maximum decrease in gain from one frame to another */
	float  prev_loudness;     /**< Loudness of previous frame */
	float  init_max;          /**< Current gain limit during initialisation */
	int    nb_adapt;          /**< Number of frames used for adaptation so far */
	int    was_speech;
	int    min_count;         /**< Number of frames processed so far */
	void  *fft_lookup;        /**< Lookup table for the FFT */
};


#ifndef WIN32
typedef WId HWND;
#endif
