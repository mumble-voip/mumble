/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "mumble_pch.hpp"

#include "AudioOutputSpeech.h"

#include "Audio.h"
#include "CELTCodec.h"
#include "ClientUser.h"
#include "Global.h"
#include "PacketDataStream.h"

#ifdef USE_OPUS
#include "opus.h"
#endif

AudioOutputSpeech::AudioOutputSpeech(ClientUser *user, unsigned int freq, MessageHandler::UDPMessageType type) : AudioOutputUser(user->qsName) {
	int err;
	p = user;
	umtType = type;
	iMixerFreq = freq;

	cCodec = NULL;
	cdDecoder = NULL;
	dsSpeex = NULL;
	opusState = NULL;

	bHasTerminator = false;
	bStereo = false;

	iSampleRate = SAMPLE_RATE;
	iFrameSize = iSampleRate / 100;
	iAudioBufferSize = iFrameSize;

	if (umtType == MessageHandler::UDPVoiceOpus) {
#ifdef USE_OPUS
		iAudioBufferSize *= 12;
		opusState = opus_decoder_create(iSampleRate, bStereo ? 2 : 1, NULL);
#endif
	} else if (umtType == MessageHandler::UDPVoiceSpeex) {
		speex_bits_init(&sbBits);

		dsSpeex = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_UWB));
		int iArg=1;
		speex_decoder_ctl(dsSpeex, SPEEX_SET_ENH, &iArg);
		speex_decoder_ctl(dsSpeex, SPEEX_GET_FRAME_SIZE, &iFrameSize);
		speex_decoder_ctl(dsSpeex, SPEEX_GET_SAMPLING_RATE, &iSampleRate);
		iAudioBufferSize = iFrameSize;
	}

	iOutputSize = static_cast<unsigned int>(ceilf(static_cast<float>(iAudioBufferSize * iMixerFreq) / static_cast<float>(iSampleRate)));
	if (bStereo) {
		iAudioBufferSize *= 2;
		iOutputSize *= 2;
	}

	srs = NULL;
	fResamplerBuffer = NULL;
	if (iMixerFreq != iSampleRate) {
		srs = speex_resampler_init(bStereo ? 2 : 1, iSampleRate, iMixerFreq, 3, &err);
		fResamplerBuffer = new float[iAudioBufferSize];
	}

	iBufferOffset = iBufferFilled = iLastConsume = 0;
	bLastAlive = true;

	iMissCount = 0;
	iMissedFrames = 0;

	ucFlags = 0xFF;

	jbJitter = jitter_buffer_init(iFrameSize);
	int margin = g.s.iJitterBufferSize * iFrameSize;
	jitter_buffer_ctl(jbJitter, JITTER_BUFFER_SET_MARGIN, &margin);

	fFadeIn = new float[iFrameSize];
	fFadeOut = new float[iFrameSize];

	float mul = static_cast<float>(M_PI / (2.0 * static_cast<double>(iFrameSize)));
	for (unsigned int i=0;i<iFrameSize;++i)
		fFadeIn[i] = fFadeOut[iFrameSize-i-1] = sinf(static_cast<float>(i) * mul);
}

AudioOutputSpeech::~AudioOutputSpeech() {
#ifdef USE_OPUS
	if (opusState)
		opus_decoder_destroy(opusState);
#endif
	if (cdDecoder) {
		cCodec->celt_decoder_destroy(cdDecoder);
	} else if (dsSpeex) {
		speex_bits_destroy(&sbBits);
		speex_decoder_destroy(dsSpeex);
	}

	if (srs)
		speex_resampler_destroy(srs);

	jitter_buffer_destroy(jbJitter);

	delete [] fFadeIn;
	delete [] fFadeOut;
	delete [] fResamplerBuffer;
}

