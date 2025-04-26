// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_MARKDOWN_H_
#define MUMBLE_MUMBLE_MARKDOWN_H_

#include <QString>

namespace Markdown {
/// Converts the given piece of text, interprets it as markdown and replaces
/// the markdown constructs with the respective HTML ones.
///
/// @param markdownInput A reference to the input string
/// @returns The processed HTML string
QString markdownToHTML(const QString &markdownInput);
} // namespace Markdown

#endif // MUMBLE_MUMBLE_MARKDOWN_H_
