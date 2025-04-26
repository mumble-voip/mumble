// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Markdown.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTextDocument>

namespace Markdown {
// Placeholder constant
const QLatin1String regularLineBreakPlaceholder("%<\\!!linebreak!!//>@");

/// Just a wrapper for QRegularExpression::match().
static QRegularExpressionMatch regexMatch(const QRegularExpression &regex, const QString &subject, qsizetype &offset) {
	return regex.match(subject, offset, QRegularExpression::NormalMatch, QRegularExpression::AnchorAtOffsetMatchOption);
}

/// Tries to match and replace an escaped character at exactly the given offset in the string
///
/// @param str A reference to the String to work on
/// @param offset The offset at which the matching shall be done. This will be modified to point right after
/// 	replacement text, if such a replacement has been made.
/// @returns Whether a replacement has been made
bool processEscapedChar(QString &str, qsizetype &offset) {
	static const QRegularExpression s_regex(QLatin1String("\\\\(.)"));

	const QRegularExpressionMatch match = regexMatch(s_regex, str, offset);

	if (match.hasMatch()) {
		QString replacement = QString::fromLatin1("%1").arg(match.captured(1)).toHtmlEscaped();

		str.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(), replacement);

		offset += replacement.size();

		return true;
	}

	return false;
}

/// Tries to match and replace a markdown section header at exactly the given offset in the string
///
/// @param str A reference to the String to work on
/// @param offset The offset at which the matching shall be done. This will be modified to point right after
/// 	replacement text, if such a replacement has been made.
/// @returns Whether a replacement has been made
bool processMarkdownHeader(QString &str, qsizetype &offset) {
	// Match a markdown section heading. Also eat up a potential following newline in order to
	// not create a huge spacing after the heading
	static const QRegularExpression s_regex(QLatin1String("^(#+) (.*)"), QRegularExpression::MultilineOption);

	const QRegularExpressionMatch match = regexMatch(s_regex, str, offset);
	if (match.hasMatch()) {
		const auto sectionLevel   = match.captured(1).size();
		const QString sectionName = match.captured(2).trimmed().toHtmlEscaped();

		const QString replacement = QString::fromLatin1("<h%1>%2</h%1>").arg(sectionLevel).arg(sectionName);

		str.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(), replacement);

		offset += replacement.size();

		return true;
	}

	return false;
}

/// Reverts the effect of QString::toHtmlEscaped. It is intended to be used for URLs that are part of the
/// actual href specification. In there we don't want to escape HTML characters as they are indeed part of
/// the underlaying link and thus we must not escape e.g. "&" by "&amp;" in there.
///
/// @param url The html-escaped URL
/// @return The un-escaped version of the given URL
QString unescapeURL(const QString &url) {
	QTextDocument doc;
	doc.setHtml(url);
	return doc.toPlainText();
}

/// Tries to match and replace a markdown link at exactly the given offset in the string
///
/// @param str A reference to the String to work on
/// @param offset The offset at which the matching shall be done. This will be modified to point right after
/// 	replacement text, if such a replacement has been made.
/// @returns Whether a replacement has been made
bool processMarkdownLink(QString &str, qsizetype &offset) {
	// Link in format [link text](url)
	static const QRegularExpression s_regex(QLatin1String("\\[([^\\]\\[]+)\\]\\(([^\\)]+)\\)"));

	const QRegularExpressionMatch match = regexMatch(s_regex, str, offset);
	if (match.hasMatch()) {
		QString url = match.captured(2);

		if (!url.startsWith(QLatin1String("http"), Qt::CaseInsensitive)) {
			// For a markdown link to work, it has to start with the protocol specification, e.g. http or https
			// As we can't know for sure that the given website supports https, we'll have to fall back to http
			// Most browsers will upgrade the request to https whenever possible anyways though, so this shouldn't be
			// too much of a problem.
			url = QLatin1String("http://") + url;
		}

		QString replacement =
			QString::fromLatin1("<a href=\"%1\">%2</a>").arg(unescapeURL(url), match.captured(1).toHtmlEscaped());
		str.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(), replacement);

		offset += replacement.size();

		return true;
	}

	return false;
}

/// Tries to match and replace a markdown bold-text at exactly the given offset in the string
///
/// @param str A reference to the String to work on
/// @param offset The offset at which the matching shall be done. This will be modified to point right after
/// 	replacement text, if such a replacement has been made.
/// @returns Whether a replacement has been made
bool processMarkdownBold(QString &str, qsizetype &offset) {
	// Bold text is marked as **bold**
	static const QRegularExpression s_regex(QLatin1String("\\*\\*([^*]+)\\*\\*"));

	const QRegularExpressionMatch match = regexMatch(s_regex, str, offset);
	if (match.hasMatch()) {
		QString replacement = QString::fromLatin1("<b>%1</b>").arg(match.captured(1).toHtmlEscaped());
		str.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(), replacement);

		offset += replacement.size();

		return true;
	}

	return false;
}

