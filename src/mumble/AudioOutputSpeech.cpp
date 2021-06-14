// Copyright 2011-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioOutputSpeech.h"

#include "Audio.h"
#include "CELTCodec.h"
#ifdef USE_OPUS
#	include "OpusCodec.h"
#endif
#include "ClientUser.h"
#include "PacketDataStream.h"
#include "SpeechFlags.h"
#include "Utils.h"
#include "Global.h"

#include <cmath>

AudioOutputSpeech::AudioOutputSpeech(ClientUser *user, unsigned int freq, MessageHandler::UDPMessageType type,
									 unsigned int systemMaxBufferSize)
	: AudioOutputUser(user->qsName) {
	int err;
	p          = user;
	umtType    = type;
	iMixerFreq = freq;

	cCodec    = nullptr;
	cdDecoder = nullptr;
	dsSpeex   = nullptr;
	oCodec    = nullptr;
	opusState = nullptr;

	bHasTerminator = false;
	bStereo        = false;

	iSampleRate = SAMPLE_RATE;

	// opus's "frame" means different from normal audio term "frame"
	// normally, a frame means a bundle of only one sample from each channel,
	// e.Global::get(). for a stereo stream, ...[LR]LRLRLR.... where the bracket indicates a frame
	// in opus term, a frame means samples that span a period of time, which can be either stereo or mono
	// e.Global::get(). ...[LRLR....LRLR].... or ...[MMMM....MMMM].... for mono stream
	// opus supports frames with: 2.5, 5, 10, 20, 40 or 60 ms of audio data.
	// sample rate / 100 means 10ms mono audio data per frame.
	iFrameSizePerChannel = iFrameSize = iSampleRate / 100; // for mono stream

	if (umtType == MessageHandler::UDPVoiceOpus) {
#ifdef USE_OPUS
		// Always pretend Stereo mode is true by default. since opus will convert mono stream to stereo stream.
		// https://tools.ietf.org/html/rfc6716#section-2.1.2
		bStereo = true;
		oCodec  = Global::get().oCodec;
		if (oCodec) {
			opusState = oCodec->opus_decoder_create(iSampleRate, bStereo ? 2 : 1, nullptr);
			oCodec->opus_decoder_ctl(
				opusState, OPUS_SET_PHASE_INVERSION_DISABLED(1)); // Disable phase inversion for better mono downmix.
		}
#endif
	} else if (umtType == MessageHandler::UDPVoiceSpeex) {
		speex_bits_init(&sbBits);

		dsSpeex  = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_UWB));
		int iArg = 1;
		speex_decoder_ctl(dsSpeex, SPEEX_SET_ENH, &iArg);
		speex_decoder_ctl(dsSpeex, SPEEX_GET_FRAME_SIZE, &iFrameSize);
		speex_decoder_ctl(dsSpeex, SPEEX_GET_SAMPLING_RATE, &iSampleRate);
		iAudioBufferSize = iFrameSize;
	}

	// iAudioBufferSize: size (in unit of float) of the buffer used to store decoded pcm data.
	// For opus, the maximum frame size of a packet is 60ms.
	iAudioBufferSize = iSampleRate * 60 / 1000; // = SampleRate * 60ms = 48000Hz * 0.06s = 2880, ~12KB

	// iBufferSize: size of the buffer to store the resampled audio data.
	// Note that the number of samples in each opus packet can be different from the number of samples the system
	// requests from us each time (this is known as the system's audio buffer size).
	// For example, the maximum size of an opus packet can be 60ms, but the system's audio buffer size is typically
	// ~5ms on my laptop.
	// Whenever the system's audio callback is called, we have two choice:
	//  1. Decode a new opus packet. Then we need a buffer to store unused samples (which don't fit in the system's
	//  buffer),
	//  2. Use unused samples from the buffer (remaining from the last decoded frame).
	// How large should this buffer be? Consider the case in which remaining samples in the buffer can not fill
	// the system's audio buffer. In that case, we need to decode a new opus packet. In the worst case, the buffer size
	// needed is
	//    60ms of new decoded audio data + system's buffer size - 1.
	iOutputSize = static_cast< unsigned int >(
		ceilf(static_cast< float >(iAudioBufferSize * iMixerFreq) / static_cast< float >(iSampleRate)));
	iBufferSize = iOutputSize + systemMaxBufferSize; // -1 has been rounded up

	if (bStereo) {
		iAudioBufferSize *= 2;
		iOutputSize *= 2;
		iBufferSize *= 2;
		iFrameSize *= 2;
	}

	pfBuffer = new float[iBufferSize];

	srs              = nullptr;
	fResamplerBuffer = nullptr;
	if (iMixerFreq != iSampleRate) {
		srs              = speex_resampler_init(bStereo ? 2 : 1, iSampleRate, iMixerFreq, 3, &err);
		fResamplerBuffer = new float[iAudioBufferSize];
	}

	iBufferOffset = iBufferFilled = iLastConsume = 0;
	bLastAlive                                   = true;

	iMissCount    = 0;
	iMissedFrames = 0;

	ucFlags = SpeechFlags::Invalid;

	jbJitter   = jitter_buffer_init(iFrameSize);
	int margin = Global::get().s.iJitterBufferSize * iFrameSize;
	jitter_buffer_ctl(jbJitter, JITTER_BUFFER_SET_MARGIN, &margin);

	fFadeIn  = new float[iFrameSizePerChannel];
	fFadeOut = new float[iFrameSizePerChannel];

	float mul = static_cast< float >(M_PI / (2.0 * static_cast< double >(iFrameSizePerChannel)));
	for (unsigned int i = 0; i < iFrameSizePerChannel; ++i)
		fFadeIn[i] = fFadeOut[iFrameSizePerChannel - i - 1] = sinf(static_cast< float >(i) * mul);
}

