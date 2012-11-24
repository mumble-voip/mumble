/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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