/// Tries to match and replace a markdown italic-text at exactly the given offset in the string
///
/// @param str A reference to the String to work on
/// @param offset The offset at which the matching shall be done. This will be modified to point right after
/// 	replacement text, if such a replacement has been made.
/// @returns Whether a replacement has been made
bool processMarkdownItalic(QString &str, qsizetype &offset) {
	// Italic text is marked as *italic*
	static const QRegularExpression s_regex(QLatin1String("\\*([^*]+)\\*"));

	const QRegularExpressionMatch match = regexMatch(s_regex, str, offset);
	if (match.hasMatch()) {
		QString replacement = QString::fromLatin1("<i>%1</i>").arg(match.captured(1).toHtmlEscaped());
		str.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(), replacement);

		offset += replacement.size();

		return true;
	}

	return false;
}

/// Tries to match and replace a markdown strikethrough-text at exactly the given offset in the string
///
/// @param str A reference to the String to work on
/// @param offset The offset at which the matching shall be done. This will be modified to point right after
/// 	replacement text, if such a replacement has been made.
/// @returns Whether a replacement has been made
bool processMarkdownStrikethrough(QString &str, qsizetype &offset) {
	// Strikethrough text is marked as ~~text~~
	static const QRegularExpression s_regex(QLatin1String("~~([^~]+)~~"));

	const QRegularExpressionMatch match = regexMatch(s_regex, str, offset);
	if (match.hasMatch()) {
		QString replacement = QString::fromLatin1("<s>%1</s>").arg(match.captured(1).toHtmlEscaped());
		str.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(), replacement);

		offset += replacement.size();

		return true;
	}

	return false;
}

/// Tries to match and replace a markdown quote (blockquote) at exactly the given offset in the string
///
/// @param str A reference to the String to work on
/// @param offset The offset at which the matching shall be done. This will be modified to point right after
/// 	replacement text, if such a replacement has been made.
/// @returns Whether a replacement has been made
bool processMarkdownBlockQuote(QString &str, qsizetype &offset) {
	// Block quotes are (consecutive) lines starting with "> "
	static const QRegularExpression s_regex(QLatin1String("^(>|&gt;) (.|\\n(>|&gt;) )+"),
											QRegularExpression::MultilineOption);

	const QRegularExpressionMatch match = regexMatch(s_regex, str, offset);
	if (match.hasMatch()) {
		QString quote = match.captured(0).replace(QLatin1String("&gt;"), QLatin1String(">"));

		QStringList lines = quote.split(QChar::fromLatin1('\n'));

		quote.clear();
		for (int i = 0; i < lines.size(); i++) {
			// remove the leading "> "
			quote += lines[i].right(lines[i].size() - 2);

			if (i != lines.size() - 1) {
				// Add linebreak back in
				quote += QString::fromLatin1("\n");
			}
		}

		QString replacement = QString::fromLatin1("<div><i>%1</i></div>")
								  .arg(quote.toHtmlEscaped().replace(QLatin1String("\n"), QLatin1String("<br/>")));
		str.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(), replacement);

		offset += replacement.size();

		return true;
	}

	return false;
}

/// Tries to match and replace a markdown inline code snippet at exactly the given offset in the string
///
/// @param str A reference to the String to work on
/// @param offset The offset at which the matching shall be done. This will be modified to point right after
/// 	replacement text, if such a replacement has been made.
/// @returns Whether a replacement has been made
bool processMarkdownInlineCode(QString &str, qsizetype &offset) {
	// Inline code fragments are marked as `code`
	static const QRegularExpression s_regex(QLatin1String("`([^`\n]+)`"));

	const QRegularExpressionMatch match = regexMatch(s_regex, str, offset);
	if (match.hasMatch()) {
		QString replacement = QString::fromLatin1("<code>%1</code>").arg(match.captured(1).toHtmlEscaped());
		str.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(), replacement);

		offset += replacement.size();

		return true;
	}

	return false;
}