AudioOutputSpeech::~AudioOutputSpeech() {
#ifdef USE_OPUS
	if (opusState)
		oCodec->opus_decoder_destroy(opusState);
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

	if (p) {
		p->setTalking(Settings::Passive);
	}

	delete[] fFadeIn;
	delete[] fFadeOut;
	delete[] fResamplerBuffer;
}

void AudioOutputSpeech::addFrameToBuffer(const QByteArray &qbaPacket, unsigned int iSeq) {
	QMutexLocker lock(&qmJitter);

	if (qbaPacket.size() < 2)
		return;

	// Voice data is transmitted through UDP packets and is not formatted by protobuf.
	// Structure is: flags + size + audio data + pos*3
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

		const unsigned char *packet = reinterpret_cast< const unsigned char * >(qba.constData());

#ifdef USE_OPUS
		if (oCodec) {
			samples = oCodec->opus_decoder_get_nb_samples(opusState, packet,
														  size); // this function return samples per channel
			samples *= 2;                                        // since we assume all input stream is stereo.
		}
#else
		return;
#endif

		// We can't handle frames which are not a multiple of 10ms.
		Q_ASSERT(samples % iFrameSize == 0);
	} else {
		// If packet not in opus format
		unsigned int header = 0;

		do {
			header = static_cast< unsigned char >(pds.next());
			samples += iFrameSize;
			pds.skip(header & 0x7f);
		} while ((header & 0x80) && pds.isValid());
	}

	if (pds.isValid()) {
		JitterBufferPacket jbp;
		jbp.data      = const_cast< char * >(qbaPacket.constData());
		jbp.len       = qbaPacket.size();
		jbp.span      = samples;
		jbp.timestamp = iFrameSize * iSeq;

		jitter_buffer_put(jbJitter, &jbp);
	}
}

