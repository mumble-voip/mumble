// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CELTCodec.h"

#include "Audio.h"
#include "Version.h"

CELTCodec::CELTCodec(const QString &version) {
	bValid = true;
	cmMode = NULL;
	qsVersion = version;
	iBitstreamVersion = INT_MIN;

	this->celt_encoder_destroy = ::celt_encoder_destroy;
	this->celt_encoder_ctl = ::celt_encoder_ctl;

	this->celt_decoder_destroy = ::celt_decoder_destroy;
	this->celt_decoder_ctl = ::celt_decoder_ctl;
}

CELTCodec::~CELTCodec() {
	if (cmMode)
		::celt_mode_destroy(const_cast<CELTMode *>(cmMode));
}

bool CELTCodec::isValid() const {
	return bValid;
}

int CELTCodec::bitstreamVersion() const {
	if (cmMode && iBitstreamVersion == INT_MIN)
		::celt_mode_info(cmMode, CELT_GET_BITSTREAM_VERSION, reinterpret_cast<celt_int32 *>(&iBitstreamVersion));

	return iBitstreamVersion;
}

QString CELTCodec::version() const {
	return qsVersion;
}

void CELTCodec::report() const {
	qWarning("CELT bitstream %08x from internal CELT with SBCELT decoding", bitstreamVersion());
}

CELTCodecSBCELT::CELTCodecSBCELT() : CELTCodec(QLatin1String("0.7.0")) {
	if (bValid) {
		cmMode = ::celt_mode_create(SAMPLE_RATE, SAMPLE_RATE / 100, NULL);
		cmSBCELTMode = ::sbcelt_mode_create(SAMPLE_RATE, SAMPLE_RATE / 100, NULL);

		this->celt_decoder_destroy = ::sbcelt_decoder_destroy;
		this->celt_decoder_ctl = ::sbcelt_decoder_ctl;
	}
}

CELTEncoder *CELTCodecSBCELT::encoderCreate() {
	return ::celt_encoder_create(cmMode, 1, NULL);
}

CELTDecoder *CELTCodecSBCELT::decoderCreate() {
	return ::sbcelt_decoder_create(cmSBCELTMode, 1, NULL);
}

int CELTCodecSBCELT::encode(CELTEncoder *st, const celt_int16 *pcm, unsigned char *compressed, int nbCompressedBytes) {
	return ::celt_encode(st, pcm, NULL, compressed, nbCompressedBytes);
}

int CELTCodecSBCELT::decode_float(CELTDecoder *st, const unsigned char *data, int len, float *pcm) {
	return ::sbcelt_decode_float(st, data, len, pcm);
}
