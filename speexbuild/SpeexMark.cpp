
#include <QtCore>

#ifdef Q_OS_WIN
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#else
#define VALGRIND
#endif

#include <math.h>
#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>
#include <speex/speex_callbacks.h>


#ifdef VALGRIND
#include <valgrind/callgrind.h>
#endif
#include "Timer.h"

int main(int argc, char **argv) {

#ifdef VALGRIND
	CALLGRIND_STOP_INSTRUMENTATION;
	CALLGRIND_ZERO_STATS;
#endif

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
	while(1) {
		QByteArray qba = f.read(iFrameSize * 2);
		if (qba.size() != iFrameSize * 2)
			break;
		v.append(qba);
	}

	int nframes = v.size();

	qWarning("Ready to process %d frames of %d samples", nframes, iFrameSize);

	QVector<short *> sv;

	short tframe[2048];
	for(int i=0;i<iFrameSize;i++)
		tframe[i] = 0;

	for(int i=0;i<nframes;i++) {
		sv.append(reinterpret_cast<short *>(v[i].data()));
	}

	SpeexBits sb;
	speex_bits_init(&sb);

#ifdef Q_OS_WIN
    if (!SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS))
             qWarning("Application: Failed to set priority!");
#endif

	Timer t;
	t.restart();

#ifdef VALGRIND
	CALLGRIND_START_INSTRUMENTATION;
#endif
	for(int i=0;i<nframes-2;i++) {
		speex_bits_reset(&sb);
		speex_echo_cancellation(ses, sv[i], sv[i+2], tframe);
		speex_preprocess_run(spp, tframe);
		speex_encode_int(enc, tframe, &sb);
	}
#ifdef VALGRIND
	CALLGRIND_STOP_INSTRUMENTATION;
#endif

#ifdef Q_OS_WIN
    if (!SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS))
             qWarning("Application: Failed to reset priority!");
#endif

	quint64 e = t.elapsed();

	qWarning("Used %llu usec", e);
	qWarning("%.2f times realtime", (20000ULL * nframes) / (e * 1.0));
}

#include "Timer.cpp"