void AudioOutputSpeech::addFrameToBuffer(const QByteArray &qbaPacket, unsigned int iSeq) {
	QMutexLocker lock(&qmJitter);

	if (qbaPacket.size() < 2)
		return;

	PacketDataStream pds(qbaPacket);

	// skip flags
	pds.next();

	int samples = 0;
	if (umtType == MessageHandler::UDPVoiceOpus) {
		int size;
		pds >> size;
		size &= 0x1fff;
		if (size == 0) {
			return;
		}

		const QByteArray &qba = pds.dataBlock(size);
		if (size != qba.size() || !pds.isValid()) {
			return;
		}

		const unsigned char *packet = reinterpret_cast<const unsigned char*>(qba.constData());

#ifdef USE_OPUS
		int frames = opus_packet_get_nb_frames(packet, size);
		samples = frames * opus_packet_get_samples_per_frame(packet, SAMPLE_RATE);
#else
		return;
#endif

		// We can't handle frames which are not a multiple of 10ms.
		Q_ASSERT(samples % iFrameSize == 0);
	} else {
		unsigned int header = 0;

		do {
			header = static_cast<unsigned char>(pds.next());
			samples += iFrameSize;
			pds.skip(header & 0x7f);
		} while ((header & 0x80) && pds.isValid());
	}

	if (pds.isValid()) {
		JitterBufferPacket jbp;
		jbp.data = const_cast<char *>(qbaPacket.constData());
		jbp.len = qbaPacket.size();
		jbp.span = samples;
		jbp.timestamp = iFrameSize * iSeq;

#ifdef REPORT_JITTER
		if (g.s.bUsage && (umtType != MessageHandler::UDPVoiceSpeex) && p && ! p->qsHash.isEmpty() && (p->qlTiming.count() < 3000)) {
			QMutexLocker qml(& p->qmTiming);

			ClientUser::JitterRecord jr;
			jr.iSequence = iSeq;
			jr.iFrames = frames;
			jr.uiElapsed = p->tTiming.restart();

			if (! p->qlTiming.isEmpty()) {
				jr.iFrames -= p->iFrames;
				jr.iSequence -= p->iSequence + p->iFrames;
			}
			p->iFrames = frames;
			p->iSequence = iSeq;

			p->qlTiming.append(jr);
		}
#endif

		jitter_buffer_put(jbJitter, &jbp);
	}
}

