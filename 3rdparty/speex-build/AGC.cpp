#include <QtCore>

#ifdef Q_OS_WIN
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shellapi.h>
#define CALLGRIND_START_INSTRUMENTATION
#define CALLGRIND_STOP_INSTRUMENTATION
#define CALLGRIND_ZERO_STATS
#else
#include <valgrind/callgrind.h>
#endif

#include <math.h>
#include <speex/speex.h>
#include <speex/speex_preprocess.h>

#include "Timer.h"

template<class T>
static inline double veccomp(const QVector<T> &a, const QVector<T> &b, const char *n) {
	long double gdiff = 0.0;
	if (a.size() != b.size()) {
		qFatal("%s: %d <=> %d", n, a.size(), b.size());
	}
	for (int i=0;i<a.size();++i) {
		double diff = fabs(a[i] - b[i]);
		if (diff > gdiff)
			gdiff = diff;
#ifdef EXACT

		if (a[i] != b[i]) {
#else
		union { T tv;
			uint32_t uv;
		} v1, v2;
		v1.uv = v2.uv = 0;
		v1.tv = a[i];
		v2.tv = b[i];
		if (fabsf(a[i] - b[i]) > 1000) {
			qWarning("%08x %08x %08x", v1.uv, v2.uv, v1.uv ^ v2.uv);
#endif
			qFatal("%s: Offset %d: %.10g <=> %.10g", n, i, static_cast<double>(a[i]), static_cast<double>(b[i]));
		}
	}
	return gdiff;
}

int main(int argc, char **argv) {

	CALLGRIND_STOP_INSTRUMENTATION;
	CALLGRIND_ZERO_STATS;

	QCoreApplication a(argc, argv);

	QFile f((argc >= 2) ? argv[1] : "wb_male.wav");
	if (! f.open(QIODevice::ReadOnly)) {
		qFatal("Failed to open file!");
	}
	f.seek(36 + 8);

	QFile o("output.agc");
	if (! o.open(QIODevice::WriteOnly))
		qFatal("Failed to open out file!");

	QFile vf("verify.agc");
	if (! vf.open(QIODevice::ReadOnly))
		qWarning("No verify!");

	QDataStream out(&o);
	QDataStream verify(&vf);

	static const int iFrameSize = 320;
	int iarg;

	SpeexPreprocessState *spp = speex_preprocess_state_init(iFrameSize, 16000);
	iarg = 0;
	speex_preprocess_ctl(spp, SPEEX_PREPROCESS_SET_VAD, &iarg);
	speex_preprocess_ctl(spp, SPEEX_PREPROCESS_SET_DENOISE, &iarg);
	speex_preprocess_ctl(spp, SPEEX_PREPROCESS_SET_DEREVERB, &iarg);

	iarg = 1;
	speex_preprocess_ctl(spp, SPEEX_PREPROCESS_SET_AGC, &iarg);
	iarg = 21747;
	speex_preprocess_ctl(spp, SPEEX_PREPROCESS_SET_AGC_TARGET, &iarg);


	QVector<QByteArray> v;
	while (1) {
		QByteArray qba = f.read(iFrameSize * sizeof(short));
		if (qba.size() != iFrameSize * sizeof(short))
			break;
		v.append(qba);
	}

	int nframes = v.size();

	qWarning("Ready to process %d frames of %d samples", nframes, iFrameSize);

	QVector<short *> qvIn;
	QVector<short> sIn(nframes * iFrameSize);

	for (int i=0;i<nframes;i++) {
		const short *ptr = reinterpret_cast<const short *>(v[i].constData());
		short *s = sIn.data() + i * iFrameSize;
		for (int j=0;j<iFrameSize;++j)
			s[j] = ptr[j];
		qvIn.append(s);
	}

#ifdef Q_OS_WIN
	if (!SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS))
		qWarning("Application: Failed to set priority!");
#endif

	Timer t;
	t.restart();

	CALLGRIND_START_INSTRUMENTATION;

	for (int i=0;i<nframes;i++) {
		speex_preprocess_run(spp, qvIn[i]);

		int v;
		speex_preprocess_ctl(spp, SPEEX_PREPROCESS_GET_AGC_GAIN, &v);
		qWarning("%d %d", i, v);

	}
	CALLGRIND_STOP_INSTRUMENTATION;

	quint64 e = t.elapsed();

#ifdef Q_OS_WIN
	if (!SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS))
		qWarning("Application: Failed to reset priority!");
#endif

	qWarning("Used %llu usec", e);
	qWarning("%.2f times realtime", (20000ULL * nframes) / (e * 1.0));

	if (! RUNNING_ON_VALGRIND) {

		out << sIn;

		if (vf.isOpen()) {
			QVector<short> vIn;
			verify >> vIn;
			veccomp(vIn, sIn, "AGC");
		}
	}

	return 0;
}
