// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ECHOCANCELLATIONOPTION_H
#define MUMBLE_MUMBLE_ECHOCANCELLATIONOPTION_H

#include <QtCore/QObject>


/// This enum lists a series of echo cancellation options
/// Each audio backend will determine which option is indeed available to that backend and
/// to the specific device combination
enum class EchoCancelOptionID {
    DISABLED = 0,
    SPEEX_MIXED = 1,
    SPEEX_MULTICHANNEL = 2,
    APPLE_AEC = 3  // Apple's Acoustic Echo Cancellation support for macOS and iOS.
};

struct EchoCancelOption {
	EchoCancelOptionID id;
	QString description;
	QString explanation;
};

// Please strictly follow the order of the EchoCancelOptionID when adding items to this array.
static const EchoCancelOption echoCancelOptions[] = {
	{ EchoCancelOptionID::DISABLED,
	  QObject::tr("Disabled"),
	  QObject::tr("Echo cancellation is disabled.") },
	{ EchoCancelOptionID::SPEEX_MIXED,
	  QObject::tr("Mixed echo cancellation (speex)"),
	  QObject::tr("Mixed has low CPU impact, but only works well if your "
						 "speakers are equally loud and equidistant from the microphone.") },
	{ EchoCancelOptionID::SPEEX_MULTICHANNEL,
	  QObject::tr("Multichannel echo cancellation (speex)"),
	  QObject::tr("Multichannel echo cancellation provides much better echo "
								 "cancellation, but at a higher CPU cost. "
								 "Multichannel echo cancellation requires more CPU, so "
								 "you should try mixed first.") },
	// Available only on Apple devices
	{ EchoCancelOptionID::APPLE_AEC,
	  QObject::tr("EXPERIMENTAL: Acoustic echo cancellation (Apple)."),
	  QObject::tr("The support for this option is experimental only! This option works best when using built-in microphone and speaker.") }
};

#endif // MUMBLE_ECHOCANCELLATIONOPTION_H
