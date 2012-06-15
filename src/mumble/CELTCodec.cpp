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

#ifdef Q_CC_GNU
#define RESOLVE(var) { var = reinterpret_cast<__typeof__(var)>(qlCELT.resolve(#var)); bValid = bValid && (var != NULL); }
#else
#define RESOLVE(var) { * reinterpret_cast<void **>(&var) = static_cast<void *>(qlCELT.resolve(#var)); bValid = bValid && (var != NULL); }
#endif

#ifdef Q_OS_WIN
extern "C" {
	void __cpuid(int a[4], int b);
};
#endif

CELTCodec::CELTCodec(const QString &version) {
	bValid = false;
	cmMode = NULL;
	qsVersion = version;
	iBitstreamVersion = INT_MIN;
	qlCELT.setLoadHints(QLibrary::ResolveAllSymbolsHint);

	QStringList alternatives;
#if defined(Q_OS_MAC)
	alternatives << QString::fromLatin1("libcelt0.%1.dylib").arg(version);
	alternatives << QString::fromLatin1("celt0.%1.dylib").arg(version);
	alternatives << QString::fromLatin1("libcelt.%1.dylib").arg(version);
	alternatives << QString::fromLatin1("celt.%1.dylib").arg(version);
#elif defined(Q_OS_UNIX)
	alternatives << QString::fromLatin1("libcelt0.so.%1").arg(version);
	alternatives << QString::fromLatin1("libcelt.so.%1").arg(version);
	alternatives << QString::fromLatin1("celt.so.%1").arg(version);
#else
	int cpuinfo[4];
	__cpuid(cpuinfo, 1);
	if (cpuinfo[3] & 0x02000000) {
		if (cpuinfo[3] & 0x04000000) {
			if (cpuinfo[2] & 0x00000001) {
				alternatives << QString::fromLatin1("celt0.%1.sse3.dll").arg(version);
			}
			alternatives << QString::fromLatin1("celt0.%1.sse2.dll").arg(version);
		}
		alternatives << QString::fromLatin1("celt0.%1.sse.dll").arg(version);
	}

	alternatives << QString::fromLatin1("celt0.%1.dll").arg(version);
#endif
	foreach(const QString &lib, alternatives) {
		qlCELT.setFileName(QApplication::instance()->applicationDirPath() + QLatin1String("/") + lib);
		if (qlCELT.load()) {
			bValid = true;
			break;
		}

#ifdef Q_OS_MAC
		qlCELT.setFileName(QApplication::instance()->applicationDirPath() + QLatin1String("/../Codecs/") + lib);
		if (qlCELT.load()) {
			bValid = true;
			break;
		}
#endif

#ifdef PLUGIN_PATH
		qlCELT.setFileName(QLatin1String(MUMTEXT(PLUGIN_PATH) "/") + lib);
		if (qlCELT.load()) {
			bValid = true;
			break;
		}
#endif

		qlCELT.setFileName(lib);
		if (qlCELT.load()) {
			bValid = true;
			break;
		}
	}

	RESOLVE(celt_mode_destroy);
	RESOLVE(celt_mode_info);

	RESOLVE(celt_encoder_destroy);
	RESOLVE(celt_encoder_ctl);

	RESOLVE(celt_decoder_destroy);
	RESOLVE(celt_decoder_ctl);
}

CELTCodec::~CELTCodec() {
	if (cmMode)
		celt_mode_destroy(const_cast<CELTMode *>(cmMode));
}

bool CELTCodec::isValid() const {
	return bValid;
}

int CELTCodec::bitstreamVersion() const {
	if (cmMode && iBitstreamVersion == INT_MIN)
		celt_mode_info(cmMode, CELT_GET_BITSTREAM_VERSION, reinterpret_cast<celt_int32 *>(&iBitstreamVersion));

	return iBitstreamVersion;
}

QString CELTCodec::version() const {
	return qsVersion;
}

void CELTCodec::report() const {
	qWarning("CELT bitstream %08x from %s", bitstreamVersion(), qPrintable(qlCELT.fileName()));
}

CELTCodec070::CELTCodec070(const QString &version) : CELTCodec(version) {
	RESOLVE(celt_mode_create);
	RESOLVE(celt_encoder_create);
	RESOLVE(celt_decoder_create);
	RESOLVE(celt_encode_float);
	RESOLVE(celt_encode);
	RESOLVE(celt_decode_float);
	RESOLVE(celt_decode);
	RESOLVE(celt_strerror);

	if (bValid) {
		cmMode = celt_mode_create(SAMPLE_RATE, SAMPLE_RATE / 100, NULL);
	}
}

CELTEncoder *CELTCodec070::encoderCreate() {
	return celt_encoder_create(cmMode, 1, NULL);
}

CELTDecoder *CELTCodec070::decoderCreate() {
	return celt_decoder_create(cmMode, 1, NULL);
}

int CELTCodec070::encode(CELTEncoder *st, const celt_int16 *pcm, unsigned char *compressed, int nbCompressedBytes) {
	return celt_encode(st, pcm, NULL, compressed, nbCompressedBytes);
}

int CELTCodec070::decode_float(CELTDecoder *st, const unsigned char *data, int len, float *pcm) {
	return celt_decode_float(st, data, len, pcm);
}

CELTCodec011::CELTCodec011(const QString &version) : CELTCodec(version) {
	RESOLVE(celt_mode_create);
	RESOLVE(celt_encoder_create_custom);
	RESOLVE(celt_decoder_create_custom);
	RESOLVE(celt_encode_float);
	RESOLVE(celt_encode);
	RESOLVE(celt_decode_float);
	RESOLVE(celt_decode);
	RESOLVE(celt_strerror);

	if (bValid) {
		cmMode = celt_mode_create(SAMPLE_RATE, SAMPLE_RATE / 100, NULL);
	}
}

CELTEncoder *CELTCodec011::encoderCreate() {
	return celt_encoder_create_custom(cmMode, 1, NULL);
}

CELTDecoder *CELTCodec011::decoderCreate() {
	return celt_decoder_create_custom(cmMode, 1, NULL);
}

int CELTCodec011::encode(CELTEncoder *st, const celt_int16 *pcm, unsigned char *compressed, int nbCompressedBytes) {
	return celt_encode(st, pcm, SAMPLE_RATE / 100, compressed, nbCompressedBytes);
}

int CELTCodec011::decode_float(CELTDecoder *st, const unsigned char *data, int len, float *pcm) {
	return celt_decode_float(st, data, len, pcm, SAMPLE_RATE / 100);
}
