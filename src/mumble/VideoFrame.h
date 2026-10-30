// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VIDEOFRAME_H_
#define MUMBLE_MUMBLE_VIDEOFRAME_H_

#include <QtCore/QMetaType>
#include <QtCore/QtGlobal>

#include <memory>

/// A decoded video picture in YUV 4:2:0 with 8 bits per sample (I420), which is what video decoders put out.
///
/// The picture is shared, not copied, when the frame is copied. It is drawn as it is (see VideoWidget), as
/// converting it to RGB on the CPU would cost about as much time as decoding it.
struct VideoFrame {
	enum class ColorSpace { BT601, BT709 };

	int width  = 0;
	int height = 0;
	/// The Y, U and V planes. U and V have half the width and height of Y, rounded up.
	const uchar *planes[3] = {};
	/// Bytes per row of each plane, which may be more than its width
	int strides[3] = {};
	/// How the colours are encoded
	ColorSpace colorSpace = ColorSpace::BT601;
	/// Whether the samples use the full range of 0..255, as opposed to the limited range of 16..235 (16..240 for
	/// U and V)
	bool fullRange = false;
	/// Keeps the planes alive
	std::shared_ptr< const void > holder;

	bool isNull() const { return !holder; }
};
Q_DECLARE_METATYPE(VideoFrame)

#endif // MUMBLE_MUMBLE_VIDEOFRAME_H_
