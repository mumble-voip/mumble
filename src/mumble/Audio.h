/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _AUDIO_H
#define _AUDIO_H

#include "mumble_pch.hpp"
#include "ClientUser.h"

#define SAMPLE_RATE 48000

typedef QPair<QString,QVariant> audioDevice;

class CELTCodec {
	private:
		Q_DISABLE_COPY(CELTCodec)
	protected:
		const CELTMode *cmMode;
		QString qsVersion;
		QLibrary qlCELT;
		bool bValid;
		mutable int iBitstreamVersion;

		void (*celt_mode_destroy)(CELTMode *mode);
		int (__cdecl *celt_mode_info)(const CELTMode *mode, int request, celt_int32 *value);

	public:
		void (__cdecl *celt_encoder_destroy)(CELTEncoder *st);
		int (__cdecl *celt_encoder_ctl)(CELTEncoder * st, int request, ...);

		void (__cdecl *celt_decoder_destroy)(CELTDecoder *st);
		int (__cdecl *celt_decoder_ctl)(CELTDecoder * st, int request, ...);

		CELTCodec(const QString &version);
		virtual ~CELTCodec();
		bool isValid() const;
		int bitstreamVersion() const;
		QString version() const;

		virtual void report() const;

		virtual CELTEncoder *encoderCreate() = 0;
		virtual CELTDecoder *decoderCreate() = 0;
		virtual int encode(CELTEncoder *st, const celt_int16 *pcm, unsigned char *compressed, int nbCompressedBytes) = 0;
		virtual int decode_float(CELTDecoder *st, const unsigned char *data, int len, float *pcm) = 0;
};

class CELTCodec070 : public CELTCodec {
	protected:
		CELTMode *(*celt_mode_create)(celt_int32 Fs, int frame_size, int *error);
		CELTEncoder *(__cdecl *celt_encoder_create)(const CELTMode *mode, int channels, int *error);
		CELTDecoder *(__cdecl *celt_decoder_create)(const CELTMode *mode, int channels, int *error);
		int (__cdecl *celt_encode_float)(CELTEncoder *st, const float *pcm, float *optional_synthesis, unsigned char *compressed, int nbCompressedBytes);
		int (__cdecl *celt_encode)(CELTEncoder *st, const celt_int16 *pcm, celt_int16 *optional_synthesis, unsigned char *compressed, int nbCompressedBytes);
		int (__cdecl *celt_decode_float)(CELTDecoder *st, const unsigned char *data, int len, float *pcm);
		int (__cdecl *celt_decode)(CELTDecoder *st, const unsigned char *data, int len, celt_int16 *pcm);
		const char *(__cdecl *celt_strerror)(int error);
	public:
		CELTCodec070(const QString &version);
		virtual CELTEncoder *encoderCreate();
		virtual CELTDecoder *decoderCreate();
		virtual int encode(CELTEncoder *st, const celt_int16 *pcm, unsigned char *compressed, int nbCompressedBytes);
		virtual int decode_float(CELTDecoder *st, const unsigned char *data, int len, float *pcm);
};

class CELTCodec080 : public CELTCodec {
	protected:
		CELTMode *(*celt_mode_create)(celt_int32 Fs, int frame_size, int *error);
		CELTEncoder *(__cdecl *celt_encoder_create)(const CELTMode *mode, int channels, int *error);
		CELTDecoder *(__cdecl *celt_decoder_create)(const CELTMode *mode, int channels, int *error);
		int (__cdecl *celt_encode_float)(CELTEncoder *st, const float *pcm, int frame_size, unsigned char *compressed, int nbCompressedBytes);
		int (__cdecl *celt_encode)(CELTEncoder *st, const celt_int16 *pcm, int frame_size, unsigned char *compressed, int nbCompressedBytes);
		int (__cdecl *celt_decode_float)(CELTDecoder *st, const unsigned char *data, int len, float *pcm, int frame_size);
		int (__cdecl *celt_decode)(CELTDecoder *st, const unsigned char *data, int len, celt_int16 *pcm, int frame_size);
		const char *(__cdecl *celt_strerror)(int error);
	public:
		CELTCodec080(const QString &version);
		virtual CELTEncoder *encoderCreate();
		virtual CELTDecoder *decoderCreate();
		virtual int encode(CELTEncoder *st, const celt_int16 *pcm, unsigned char *compressed, int nbCompressedBytes);
		virtual int decode_float(CELTDecoder *st, const unsigned char *data, int len, float *pcm);
};

class CELTCodec011 : public CELTCodec {
	protected:
		CELTMode *(*celt_mode_create)(celt_int32 Fs, int frame_size, int *error);
		CELTEncoder *(__cdecl *celt_encoder_create_custom)(const CELTMode *mode, int channels, int *error);
		CELTDecoder *(__cdecl *celt_decoder_create_custom)(const CELTMode *mode, int channels, int *error);
		int (__cdecl *celt_encode_float)(CELTEncoder *st, const float *pcm, int frame_size, unsigned char *compressed, int nbCompressedBytes);
		int (__cdecl *celt_encode)(CELTEncoder *st, const celt_int16 *pcm, int frame_size, unsigned char *compressed, int nbCompressedBytes);
		int (__cdecl *celt_decode_float)(CELTDecoder *st, const unsigned char *data, int len, float *pcm, int frame_size);
		int (__cdecl *celt_decode)(CELTDecoder *st, const unsigned char *data, int len, celt_int16 *pcm, int frame_size);
		const char *(__cdecl *celt_strerror)(int error);
	public:
		CELTCodec011(const QString &version);
		virtual CELTEncoder *encoderCreate();
		virtual CELTDecoder *decoderCreate();
		virtual int encode(CELTEncoder *st, const celt_int16 *pcm, unsigned char *compressed, int nbCompressedBytes);
		virtual int decode_float(CELTDecoder *st, const unsigned char *data, int len, float *pcm);
};

class LoopUser : public ClientUser {
	private:
		Q_DISABLE_COPY(LoopUser)
	protected:
		QMutex qmLock;
		QTime qtTicker;
		QTime qtLastFetch;
		QMultiMap<float, QByteArray> qmPackets;
		LoopUser();
	public:
		static LoopUser lpLoopy;
		virtual void addFrame(const QByteArray &packet);
		void fetchFrames();
};

class RecordUser : public LoopUser {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(RecordUser)
	public:
		RecordUser();
		~RecordUser();
		virtual void addFrame(const QByteArray &packet);
};

namespace Audio {
	void startInput(const QString &input = QString());
	void stopInput();

	void startOutput(const QString &output = QString());
	void stopOutput();

	void start(const QString &input = QString(), const QString &output = QString());
	void stop();
}

#endif