/// Tries to match and replace a markdown code block at exactly the given offset in the string
///
/// @param str A reference to the String to work on
/// @param offset The offset at which the matching shall be done. This will be modified to point right after
/// 	replacement text, if such a replacement has been made.
/// @returns Whether a replacement has been made
bool processMarkdownCodeBlock(QString &str, qsizetype &offset) {
	// Code blocks are marked as ```code```
	// Also consume a potential following newline as the <pre> tag will cause a linebreak anyways
	static const QRegularExpression s_regex(QLatin1String("```.*\\n((?:[^`]|``?[^`]?)*)```(\\r\\n|\\n|\\r)?"));

	const QRegularExpressionMatch match = regexMatch(s_regex, str, offset);
	if (match.hasMatch()) {
		QString code = match.captured(1).toHtmlEscaped();

		// Trim away leading linebreaks
		while (code.size() >= 1 && (code[0] == QLatin1Char('\n') || code[0] == QLatin1Char('\r'))) {
			code = code.right(code.size() - 1);
		}
		// Trim end of string
		while (code.size() >= 1 && code[code.size() - 1].isSpace()) {
			code = code.left(code.size() - 1);
		}

		if (code.isEmpty()) {
			return false;
		}

		// Replace linebreaks with a special placeholder as the linebreaks in a <pre> block must not be replaced
		// with <br/>
		QString replacement =
			QString::fromLatin1("<pre>%1</pre>").arg(code.replace(QLatin1String("\n"), regularLineBreakPlaceholder));
		str.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(), replacement);

		offset += replacement.size();

		return true;
	}

	return false;
}

/// Tries to match and replace a plain link at exactly the given offset in the string
///
/// @param str A reference to the String to work on
/// @param offset The offset at which the matching shall be done. This will be modified to point right after
/// 	replacement text, if such a replacement has been made.
/// @returns Whether a replacement has been made
bool processPlainLink(QString &str, qsizetype &offset) {
	// We support links with prefixed protocol (e.g. https://bla.com) and prefixed with www (e.g. www.bla.com)
	// The last part of the regex matches percent encoded characters in the url
	// See also https://stackoverflow.com/a/1547940/3907364
	static const QRegularExpression s_regex(
		QLatin1String("([a-zA-Z]+://|[wW][wW][wW]\\.)([A-Za-z0-9-._~:/?#\\[\\]@!$&'()*+,;=]|%[a-fA-F0-9]{2})+"));

	const QRegularExpressionMatch match = regexMatch(s_regex, str, offset);
	if (match.hasMatch()) {
		QString url = match.captured(0);

		if (url.startsWith(QLatin1String("www"), Qt::CaseInsensitive)) {
			// Link is missing a protocol specification - use https as the default
			url = QStringLiteral("https://") + url;
		}

		QString replacement = QString::fromLatin1("<a href=\"%1\">%2</a>").arg(unescapeURL(url), url.toHtmlEscaped());
		str.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(), replacement);

		offset += replacement.size();

		return true;
	}

	return false;
}

void escapeCharacter(QString &str, qsizetype &offset) {
	QString tmp(str[offset]);

	tmp = tmp.toHtmlEscaped();

	if (tmp.size() == 1 && tmp[0] == str[offset]) {
		// Nothing to escape
		return;
	}

	// Perform the replacement
	QString first;
	QString second;

	if (offset > 0) {
		first = str.left(offset);
	}
	if (offset < str.size() - 1) {
		second = str.right(str.size() - offset - 1);
	}

	str = first + tmp + second;

	offset += tmp.size() - 1;
}

QString markdownToHTML(const QString &markdownInput) {
	QString htmlString = markdownInput;
	qsizetype offset   = 0;

	while (offset < htmlString.size()) {
		// The trick here is to know that in a condition the or-branches are only
		// processed until the first expression returns true. At this point no
		// lower or-branch will be executed. This results in each of these functions
		// being called in succession until the first returns true (meaning that it
		// was able to recognize and replace a pattern).
		// Each function will only try to match its pattern at the exact offset given.
		// If a function was able to match and replace, it'll update the offset by
		// itself in order for the processing to start over right after the replacement
		// text (avoiding replacing parts of the replacement text which will probably
		// render the initial replacement invalid).
		// If no function matches, we increase the offset manually.
		// Do this until the end of the text has been reached.
		if (!(processMarkdownHeader(htmlString, offset) || processMarkdownLink(htmlString, offset)
			  || processMarkdownBold(htmlString, offset) || processMarkdownItalic(htmlString, offset)
			  || processMarkdownStrikethrough(htmlString, offset) || processMarkdownBlockQuote(htmlString, offset)
			  || processMarkdownCodeBlock(htmlString, offset) || processMarkdownInlineCode(htmlString, offset)
			  || processPlainLink(htmlString, offset) || processEscapedChar(htmlString, offset))) {
			escapeCharacter(htmlString, offset);
			offset++;
		}
	}

	// Replace linebreaks afterwards in order to not mess up the RegEx used by the
	// different functions.
	static const QRegularExpression s_doubleLineBreakRegEx(QLatin1String("(\r\n|\n|\r)(\r\n|\n|\r)"));
	htmlString.replace(s_doubleLineBreakRegEx, QLatin1String("<br/>"));

	// Remove single newlines
	static const QRegularExpression s_singleLineBreak("\r\n|\n|\r");
	htmlString.replace(s_singleLineBreak, "");

	// Restore linebreaks in <pre> blocks
	htmlString.replace(regularLineBreakPlaceholder, QLatin1String("\n"));

	return htmlString;
}
} // namespace Markdown
