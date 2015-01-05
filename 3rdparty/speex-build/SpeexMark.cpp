
#if defined(__INTEL_COMPILER)
#include <mathimf.h>
#else
#include <math.h>
#endif

#include <QtCore>

static const float tfreq1 = 48000.f;
static const float tfreq2 = 44100.f;

#ifdef Q_OS_WIN
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#define CALLGRIND_START_INSTRUMENTATION
#define CALLGRIND_STOP_INSTRUMENTATION
#define CALLGRIND_ZERO_STATS
#define lroundf(x) ( static_cast<long int>( (x) + ((x) >= 0.0f ? 0.5f : -0.5f) ) )
#define lround(x) ( static_cast<long int>( (x) + ((x) >= 0.0 ? 0.5 : -0.5) ) )
#else
#include <valgrind/callgrind.h>
#endif

#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>
#include <speex/speex_callbacks.h>
#include <speex/speex_resampler.h>

#include "Timer.h"

int main(int argc, char **argv) {

	CALLGRIND_STOP_INSTRUMENTATION;
	CALLGRIND_ZERO_STATS;

	QCoreApplication a(argc, argv);

	QFile f((argc >= 2) ? argv[1] : "wb_male.wav");
	if (! f.open(QIODevice::ReadOnly)) {
		qFatal("Failed to open file!");
	}
	f.seek(36 + 8);

	void *enc = speex_encoder_init(&speex_wb_mode);

	int iarg = 1;
	speex_encoder_ctl(enc, SPEEX_SET_VBR, &iarg);
	iarg = 0;
	speex_encoder_ctl(enc, SPEEX_SET_VAD, &iarg);
	speex_encoder_ctl(enc, SPEEX_SET_DTX, &iarg);

	float farg = 6.0;
	speex_encoder_ctl(enc, SPEEX_SET_VBR_QUALITY, &farg);
	speex_encoder_ctl(enc, SPEEX_GET_BITRATE, &iarg);
	speex_encoder_ctl(enc, SPEEX_SET_VBR_MAX_BITRATE, &iarg);

	iarg = 4;
	speex_encoder_ctl(enc, SPEEX_SET_COMPLEXITY, &iarg);

	int iFrameSize;
	speex_encoder_ctl(enc, SPEEX_GET_FRAME_SIZE, &iFrameSize);

	void *dec = speex_decoder_init(&speex_wb_mode);
	iarg = 1;
	speex_decoder_ctl(dec, SPEEX_SET_ENH, &iarg);

	SpeexPreprocessState *spp = speex_preprocess_state_init(iFrameSize, 16000);
	iarg = 1;
	speex_preprocess_ctl(spp, SPEEX_PREPROCESS_SET_VAD, &iarg);
	speex_preprocess_ctl(spp, SPEEX_PREPROCESS_SET_DENOISE, &iarg);
	speex_preprocess_ctl(spp, SPEEX_PREPROCESS_SET_AGC, &iarg);
	speex_preprocess_ctl(spp, SPEEX_PREPROCESS_SET_DEREVERB, &iarg);

	SpeexEchoState *ses = speex_echo_state_init(iFrameSize, iFrameSize * 10);
	iarg = 16000;
	speex_echo_ctl(ses, SPEEX_SET_SAMPLING_RATE, &iarg);
	speex_preprocess_ctl(spp, SPEEX_PREPROCESS_SET_ECHO_STATE, ses);

	QVector<QByteArray> v;
	while (1) {
		QByteArray qba = f.read(iFrameSize * 2);
		if (qba.size() != iFrameSize * 2)
			break;
		v.append(qba);
	}

	int nframes = v.size();

	qWarning("Ready to process %d frames of %d samples", nframes, iFrameSize);

	QVector<short *> sv;

	short tframe[2048];
	for (int i=0;i<iFrameSize;i++)
		tframe[i] = 0;

	for (int i=0;i<nframes;i++) {
		sv.append(reinterpret_cast<short *>(v[i].data()));
	}

	float oframe[2048];
	float resampframe[32768];

	const float sfraq1 = tfreq1 / 16000.0f;
	float fOutSize1 = iFrameSize * sfraq1;
	int iOutSize1 = lroundf(fOutSize1);

	const float sfraq2 = tfreq2 / 16000.0f;
	float fOutSize2 = iFrameSize * sfraq2;
	int iOutSize2 = lroundf(fOutSize2);

	int err;
	SpeexResamplerState *srs1 = speex_resampler_init(1, 16000, lroundf(tfreq1), 3, &err);
	SpeexResamplerState *srs2 = speex_resampler_init(1, 16000, lroundf(tfreq2), 3, &err);

	SpeexBits sb;
	speex_bits_init(&sb);

#ifdef Q_OS_WIN
	if (!SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS))
		qWarning("Application: Failed to set priority!");
#endif

	int len;
	char data[4096];
	spx_uint32_t inlen;
	spx_uint32_t outlen;

	Timer t;
	t.restart();

	nframes = qMin(nframes, 10);

	int iter = 100;

	CALLGRIND_START_INSTRUMENTATION;

	for (int j=0;j<iter;j++) {
		for (int i=0;i<nframes-2;i++) {
			speex_bits_reset(&sb);

			speex_echo_cancellation(ses, sv[i], sv[i+2], tframe);

			speex_preprocess_run(spp, tframe);

			speex_encode_int(enc, tframe, &sb);
			len = speex_bits_nbytes(&sb);
			speex_bits_write(&sb, data, len);

			speex_bits_read_from(&sb, data, len);
			speex_decode(dec, &sb, oframe);

			inlen = iFrameSize;
			outlen = iOutSize1;
			speex_resampler_process_float(srs1, 0, oframe, &inlen, resampframe, &outlen);

			inlen = iFrameSize;
			outlen = iOutSize2;
			speex_resampler_process_float(srs2, 0, oframe, &inlen, resampframe, &outlen);

		}
	}
	CALLGRIND_STOP_INSTRUMENTATION;

	quint64 e = t.elapsed();

#ifdef Q_OS_WIN
	if (!SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS))
		qWarning("Application: Failed to reset priority!");
#endif

	qWarning("Used %llu usec", e);
	qWarning("%.2f times realtime", (20000ULL * nframes * iter) / (e * 1.0));
	return 0;
}