bool AudioOutputSpeech::needSamples(unsigned int snum) {
	for (unsigned int i=iLastConsume;i<iBufferFilled;++i)
		pfBuffer[i-iLastConsume]=pfBuffer[i];
	iBufferFilled -= iLastConsume;

	iLastConsume = snum;

	if (iBufferFilled >= snum)
		return bLastAlive;

	float *pOut;
	bool nextalive = bLastAlive;

	while (iBufferFilled < snum) {
		int decodedSamples = iFrameSize;
		resizeBuffer(iBufferFilled + iOutputSize);

		pOut = (srs) ? fResamplerBuffer : (pfBuffer + iBufferFilled);

		if (! bLastAlive) {
			memset(pOut, 0, iFrameSize * sizeof(float));
		} else {
			if (p == &LoopUser::lpLoopy) {
				LoopUser::lpLoopy.fetchFrames();
			}

			int avail = 0;
			int ts = jitter_buffer_get_pointer_timestamp(jbJitter);
			jitter_buffer_ctl(jbJitter, JITTER_BUFFER_GET_AVAILABLE_COUNT, &avail);

			if (p && (ts == 0)) {
				int want = iroundf(p->fAverageAvailable);
				if (avail < want) {
					++iMissCount;
					if (iMissCount < 20) {
						memset(pOut, 0, iFrameSize * sizeof(float));
						goto nextframe;
					}
				}
			}

			if (qlFrames.isEmpty()) {
				QMutexLocker lock(&qmJitter);

				char data[4096];
				JitterBufferPacket jbp;
				jbp.data = data;
				jbp.len = 4096;

				spx_int32_t startofs = 0;

				if (jitter_buffer_get(jbJitter, &jbp, iFrameSize, &startofs) == JITTER_BUFFER_OK) {
					PacketDataStream pds(jbp.data, jbp.len);

					iMissCount = 0;
					ucFlags = static_cast<unsigned char>(pds.next());

					bHasTerminator = false;
					if (umtType == MessageHandler::UDPVoiceOpus) {
						int size;
						pds >> size;

						bHasTerminator = size & 0x2000;
						qlFrames << pds.dataBlock(size & 0x1fff);
					} else {
						unsigned int header = 0;
						do {
							header = static_cast<unsigned int>(pds.next());
							if (header)
								qlFrames << pds.dataBlock(header & 0x7f);
							else
								bHasTerminator = true;
						} while ((header & 0x80) && pds.isValid());
					}

					if (pds.left()) {
						pds >> fPos[0];
						pds >> fPos[1];
						pds >> fPos[2];
					} else {
						fPos[0] = fPos[1] = fPos[2] = 0.0f;
					}

					if (p) {
						float a = static_cast<float>(avail);
						if (avail >= p->fAverageAvailable)
							p->fAverageAvailable = a;
						else
							p->fAverageAvailable *= 0.99f;
					}
				} else {
					jitter_buffer_update_delay(jbJitter, &jbp, NULL);

					iMissCount++;
					if (iMissCount > 10)
						nextalive = false;
				}
			}

			if (! qlFrames.isEmpty()) {
				QByteArray qba = qlFrames.takeFirst();

				if (umtType == MessageHandler::UDPVoiceCELTAlpha || umtType == MessageHandler::UDPVoiceCELTBeta) {
					int wantversion = (umtType == MessageHandler::UDPVoiceCELTAlpha) ? g.iCodecAlpha : g.iCodecBeta;
					if ((p == &LoopUser::lpLoopy) && (! g.qmCodecs.isEmpty())) {
						QMap<int, CELTCodec *>::const_iterator i = g.qmCodecs.constEnd();
						--i;
						wantversion = i.key();
					}
					if (cCodec && (cCodec->bitstreamVersion() != wantversion)) {
						cCodec->celt_decoder_destroy(cdDecoder);
						cdDecoder = NULL;
					}
					if (! cCodec) {
						cCodec = g.qmCodecs.value(wantversion);
						if (cCodec) {
							cdDecoder = cCodec->decoderCreate();
						}
					}
					if (cdDecoder)
						cCodec->decode_float(cdDecoder, qba.isEmpty() ? NULL : reinterpret_cast<const unsigned char *>(qba.constData()), qba.size(), pOut);
					else
						memset(pOut, 0, sizeof(float) * iFrameSize);
				} else if (umtType == MessageHandler::UDPVoiceOpus) {
#ifdef USE_OPUS
					decodedSamples = opus_decode_float(opusState,
					                                   qba.isEmpty() ?
					                                       NULL :
					                                       reinterpret_cast<const unsigned char *>(qba.constData()),
					                                   qba.size(),
					                                   pOut,
					                                   iAudioBufferSize,
					                                   0);
					if (decodedSamples < 0) {
						decodedSamples = iFrameSize;
						memset(pOut, 0, iFrameSize * sizeof(float));
					}
#endif
				} else {
					if (qba.isEmpty()) {
						speex_decode(dsSpeex, NULL, pOut);
					} else {
						speex_bits_read_from(&sbBits, qba.data(), qba.size());
						speex_decode(dsSpeex, &sbBits, pOut);
					}
					for (unsigned int i=0;i<iFrameSize;++i)
						pOut[i] *= (1.0f / 32767.f);
				}

				bool update = true;
				if (p) {
					float &fPowerMax = p->fPowerMax;
					float &fPowerMin = p->fPowerMin;

					float pow = 0.0f;
					for (int i = 0; i < decodedSamples; ++i)
						pow += pOut[i] * pOut[i];
					pow = sqrtf(pow / static_cast<float>(decodedSamples));

					if (pow >= fPowerMax) {
						fPowerMax = pow;
					} else {
						if (pow <= fPowerMin) {
							fPowerMin = pow;
						} else {
							fPowerMax = 0.99f * fPowerMax;
							fPowerMin += 0.0001f * pow;
						}
					}

					update = (pow < (fPowerMin + 0.01f * (fPowerMax - fPowerMin)));
				}
				if (qlFrames.isEmpty() && update)
					jitter_buffer_update_delay(jbJitter, NULL, NULL);

				if (qlFrames.isEmpty() && bHasTerminator)
					nextalive = false;
			} else {
				if (umtType == MessageHandler::UDPVoiceCELTAlpha || umtType == MessageHandler::UDPVoiceCELTBeta) {
					if (cdDecoder)
						cCodec->decode_float(cdDecoder, NULL, 0, pOut);
					else
						memset(pOut, 0, sizeof(float) * iFrameSize);
				} else if (umtType == MessageHandler::UDPVoiceOpus) {
#ifdef USE_OPUS
					decodedSamples = opus_decode_float(opusState, NULL, 0, pOut, iFrameSize, 0);
					if (decodedSamples < 0) {
						decodedSamples = iFrameSize;
						memset(pOut, 0, iFrameSize * sizeof(float));
					}
#endif
				} else {
					speex_decode(dsSpeex, NULL, pOut);
					for (unsigned int i=0;i<iFrameSize;++i)
						pOut[i] *= (1.0f / 32767.f);
				}
			}

			if (! nextalive) {
				for (unsigned int i=0;i<iFrameSize;++i)
					pOut[i] *= fFadeOut[i];
			} else if (ts == 0) {
				for (unsigned int i=0;i<iFrameSize;++i)
					pOut[i] *= fFadeIn[i];
			}

			for (int i = decodedSamples / iFrameSize; i > 0; --i) {
				jitter_buffer_tick(jbJitter);
			}
		}
nextframe:
		spx_uint32_t inlen = decodedSamples;
		spx_uint32_t outlen = static_cast<unsigned int>(ceilf(static_cast<float>(decodedSamples * iMixerFreq) / static_cast<float>(iSampleRate)));
		if (srs && bLastAlive)
			speex_resampler_process_float(srs, 0, fResamplerBuffer, &inlen, pfBuffer + iBufferFilled, &outlen);
		iBufferFilled += outlen;
	}

	if (p) {
		Settings::TalkState ts;
		if (! nextalive)
			ucFlags = 0xFF;
		switch (ucFlags) {
			case 0:
				ts = Settings::Talking;
				break;
			case 1:
				ts = Settings::Shouting;
				break;
			case 0xFF:
				ts = Settings::Passive;
				break;
			default:
				ts = Settings::Whispering;
				break;
		}
		p->setTalking(ts);
	}

	bool tmp = bLastAlive;
	bLastAlive = nextalive;
	return tmp;
}
