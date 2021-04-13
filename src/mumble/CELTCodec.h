// Copyright 2012-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CELTCODEC_H_
#define MUMBLE_MUMBLE_CELTCODEC_H_

#include <celt.h>

#include <QtCore/QLibrary>
#include <QtCore/QString>

#ifndef Q_OS_WIN
#	define __cdecl
#endif

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
	int(__cdecl *celt_mode_info)(const CELTMode *mode, int request, celt_int32 *value);

public:
	void(__cdecl *celt_encoder_destroy)(CELTEncoder *st);
	int(__cdecl *celt_encoder_ctl)(CELTEncoder *st, int request, ...);

	void(__cdecl *celt_decoder_destroy)(CELTDecoder *st);
	int(__cdecl *celt_decoder_ctl)(CELTDecoder *st, int request, ...);

	CELTCodec(const QString &celt_version);
	virtual ~CELTCodec();
	bool isValid() const;
	int bitstreamVersion() const;
	QString version() const;

	virtual void report() const;

	virtual CELTEncoder *encoderCreate()                                                                         = 0;
	virtual CELTDecoder *decoderCreate()                                                                         = 0;
	virtual int encode(CELTEncoder *st, const celt_int16 *pcm, unsigned char *compressed, int nbCompressedBytes) = 0;
	virtual int decode_float(CELTDecoder *st, const unsigned char *data, int len, float *pcm)                    = 0;
};

class CELTCodec070 : public CELTCodec {
protected:
	CELTMode *(*celt_mode_create)(celt_int32 Fs, int frame_size, int *error);
	CELTEncoder *(__cdecl *celt_encoder_create)(const CELTMode *mode, int channels, int *error);
	CELTDecoder *(__cdecl *celt_decoder_create)(const CELTMode *mode, int channels, int *error);
	int(__cdecl *celt_encode_float)(CELTEncoder *st, const float *pcm, float *optional_synthesis,
									unsigned char *compressed, int nbCompressedBytes);
	int(__cdecl *celt_encode)(CELTEncoder *st, const celt_int16 *pcm, celt_int16 *optional_synthesis,
							  unsigned char *compressed, int nbCompressedBytes);
	int(__cdecl *celt_decode_float)(CELTDecoder *st, const unsigned char *data, int len, float *pcm);
	int(__cdecl *celt_decode)(CELTDecoder *st, const unsigned char *data, int len, celt_int16 *pcm);
	const char *(__cdecl *celt_strerror)(int error);

public:
	CELTCodec070(const QString &celt_version);
	CELTEncoder *encoderCreate() Q_DECL_OVERRIDE;
	CELTDecoder *decoderCreate() Q_DECL_OVERRIDE;
	int encode(CELTEncoder *st, const celt_int16 *pcm, unsigned char *compressed,
			   int nbCompressedBytes) Q_DECL_OVERRIDE;
	int decode_float(CELTDecoder *st, const unsigned char *data, int len, float *pcm) Q_DECL_OVERRIDE;
};

class CELTCodecSBCELT : public CELTCodec {
protected:
	const CELTMode *cmSBCELTMode;

public:
	CELTCodecSBCELT();
	CELTEncoder *encoderCreate() Q_DECL_OVERRIDE;
	CELTDecoder *decoderCreate() Q_DECL_OVERRIDE;
	int encode(CELTEncoder *st, const celt_int16 *pcm, unsigned char *compressed,
			   int nbCompressedBytes) Q_DECL_OVERRIDE;
	int decode_float(CELTDecoder *st, const unsigned char *data, int len, float *pcm) Q_DECL_OVERRIDE;
};

#endif // CELTCODEC_H_
