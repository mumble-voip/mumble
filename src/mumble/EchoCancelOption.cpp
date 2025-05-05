// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "EchoCancelOption.h"

#include <QObject>

const std::vector< EchoCancelOption > &EchoCancelOption::getOptions() {
	// Note that we have to create this list lazily (at the time it is needed for the first time) in order
	// to make sure that there actually is a translator installed that can translate these Strings. If we
	// create these objects too early (e.g. by making them a static variable in global namespace), the Strings
	// will not get translated.

	// The item order MUST follow the order in the EchoCancelOptionID enum
	static const std::vector< EchoCancelOption > echoCancelOptions = {
		{ EchoCancelOptionID::DISABLED, QObject::tr("Disabled"), QObject::tr("Echo cancellation is disabled.") },
		{ EchoCancelOptionID::SPEEX_MIXED, QObject::tr("Mixed echo cancellation (speex)"),
		  QObject::tr("Mixed has low CPU impact, but only works well if your "
					  "speakers are equally loud and equidistant from the microphone.") },
		{ EchoCancelOptionID::SPEEX_MULTICHANNEL, QObject::tr("Multichannel echo cancellation (speex)"),
		  QObject::tr("Multichannel echo cancellation provides much better echo "
					  "cancellation, but at a higher CPU cost. "
					  "Multichannel echo cancellation requires more CPU, so "
					  "you should try mixed first.") },
		// Available only on Apple devices
		{ EchoCancelOptionID::APPLE_AEC, QObject::tr("EXPERIMENTAL: Acoustic echo cancellation (Apple)."),
		  QObject::tr("The support for this option is experimental only! This option works best when using built-in "
					  "microphone and speaker.") }
	};

	return echoCancelOptions;
}
