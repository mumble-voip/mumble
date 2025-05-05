// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_HTMLFILTER_H_
#define MUMBLE_HTMLFILTER_H_

#include <QString>

/// HTMLFilter provides utilitites for
/// creating a plain text representaton
/// of a HTML document.
///
/// It is used for converting Mumble
/// text messages, comments, and more
/// to plain text when a server is
/// configured to disallow HTML.
class HTMLFilter {
	/// escapeTags returns the in HTML document
	/// with all occurrences of > and <
	/// replaced by &lt; and &gt;.
	static QString escapeTags(const QString &in);

public:
	/// filter does a best-effort conversion of the
	/// in HTML document to a plain-text representation.
	///
	/// If the filtering process succeeded, the function
	/// writes the filtered document as plain text to
	/// out, and returns true.
	///
	/// If the filtering failed, the function returns false
	/// and out is left unchanged.
	static bool filter(const QString &in, QString &out);
};

#endif
