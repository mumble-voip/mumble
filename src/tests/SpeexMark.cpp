#include <QtCore>
#include <math.h>   
#include <speex/speex.h>
#include <speex/speex_jitter.h>
#include <speex/speex_preprocess.h>
#include <speex/speex_echo.h>
#include <speex/speex_callbacks.h>
#include <valgrind/callgrind.h>

#include "Timer.h"

int main(int argc, char **argv) {

	CALLGRIND_STOP_INSTRUMENTATION;
	CALLGRIND_ZERO_STATS;
	
	QCoreApplication a(argc, argv);
	
	QFile f("wb_male.wav");
	f.open(QIODevice::ReadOnly);
	f.seek(36 + 8);
	
	void *enc = speex_encoder_init(&speex_wb_mode);
	
	int iarg = 1;
	speex_encoder_ctl(enc, SPEEX_SET_VBR, &iarg);
	iarg = 0;
	speex_encoder_ctl(enc, SPEEX_SET_VAD, &iarg);
	speex_encoder_ctl(enc, SPEEX_SET_DTX, &iarg);
	
	float farg = 5.0;
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
	
	
	QVector<QByteArray> v;
	while(1) {
		QByteArray qba = f.read(iFrameSize * 2);
		if (qba.size() != iFrameSize *2) 
			break;
		v.append(qba);
	}
	
	int nframes = v.size();
	
	qWarning("Ready to process %d frames of %d samples", nframes, iFrameSize);

	QVector<short *> sv;
	for(int i=0;i<nframes;i++) {
		sv.append(reinterpret_cast<short *>(v[i].data()));
	}

	SpeexBits sb;
	speex_bits_init(&sb);
	
	Timer t;
	t.restart();
	
	CALLGRIND_START_INSTRUMENTATION;
	for(int i=0;i<nframes;i++) {
		speex_bits_reset(&sb);	
		speex_preprocess(spp, sv[i], NULL);
		speex_encode_int(enc, sv[i], &sb);
	}
	CALLGRIND_STOP_INSTRUMENTATION;
	
	quint64 e = t.elapsed();
	
	qWarning("Used %llu usec", e);
	qWarning("%.2f times realtime", (20000ULL * nframes) / (e * 1.0));
}
