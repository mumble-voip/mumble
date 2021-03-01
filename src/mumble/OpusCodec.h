// Copyright 2018-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OPUSCODEC_H_
#define MUMBLE_MUMBLE_OPUSCODEC_H_

#include <opus.h>

#include <QtCore/QLibrary>

#ifndef Q_OS_WIN
#	define __cdecl
#endif

/// Loads Opus from a shared library and acts as a wrapper for its functions.
class OpusCodec {
private:
	Q_DISABLE_COPY(OpusCodec)
protected:
	QLibrary qlOpus;
	bool bValid;

public:
	OpusCodec();
	virtual ~OpusCodec();

	bool isValid() const;
	void report() const;

	const char *(__cdecl *opus_get_version_string)();

	OpusEncoder *(__cdecl *opus_encoder_create)(opus_int32 Fs, int channels, int application, int *error);
	int(__cdecl *opus_encoder_ctl)(OpusEncoder *st, int request, ...);
	void(__cdecl *opus_encoder_destroy)(OpusEncoder *st);
	OpusDecoder *(__cdecl *opus_decoder_create)(opus_int32 Fs, int channels, int *error);
	int(__cdecl *opus_decoder_ctl)(OpusDecoder *st, int request, ...);
	void(__cdecl *opus_decoder_destroy)(OpusDecoder *st);

	int(__cdecl *opus_encode)(OpusEncoder *st, const opus_int16 *pcm, int frame_size, unsigned char *compressed,
							  int nbCompressedBytes);
	int(__cdecl *opus_decode_float)(OpusDecoder *st, const unsigned char *data, opus_int32 len, float *pcm,
									int frame_size, int decode_fec);

	int(__cdecl *opus_decoder_get_nb_samples)(OpusDecoder *st, const unsigned char packet[], opus_int32 len);

	int(__cdecl *opus_packet_get_samples_per_frame)(const unsigned char *data, opus_int32 Fs);
};

#endif // OPUSCODEC_H_