bool AudioOutputSpeech::prepareSampleBuffer(unsigned int frameCount) {
	unsigned int channels = bStereo ? 2 : 1;
	// Note: all stereo supports are crafted for opus, since other codecs are deprecated and will soon be removed.

	unsigned int sampleCount = frameCount * channels;

	// we can not control exactly how many frames decoder returns
	// so we need a buffer to keep unused frames
	// shift the buffer, remove decoded and played frames
	for (unsigned int i = iLastConsume; i < iBufferFilled; ++i)
		pfBuffer[i - iLastConsume] = pfBuffer[i];

	iBufferFilled -= iLastConsume;

	iLastConsume = sampleCount;

	// Maximum interaural delay is accounted for to prevent audio glitches
	if (iBufferFilled >= sampleCount + INTERAURAL_DELAY)
		return bLastAlive;

	float *pOut;
	bool nextalive = bLastAlive;

	while (iBufferFilled < sampleCount + INTERAURAL_DELAY) {
		int decodedSamples = iFrameSize;
		resizeBuffer(iBufferFilled + iOutputSize + INTERAURAL_DELAY);
		// TODO: allocating memory in the audio callback will crash mumble in some cases.
		//       we need to initialize the buffer with an appropriate size when initializing
		//       this class. See #4250.

		pOut = (srs) ? fResamplerBuffer : (pfBuffer + iBufferFilled);

		if (!bLastAlive) {
			memset(pOut, 0, iFrameSize * sizeof(float));
		} else {
			if (p == &LoopUser::lpLoopy) {
				LoopUser::lpLoopy.fetchFrames();
			}

			int avail = 0;
			int ts    = jitter_buffer_get_pointer_timestamp(jbJitter);
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
				jbp.len  = 4096;

				spx_int32_t startofs = 0;

				if (jitter_buffer_get(jbJitter, &jbp, iFrameSize, &startofs) == JITTER_BUFFER_OK) {
					PacketDataStream pds(jbp.data, jbp.len);
					// pds structure is: flags + size (14-16 terminator + 1-15 size) + audio data + pos*3

					iMissCount = 0;
					ucFlags    = static_cast< unsigned char >(pds.next());

					bHasTerminator = false;
					if (umtType == MessageHandler::UDPVoiceOpus) {
						int size;
						pds >> size;

						bHasTerminator = size & 0x2000;
						qlFrames << pds.dataBlock(size & 0x1fff);
						// if using opus, there will be at most only one element in qlFrames
						// Q_ASSERT(qlFrames.size() == 1);
					} else {
						unsigned int header = 0;
						do {
							header = static_cast< unsigned int >(pds.next());
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
						float a = static_cast< float >(avail);
						if (avail >= p->fAverageAvailable)
							p->fAverageAvailable = a;
						else
							p->fAverageAvailable *= 0.99f;
					}
				} else {
					// Let the jitter buffer know it's the right time to adjust the buffering delay to the network
					// conditions.
					jitter_buffer_update_delay(jbJitter, &jbp, nullptr);

					iMissCount++;
					if (iMissCount > 10)
						nextalive = false;
				}
			}

			if (!qlFrames.isEmpty()) {
				QByteArray qba = qlFrames.takeFirst();

				if (umtType == MessageHandler::UDPVoiceCELTAlpha || umtType == MessageHandler::UDPVoiceCELTBeta) {
					int wantversion = (umtType == MessageHandler::UDPVoiceCELTAlpha) ? Global::get().iCodecAlpha
																					 : Global::get().iCodecBeta;
					if ((p == &LoopUser::lpLoopy) && (!Global::get().qmCodecs.isEmpty())) {
						QMap< int, CELTCodec * >::const_iterator i = Global::get().qmCodecs.constEnd();
						--i;
						wantversion = i.key();
					}
					if (cCodec && (cCodec->bitstreamVersion() != wantversion)) {
						cCodec->celt_decoder_destroy(cdDecoder);
						cdDecoder = nullptr;
					}
					if (!cCodec) {
						cCodec = Global::get().qmCodecs.value(wantversion);
						if (cCodec) {
							cdDecoder = cCodec->decoderCreate();
						}
					}
					if (cdDecoder)
						cCodec->decode_float(cdDecoder,
											 qba.isEmpty() ? nullptr
														   : reinterpret_cast< const unsigned char * >(qba.constData()),
											 qba.size(), pOut);
					else
						memset(pOut, 0, sizeof(float) * iFrameSize);
				} else if (umtType == MessageHandler::UDPVoiceOpus) {
#ifdef USE_OPUS
					if (oCodec) {
						if (qba.isEmpty() || !(p && p->bLocalMute)) {
							// If qba is empty, we have to let Opus know about the packet loss
							// Otherwise if the associated user is not locally muted, we want to decode the audio packet
							// normally in order to be able to play it.
							decodedSamples = oCodec->opus_decode_float(
								opusState,
								qba.isEmpty() ? nullptr : reinterpret_cast< const unsigned char * >(qba.constData()),
								qba.size(), pOut, iAudioBufferSize, 0);
						} else {
							// If the packet is non-empty, but the associated user is locally muted,
							// we don't have to decode the packet. Instead it is enough to know how many
							// samples it contained so that we can then mute the appropriate output length
							decodedSamples = oCodec->opus_packet_get_samples_per_frame(
								reinterpret_cast< const unsigned char * >(qba.constData()), SAMPLE_RATE);
						}

						// The returned sample count we get from the Opus functions refer to samples per channel.
						// Thus in order to get the total amount, we have to multiply by the channel count.
						decodedSamples *= channels;
					}

					if (decodedSamples < 0) {
						decodedSamples = iFrameSize;
						memset(pOut, 0, iFrameSize * sizeof(float));
					}
#endif
				} else if (umtType == MessageHandler::UDPVoiceSpeex) {
					if (qba.isEmpty()) {
						speex_decode(dsSpeex, nullptr, pOut);
					} else {
						speex_bits_read_from(&sbBits, qba.data(), qba.size());
						speex_decode(dsSpeex, &sbBits, pOut);
					}
					for (unsigned int i = 0; i < iFrameSize; ++i)
						pOut[i] *= (1.0f / 32767.f);
				} else {
					qWarning("AudioOutputSpeech: encountered unknown message type %li in prepareSampleBuffer().",
							 static_cast< long >(umtType));
				}

				bool update = true;
				if (p) {
					float &fPowerMax = p->fPowerMax;
					float &fPowerMin = p->fPowerMin;

					float pow = 0.0f;
					for (int i = 0; i < decodedSamples; ++i)
						pow += pOut[i] * pOut[i];
					pow = sqrtf(pow / static_cast< float >(decodedSamples)); // Average over both L and R channel.

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

					update = (pow < (fPowerMin + 0.01f * (fPowerMax - fPowerMin))); // Update jitter buffer when quiet.
				}
				// qlFrames.isEmpty() will always be true if using opus.
				// Q_ASSERT(qlFrames.isEmpty());
				if (qlFrames.isEmpty() && update)
					jitter_buffer_update_delay(jbJitter, nullptr, nullptr);

				if (qlFrames.isEmpty() && bHasTerminator)
					nextalive = false;
			} else {
				if (umtType == MessageHandler::UDPVoiceCELTAlpha || umtType == MessageHandler::UDPVoiceCELTBeta) {
					if (cdDecoder)
						cCodec->decode_float(cdDecoder, nullptr, 0, pOut);
					else
						memset(pOut, 0, sizeof(float) * iFrameSize);
				} else if (umtType == MessageHandler::UDPVoiceOpus) {
#ifdef USE_OPUS
					if (oCodec) {
						decodedSamples = oCodec->opus_decode_float(opusState, nullptr, 0, pOut, iFrameSize, 0);
						decodedSamples *= channels;
					}

					if (decodedSamples < 0) {
						decodedSamples = iFrameSize;
						memset(pOut, 0, iFrameSize * sizeof(float));
					}
#endif
				} else {
					speex_decode(dsSpeex, nullptr, pOut);
					for (unsigned int i = 0; i < iFrameSize; ++i)
						pOut[i] *= (1.0f / 32767.f);
				}
			}

			if (!nextalive) {
				for (unsigned int i = 0; i < static_cast< unsigned int >(iFrameSizePerChannel); ++i) {
					for (unsigned int s = 0; s < channels; ++s)
						pOut[i * channels + s] *= fFadeOut[i];
				}
			} else if (ts == 0) {
				for (unsigned int i = 0; i < static_cast< unsigned int >(iFrameSizePerChannel); ++i) {
					for (unsigned int s = 0; s < channels; ++s)
						pOut[i * channels + s] *= fFadeIn[i];
				}
			}

			for (int i = decodedSamples / iFrameSize; i > 0; --i) {
				jitter_buffer_tick(jbJitter);
			}
		}
	nextframe:
		if (p && p->bLocalMute) {
			// Overwrite the output with zeros as this user is muted
			// NOTE: If Opus is used, then in this case no samples have actually been decoded and thus
			// we don't discard previously done work (in form of decoding the audio stream) by overwriting
			// it with zeros.
			memset(pOut, 0, decodedSamples * sizeof(float));
		}

		spx_uint32_t inlen  = decodedSamples / channels; // per channel
		spx_uint32_t outlen = static_cast< unsigned int >(
			ceilf(static_cast< float >(decodedSamples / channels * iMixerFreq) / static_cast< float >(iSampleRate)));
		if (srs && bLastAlive) {
			if (channels == 1) {
				speex_resampler_process_float(srs, 0, fResamplerBuffer, &inlen, pfBuffer + iBufferFilled, &outlen);
			} else if (channels == 2) {
				speex_resampler_process_interleaved_float(srs, fResamplerBuffer, &inlen, pfBuffer + iBufferFilled,
														  &outlen);
			}
		}
		iBufferFilled += outlen * channels;
	}

	if (p) {
		Settings::TalkState ts;
		if (!nextalive)
			ucFlags = SpeechFlags::Invalid;
		switch (ucFlags) {
			case SpeechFlags::Listen:
				// Fallthrough
			case SpeechFlags::Normal:
				ts = Settings::Talking;
				break;
			case SpeechFlags::Shout:
				ts = Settings::Shouting;
				break;
			case SpeechFlags::Invalid:
				ts = Settings::Passive;
				break;
			default:
				ts = Settings::Whispering;
				break;
		}

		if (ts != Settings::Passive && p->bLocalMute) {
			ts = Settings::MutedTalking;
		}

		p->setTalking(ts);
	}

	bool tmp   = bLastAlive;
	bLastAlive = nextalive;
	return tmp;
}
