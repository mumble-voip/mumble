// Copyright 2018-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OpusCodec.h"

#include "Audio.h"
#include "MumbleApplication.h"
#include "Version.h"

#ifdef Q_CC_GNU
#	define RESOLVE(var)                                                        \
		{                                                                       \
			var    = reinterpret_cast< __typeof__(var) >(qlOpus.resolve(#var)); \
			bValid = bValid && var;                                             \
		}
#else
#	define RESOLVE(var)                                                                      \
		{                                                                                     \
			*reinterpret_cast< void ** >(&var) = static_cast< void * >(qlOpus.resolve(#var)); \
			bValid                             = bValid && var;                               \
		}
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


	QStringList basePaths;
	basePaths << MumbleApplication::instance()->applicationVersionRootPath() + "/";
#ifdef Q_OS_MAC
	basePaths << QApplication::instance()->applicationDirPath() + "/../Codecs/";
#endif
#ifdef MUMBLE_LIBRARY_PATH
	basePaths << MUMTEXT(MUMBLE_LIBRARY_PATH) "/";
#endif
	basePaths << ""; // General search without telling it a particular path to look for

	for (const QString &basePath : basePaths) {
		for (const QString &libName : alternatives) {
			qlOpus.setFileName(basePath + libName);
			if (qlOpus.load()) {
				bValid = true;
				break;
			}
		}

		if (bValid) {
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
	RESOLVE(opus_decoder_ctl);
	RESOLVE(opus_decoder_destroy);

	RESOLVE(opus_decoder_get_nb_samples);

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

#undef RESOLVE
