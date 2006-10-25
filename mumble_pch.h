#ifdef WIN32
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#else
#define CopyMemory(dst,ptr,len) memcpy(dst,ptr,len)
#define ZeroMemory(ptr,len) memset(ptr, 0, len)
#define __cdecl
#endif

#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtOpenGL>
#include <QtSql>
#include <speex/speex.h>

#include <math.h>
#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifndef JITTER_BUFFER_OK
#define SPEEX_ANCIENT
#endif

#ifdef SPEEX_ANCIENT
typedef SpeexPreprocessState CloneSpeexPreprocessState;
#else
typedef struct {
   /* Basic info */
   int    frame_size;        /**< Number of samples processed each time */
   int    ps_size;           /**< Number of points in the power spectrum */
   int    sampling_rate;     /**< Sampling rate of the input/output */
   int    nbands;
   void *bank;

   /* Parameters */
   int    denoise_enabled;
   int    agc_enabled;
   float  agc_level;
   int    vad_enabled;
   int    dereverb_enabled;
   float  reverb_decay;
   float  reverb_level;
   float  speech_prob_start;
   float  speech_prob_continue;
   int    noise_suppress;
   int    echo_suppress;
   int    echo_suppress_active;

   /* DSP-related arrays */
   float *frame;             /**< Processing frame (2*ps_size) */
   float *ft;                /**< Processing frame in freq domain (2*ps_size) */
   float *ps;                /**< Current power spectrum */
   float *gain2;             /**< Adjusted gains */
   float *gain_floor;        /**< Minimum gain allowed */
   float *window;            /**< Analysis/Synthesis window */
   float *noise;             /**< Noise estimate */
   float *reverb_estimate;   /**< Estimate of reverb energy */
   float *old_ps;            /**< Power spectrum for last frame */
   float *gain;              /**< Ephraim Malah gain */
   float *prior;             /**< A-priori SNR */
   float *post;              /**< A-posteriori SNR */

   float *S;                 /**< Smoothed power spectrum */
   float *Smin;              /**< See Cohen paper */
   float *Stmp;              /**< See Cohen paper */
   float *update_prob;       /**< Propability of speech presence for noise update */

   float *zeta;              /**< Smoothed a priori SNR */

   float *loudness_weight;   /**< Perceptual loudness curve */

   float *echo_noise;

   /* Misc */
   float *inbuf;             /**< Input buffer (overlapped analysis) */
   float *outbuf;            /**< Output buffer (for overlap and add) */

   int    was_speech;
   float  loudness;          /**< loudness estimate */
   float  loudness2;         /**< loudness estimate */
   int    nb_adapt;          /**< Number of frames used for adaptation so far */
   int    nb_loudness_adapt; /**< Number of frames used for loudness adaptation so far */
   int    min_count;         /**< Number of frames processed so far */
   void  *fft_lookup;        /**< Lookup table for the FFT */
} CloneSpeexPreprocessState;
#endif

#ifndef WIN32
typedef WId HWND;
#endif
