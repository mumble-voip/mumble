// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OpusCodec.h"

#include "Audio.h"
#include "Version.h"
#include "MumbleApplication.h"

#ifdef Q_CC_GNU
#define RESOLVE(var) { var = reinterpret_cast<__typeof__(var)>(qlOpus.resolve(#var)); bValid = bValid && (var != NULL); }
#else
#define RESOLVE(var) { * reinterpret_cast<void **>(&var) = static_cast<void *>(qlOpus.resolve(#var)); bValid = bValid && (var != NULL); }
#endif

OpusCodec::OpusCodec() {
	bValid = false;
	qlOpus.setLoadHints(QLibrary::ResolveAllSymbolsHint);

	QStringList alternatives;
#if defined(Q_OS_MAC)
	alternatives << QString::fromLatin1("libopus0.dylib");
	alternatives << QString::fromLatin1("opus0.dylib");
	alternatives << QString::fromLatin1("libopus.dylib");
	alternatives << QString::fromLatin1("opus.dylib");
#elif defined(Q_OS_UNIX)
	alternatives << QString::fromLatin1("libopus.so.0");
	alternatives << QString::fromLatin1("libopus0.so");
	alternatives << QString::fromLatin1("libopus.so");
	alternatives << QString::fromLatin1("opus.so");
#else
	alternatives << QString::fromLatin1("opus0.dll");
	alternatives << QString::fromLatin1("opus.dll");
#endif
	foreach(const QString &lib, alternatives) {
		qlOpus.setFileName(MumbleApplication::instance()->applicationVersionRootPath() + QLatin1String("/") + lib);
		if (qlOpus.load()) {
			bValid = true;
			break;
		}

#ifdef Q_OS_MAC
		qlOpus.setFileName(QApplication::instance()->applicationDirPath() + QLatin1String("/../Codecs/") + lib);
		if (qlOpus.load()) {
			bValid = true;
			break;
		}
#endif

#ifdef PLUGIN_PATH
		qlOpus.setFileName(QLatin1String(MUMTEXT(PLUGIN_PATH) "/") + lib);
		if (qlOpus.load()) {
			bValid = true;
			break;
		}
#endif

		qlOpus.setFileName(lib);
		if (qlOpus.load()) {
			bValid = true;
			break;
		}
	}

	RESOLVE(opus_get_version_string);

	RESOLVE(opus_encode);
	RESOLVE(opus_decode_float);

	RESOLVE(opus_encoder_create);
	RESOLVE(opus_encoder_ctl);
	RESOLVE(opus_encoder_destroy);
	RESOLVE(opus_decoder_create);
	RESOLVE(opus_decoder_destroy);

	RESOLVE(opus_packet_get_nb_frames);
	RESOLVE(opus_packet_get_samples_per_frame);
}

OpusCodec::~OpusCodec() {
	qlOpus.unload();
}

bool OpusCodec::isValid() const {
	return bValid;
}

void OpusCodec::report() const {
	qDebug("%s from %s", opus_get_version_string(), qPrintable(qlOpus.fileName()));
}
