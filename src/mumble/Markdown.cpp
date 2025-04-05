// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Markdown.h"

#include "CustomElements.h"
#include "Log.h"
#include "MainWindow.h"
#include "Global.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTextDocument>

namespace Markdown {
struct Image {
	QString url;
	QString altText;
	QString title;
	QString width;
	QString height;
};
struct Items {
	const QString &inputStr;
	qsizetype offset;
	QString htmlStr;
	QList< Markdown::Image > &images;
	QHash< QString, std::tuple< QString, QString > > &references;
};

// Placeholder constants
const QLatin1String imagePlaceholder("%<\\!!image!!//>@");
const QLatin1String referencePlaceholder("%<\\!!reference!!//>@");

// Sign constant for use in a function where this value cancels the replacement
const QString doNoReplacementSign("\\");

/// Just a wrapper for QRegularExpression::match() where it will only match
/// if the match starts at the specified offset
///
/// @param regex The regex to match with
/// @param items A reference to the items to work on
/// @returns The regex match
static QRegularExpressionMatch regexMatch(const QRegularExpression &regex, Markdown::Items &items) {
	return regex.match(items.inputStr, items.offset, QRegularExpression::NormalMatch,
					   QRegularExpression::AnchorAtOffsetMatchOption);
}

/// Tries to return the first non-empty capturing group within the given index range of capturing groups
///
/// @param match The regex match to traverse the groups of
/// @param groupStartIndex The index of the first group to check for a match.
///     By default this is that of the first explicit group.
/// @param groupEndIndex The index of the last group to check for a match.
///     By default this limit is disabled and the search continues until the last group has been searched.
/// @returns The first non-empty group
QString getFirstMatchedGroup(const QRegularExpressionMatch &match, int groupStartIndex = 1, int groupEndIndex = -1) {
	QString firstMatchedGroup = match.captured(groupStartIndex);
	for (int i = 0; i < match.lastCapturedIndex(); ++i) {
		int currentGroupIndex = groupStartIndex + i;
		if (!firstMatchedGroup.isEmpty() || currentGroupIndex == groupEndIndex) {
			break;
		}
		firstMatchedGroup = match.captured(currentGroupIndex + 1);
	}
	return firstMatchedGroup;
}

/// Tries to match and replace a text at exactly the given offset in the string included in the items
///
/// @param items The items to work on
/// @param regex The regex to match with
/// @param contentWrapper The text, typically HTML tags, to wrap the replacement content of the match with
/// @param actionOnMatch If provided, this is the action to take when handling a match, otherwise the default
///     action to set the replacement to wrap the first explicit non-empty capturing group is used.
/// @returns Whether a replacement has been made
bool regexMatchAndReplace(
	Markdown::Items &items, const QRegularExpression &regex, const char *contentWrapper,
	std::function< QString(const QRegularExpressionMatch &, const char *) > actionOnMatch = nullptr) {
	const QRegularExpressionMatch match = regexMatch(regex, items);
	if (!match.hasMatch()) {
		return false;
	}

	QString replacement = actionOnMatch != nullptr
							  ? actionOnMatch(match, contentWrapper)
							  : QLatin1String(contentWrapper).arg(getFirstMatchedGroup(match).toHtmlEscaped());
	bool isReplacement = replacement != doNoReplacementSign;
	if (isReplacement) {
		items.htmlStr.append(replacement);
		items.offset = match.capturedEnd();
	}
	return isReplacement;
}

/// Tries to escape a character at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool escapeCharacter(Markdown::Items &items) {
	qsizetype &offset        = items.offset;
	const QChar &charToEsc   = items.inputStr[offset];
	QString escapedChar      = QString(charToEsc).toHtmlEscaped();
	qsizetype escapedCharLth = escapedChar.size();
	if (escapedCharLth == 1 && escapedChar[0] == charToEsc) {
		// Nothing to escape
		return false;
	}

	items.htmlStr.append(escapedChar);
	offset += escapedCharLth;
	return true;
}

/// Reverts the effect of QString::toHtmlEscaped. It is intended to be used for URLs that are part of the
/// actual href specification. In there we don't want to escape HTML characters as they are indeed part of
/// the underlying link and thus we must not escape e.g. "&" by "&amp;" in there.
///
/// @param url The HTML-escaped URL
/// @return The un-escaped version of the given URL
QString unescapeURL(const QString &url) {
	QTextDocument doc;
	doc.setHtml(url);
	return doc.toPlainText();
}

/// Gets the given text with a modification to make it compatible as either a reference or a URL
/// depending on the second argument, where `true` is a reference and `false` is a URL.
/// A reference has a reference placeholder inserted at the start and a URL is replaced with its un-escaped version.
///
/// @param url The HTML-escaped URL
/// @param isReference Whether the given text is a reference
/// @return The text with the reference placeholder at the start or the un-escaped version of the URL
QString referenceOrUnescapeURL(QString &url, bool isReference) {
	return isReference ? url.prepend(referencePlaceholder) : unescapeURL(url);
}

/// Checks if the given brackets are of the same type and returns `true` if they are, otherwise `false`.
/// If nothing is given then there are no conflicting bracket types and so `true` is returned.
/// The supported types are square brackets and parentheses.
/// This is intended to be used for reference links and reference images, where the type of
/// the last pair of brackets denotes if it is a reference or not.
///
/// @param startBracket The opening bracket
/// @param endBracket The closing bracket
/// @return Whether the brackets are of the same type
bool isSameBracketType(const QString &startBracket, const QString &endBracket) {
	if (startBracket.isEmpty() && endBracket.isEmpty()) {
		return true;
	}

	bool squareBracketsInsteadOfParentheses = endBracket == "]";
	QString startBracketOfThisType(squareBracketsInsteadOfParentheses ? "[" : "(");
	QString endBracketOfThisType(squareBracketsInsteadOfParentheses ? "]" : ")");

	return startBracket == startBracketOfThisType && endBracket == endBracketOfThisType;
}

/// Tries to match and replace an escaped character at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processEscapedChar(Markdown::Items &items) {
	if (items.inputStr[items.offset] != '\\') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("\\\\(.)"));
	return regexMatchAndReplace(items, s_regex, "%1");
}

/// Tries to match and replace plain linebreaks at exactly the given offset in the string.
/// Double linebreaks are filled in with `br` elements and single linebreaks are removed.
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processPlainLinebreaks(Markdown::Items &items) {
	QChar matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '\n' && matchStartCharacter != '\r') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("\\R{2}"));
	return regexMatchAndReplace(items, s_regex, "<br/><br/>",
								[](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString content   = match.captured(0);
									bool isOneNewline = content.size() < (content.startsWith("\r\n") ? 3 : 2);
									return QLatin1String(!isOneNewline ? contentWrapper : " ");
								});
}

/// Tries to match and replace characters at every offset in the string with the given formats
///
/// @param items A reference to the items to work on. This includes an offset for the input text which,
//      in case a replacement is made, will be set to the index right after the replaced part.
/// @param formats The functions to try to replace a part of the input with
void processFormatting(Markdown::Items &items, QList< std::function< bool(Markdown::Items &) > > formats) {
	const QString &inputStr = items.inputStr;
	qsizetype &offset       = items.offset;
	while (offset < inputStr.size()) {
		bool isReplacementAtOffset = false;
		for (auto format : formats) {
			if (format(items)) {
				isReplacementAtOffset = true;
				break;
			}
		}
		if (!(isReplacementAtOffset || processPlainLinebreaks(items) || processEscapedChar(items)
			  || escapeCharacter(items))) {
			items.htmlStr.append(inputStr[offset++]);
		}
	}
}

/// Tries to match and replace characters at every offset in the string with the given formats
///
/// @param inputStr A reference to the input text to work on. This will be modified to contain the HTML output.
/// @param items A reference to the items to work on
/// @param formats The functions to try to replace with
void processFormatting(QString &inputStr, Markdown::Items &items,
					   QList< std::function< bool(Markdown::Items &) > > formats) {
	Markdown::Items substringItems = { inputStr, (qsizetype) 0, QLatin1String(), items.images, items.references };
	processFormatting(substringItems, formats);
	inputStr = substringItems.htmlStr;
}

/// Tries to match and replace an HTML comment at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processHTMLComment(Markdown::Items &items) {
	// Comment in format <!--text-->
	if (items.inputStr[items.offset] != '<') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("<!-{2,}((?:.|\n)*?)-{2,}>"));
	return regexMatchAndReplace(items, s_regex, "");
}

/// Tries to match and replace an HTML fixed space at exactly the given offset in the string
/// Note: Halfwidth Hangul Filler (`&#xFFA0;`) is used instead of the regular non-breaking space (`&nbsp;`) in order
/// to avoid breaking the detection of image messages which results in using the wrong message character limit.
/// It is somewhat longer but works the same way in preserving each space instead of collapsing
/// to one space between other characters or none otherwise. Using `&nbsp;` to add a fixed space is still accepted
/// but it is implemented with said alternative fixed space.
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processHTMLFixedSpace(Markdown::Items &items) {
	// Fixed space in format &nbsp; or &#xFFA0;
	if (items.inputStr[items.offset] != '&') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("&nbsp;|&#xFFA0;"));
	return regexMatchAndReplace(items, s_regex, "&#xFFA0;");
}

/// Tries to match and replace an HTML linebreak at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processHTMLBr(Markdown::Items &items) {
	// Linebreak in format <br> or <br/> or <br />
	if (items.inputStr[items.offset] != '<') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("<br\\h?/?>"));
	return regexMatchAndReplace(items, s_regex, "<br/>");
}

/// Tries to match and replace a Markdown linebreak at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownLinebreak(Markdown::Items &items) {
	// Linebreak in the following format
	// text\ or text[2+ spaces]
	// one or more non-whitespace characters somewhere on this line right after the previous one
	QChar matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '\\' && matchStartCharacter != ' ') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("(?:\\\\|\\h{2,})(?=\n.*\\S)"));
	return regexMatchAndReplace(items, s_regex, "<br/>");
}

/// Tries to match and replace a Markdown- or HTML linebreak at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processLinebreak(Markdown::Items &items) {
	return processMarkdownLinebreak(items) || processHTMLBr(items);
}

/// Tries to match and replace an HTML span-element with color at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processHTMLSpanWithColor(Markdown::Items &items) {
	// Color in format <span style="color: color-code;">text</span>
	if (items.inputStr[items.offset] != '<') {
		return false;
	}
	static const QRegularExpression s_regex(
		QLatin1String("<span style=(?:\"color:\\h?(.+?);\"|'color:\\h?(.+?);')>(.+?)</span>"));
	return regexMatchAndReplace(items, s_regex, "<span style=\"color: %1;\">%2</span>",
								[](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString color = getFirstMatchedGroup(match, 1, 2).toHtmlEscaped();
									QString text  = match.captured(3).toHtmlEscaped();
									return QLatin1String(contentWrapper).arg(color, text);
								});
}

/// Tries to match and replace an HTML font-element with color at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processHTMLFontWithColor(Markdown::Items &items) {
	// Color in format <font color="color-code">text</font>
	if (items.inputStr[items.offset] != '<') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("<font color=(?:\"(.+?)\"|'(.+?)')>(.+?)</font>"));
	return regexMatchAndReplace(items, s_regex, "<font color=\"%1\">%2</font>",
								[](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString color = getFirstMatchedGroup(match, 1, 2).toHtmlEscaped();
									QString text  = match.captured(3).toHtmlEscaped();
									return QLatin1String(contentWrapper).arg(color, text);
								});
}

/// Tries to match and replace a LaTeX colored text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processLaTeXColor(Markdown::Items &items) {
	// Color in format $\color{color-code}\text{text}$ or $\color{color-code}{\text{text}}$
	if (items.inputStr[items.offset] != '$') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("\\$\\\\color\\{(.+?)\\}(?:\\\\text\\{(.+?)\\}"
														  "|\\{\\\\text\\{(.+?)\\}\\})\\$"));
	return regexMatchAndReplace(items, s_regex, "<font color=\"%1\">%2</font>",
								[](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString color = match.captured(1).toHtmlEscaped();
									QString text  = getFirstMatchedGroup(match, 2).toHtmlEscaped();
									return QLatin1String(contentWrapper).arg(color, text);
								});
}

/// Tries to match and replace a LaTeX colored text or HTML span- or font with color
/// at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processColor(Markdown::Items &items) {
	return processLaTeXColor(items) || processHTMLSpanWithColor(items) || processHTMLFontWithColor(items);
}

/// Tries to match and replace a plain link at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processPlainLink(Markdown::Items &items) {
	// We support links with prefixed protocol (e.g. https://bla.com) and prefixed with www (e.g. www.bla.com)
	// The last part of the regex matches percent encoded characters in the URL
	// See also https://stackoverflow.com/a/1547940/3907364
	if (!items.inputStr[items.offset].isLetter()) {
		return false;
	}
	static const QRegularExpression s_regex(
		QLatin1String("([a-zA-Z]{1,10}://|[wW]{3}\\.)([A-Za-z0-9-._~:/?#\\[\\]@!$&'()*+,;=]|%[a-fA-F0-9]{2})+"));
	return regexMatchAndReplace(items, s_regex, "<a href=\"%1\">%2</a>",
								[](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString url = match.captured(0).toHtmlEscaped();

									if (url.startsWith(QLatin1String("www"), Qt::CaseInsensitive)) {
										// Link is missing a protocol specification - use https as the default
										url.prepend(QStringLiteral("https://"));
									}

									return QLatin1String(contentWrapper).arg(unescapeURL(url), url);
								});
}

bool processMarkdownEmphasisOrScript(Markdown::Items &items);

bool processMarkdownInlineCode(Markdown::Items &items);

/// Tries to match and replace a Markdown link at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownLink(Markdown::Items &items) {
	// Link in format [link text](URL "tooltip") where the URL may include spaces if wrapped in angular brackets (<>),
	// the double quotes may be single quotes or parentheses and the tooltip is optional
	// The parentheses containing the URL may be square brackets containing a reference instead,
	// where the first pair of square brackets may be omitted to make the link text the same as the reference name
	if (items.inputStr[items.offset] != '[') {
		return false;
	}
	static const QRegularExpression s_regex(
		QLatin1String("(\\[([^\\]\n]+)\\])?(\\(|\\[)(?:<(.+?)>|((?<=\\()[^\\)\\h\n]+|[^\\]\\h\n]+))"
					  "(?:\\h(?:\"([^\"\n]+)\"|'([^'\n]+)'|\\(([^\\)\n]+)\\)))?(\\)|\\])"));
	return regexMatchAndReplace(items, s_regex, "<a href=\"%1\"%3>%2</a>",
								[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString textWithBrackets = match.captured(1);
									QString text             = match.captured(2);
									QString urlStartBracket  = match.captured(3);
									QString url              = getFirstMatchedGroup(match, 4, 5).toHtmlEscaped();
									QString title            = getFirstMatchedGroup(match, 6, 8).toHtmlEscaped();
									QString urlEndBracket    = match.captured(9);
									if (!isSameBracketType(urlStartBracket, urlEndBracket)) {
										return doNoReplacementSign;
									}

									bool isReference = urlEndBracket == "]";
									if (!isReference && !url.startsWith(QLatin1String("http"), Qt::CaseInsensitive)) {
										// For a Markdown link to work, it has to start with the protocol specification,
										// e.g. http or https As we can't know for sure that the given website supports
										// https, we'll have to fall back to http Most browsers will upgrade the request
										// to https whenever possible anyways though, so this shouldn't be too much of a
										// problem.
										url.prepend(QLatin1String("http://"));
									} else if (isReference && textWithBrackets.isEmpty()) {
										text = url;
									}

									processFormatting(text, items,
													  { processMarkdownEmphasisOrScript, processMarkdownInlineCode,
														processColor, processHTMLFixedSpace, processHTMLComment });
									return QLatin1String(contentWrapper)
										.arg(referenceOrUnescapeURL(url, isReference), text,
											 QLatin1String(!title.isEmpty() ? " title=\"%1\"" : "").arg(title));
								});
}

/// Tries to match, replace and add a Markdown reference definition at exactly the given offset in the string.
/// Duplicates, that is definitions with the same name, are ignored in favor of the first one in the text.
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownReferenceDefinition(Markdown::Items &items) {
	// Reference definition in format [reference]: URL "tooltip" where the URL may include spaces if wrapped
	// in angular brackets (<>), the double quotes may be single quotes or parentheses and the tooltip is optional
	QChar matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '[' && matchStartCharacter != ' ') {
		return false;
	}
	static const QRegularExpression s_regex(
		QLatin1String("^\\h{,3}\\[([^\\]\n]+)\\]:\\h(?:<(.+?)>|([^\\h\n]+))"
					  "(?:\\h(?:\"([^\"\n]+)\"|'([^'\n]+)'|\\(([^\\)\n]+)\\)))?\\h*\n?\\h*$"),
		QRegularExpression::MultilineOption);
	return regexMatchAndReplace(items, s_regex, "",
								[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString reference = match.captured(1).toHtmlEscaped();
									QString url       = getFirstMatchedGroup(match, 2, 3).toHtmlEscaped();
									QString title     = getFirstMatchedGroup(match, 4, 6).toHtmlEscaped();

									if (!url.startsWith(QLatin1String("http"), Qt::CaseInsensitive)) {
										url.prepend(QLatin1String("http://"));
									}

									QHash< QString, std::tuple< QString, QString > > &references = items.references;
									if (!references.contains(reference)) {
										references[reference] = std::make_tuple(unescapeURL(url), title);
									}
									return QLatin1String(contentWrapper);
								});
}

/// Tries to find and insert queued Markdown references, replacing each placeholder and name
/// with the value of the definition by the same name
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
void processQueuedMarkdownReferences(Markdown::Items &items) {
	QString &htmlStr                               = items.htmlStr;
	static const QString regexReferencePlaceholder = QString(referencePlaceholder).replace("\\", "\\\\");
	static const QRegularExpression s_reference(
		QLatin1String("<a\\h.*?href=\"%1(.+?)\".*?>(.+?)</a>").arg(regexReferencePlaceholder));
	qsizetype offset = 0;
	do {
		QRegularExpressionMatch match = s_reference.match(htmlStr, offset);
		if (!match.hasMatch()) {
			break;
		}
		QString reference       = match.captured(1);
		QString text            = match.captured(2);
		auto [url, title]       = items.references.value(reference);
		bool isReferenceDefined = !url.isEmpty();

		QString titleAttribute(!title.isEmpty() ? QLatin1String(" title=\"%1\"").arg(title) : "");
		QString replacement = isReferenceDefined
								  ? QLatin1String("<a href=\"%1\"%3>%2</a>").arg(url, text, titleAttribute)
								  : reference != text ? QLatin1String("[%2][%1]").arg(reference, text)
													  : QLatin1String("[%1]").arg(reference);

		qsizetype matchStartIndex = match.capturedStart();
		htmlStr.replace(matchStartIndex, match.capturedLength(), replacement);
		offset = matchStartIndex + replacement.size();
	} while (true);
}

/// Tries to match, replace and queue a Markdown image with a placeholder at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been queued
bool processMarkdownImage(Markdown::Items &items) {
	// Image in format ![alt text](URL "tooltip"){width=pxAmount height=pxAmount}
	// where the URL may include spaces if wrapped in angular brackets (<>) or be a file uri
	// (local file path starting with "file:///"), the double quotes may be single quotes or parentheses and
	// the alt text is recommended for accessibility but not required
	// A broken image will only be shown if the alt text contains at least one character
	// The tooltip and size attributes are also optional, where in the latter spaces may be commas and
	// each value may have up to four digits
	// The parentheses containing the URL may be square brackets containing a reference instead
	// The image may be wrapt in a Markdown link
	QChar matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '!' && matchStartCharacter != '[') {
		return false;
	}
	static const QRegularExpression s_regex(
		QLatin1String("\\[?!\\[([^\\]\n]*)\\](\\(|\\[)(?:<(.+?)>|((?<=\\()[^\\)\\h\n]+|[^\\]\\h\n]+))"
					  "(?:\\h(?:\"([^\"\n]+)\"|'([^'\n]+)'|\\(([^\\)\n]+)\\)))?(\\)|\\])"
					  "(?:\\{\\h?(?:(?:width=(\\d{1,4})\\d*(?:\\h|,\\h?))?height=(\\d{1,4})\\d*"
					  "|(?:height=(\\d{1,4})\\d*(?:\\h|,\\h?))?width=(\\d{1,4})\\d*\\h?)?\\})?"
					  "(?:\\](\\(|\\[)(?:<(.+?)>|((?<=\\()[^\\)\\h\n]+|[^\\]\\h\n]+))"
					  "(?:\\h(?:\"([^\"\n]+)\"|'([^'\n]+)'|\\(([^\\)\n]+)\\)))?(\\)|\\]))?"));
	return regexMatchAndReplace(
		items, s_regex, "<a href=\"%1\"%3>%2</a>",
		[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			QString altText              = match.captured(1).toHtmlEscaped();
			QString imageUrlStartBracket = match.captured(2);
			QString url                  = getFirstMatchedGroup(match, 3, 4).toHtmlEscaped();
			QString title                = getFirstMatchedGroup(match, 5, 7).toHtmlEscaped();
			QString imageUrlEndBracket   = match.captured(8);

			QString width  = (match.captured(9).isEmpty() ? match.captured(12) : match.captured(9)).toHtmlEscaped();
			QString height = getFirstMatchedGroup(match, 10, 11).toHtmlEscaped();

			QString wrapperUrlStartBracket = match.captured(13);
			QString link                   = getFirstMatchedGroup(match, 14, 15).toHtmlEscaped();
			QString linkTitle              = getFirstMatchedGroup(match, 16, 18).toHtmlEscaped();
			QString wrapperUrlEndBracket   = match.captured(19);

			bool hasLink                 = !link.isEmpty();
			bool startsWithSquareBracket = match.captured(0).startsWith('[');
			if ((hasLink && !startsWithSquareBracket) || (!hasLink && startsWithSquareBracket)
				|| !isSameBracketType(imageUrlStartBracket, imageUrlEndBracket)
				|| !isSameBracketType(wrapperUrlStartBracket, wrapperUrlEndBracket)) {
				return doNoReplacementSign;
			}

			bool isReferenceImage       = imageUrlEndBracket == "]";
			bool isReferenceWrapperLink = wrapperUrlEndBracket == "]";
			if (!isReferenceImage && !url.startsWith(QLatin1String("http"), Qt::CaseInsensitive)
				&& !url.startsWith(QLatin1String("file:///"), Qt::CaseInsensitive)) {
				url.prepend(QLatin1String("http://"));
			}
			if (!isReferenceWrapperLink && !link.startsWith(QLatin1String("http"), Qt::CaseInsensitive)) {
				link.prepend(QLatin1String("http://"));
			}

			items.images.append({ referenceOrUnescapeURL(url, isReferenceImage), altText, title, width, height });
			return hasLink ? QLatin1String(contentWrapper)
								 .arg(referenceOrUnescapeURL(link, isReferenceWrapperLink), imagePlaceholder,
									  !linkTitle.isEmpty() ? QLatin1String(" title=\"%1\"").arg(linkTitle) : "")
						   : imagePlaceholder;
		});
}

/// Tries to request and insert queued Markdown images, replacing their placeholders in the string
/// by order of appearance from start to end. Downloads and loading of local files will be awaited in parallel.
///
/// @param str A reference to the String to work on
void processQueuedMarkdownImages(Markdown::Items &items) {
	QString &htmlStr                 = items.htmlStr;
	QList< Markdown::Image > &images = items.images;
	qsizetype imageAmount            = images.size();
	if (imageAmount == 0) {
		return;
	}

	QList< std::tuple< QString, QSize, QString > > base64Images(imageAmount);
	QEventLoop loop;
	QNetworkAccessManager networkManager;
	int imageMessageLength                = static_cast< int >(Global::get().uiImageLength);
	qsizetype remainingImageMessageLength = imageMessageLength - htmlStr.size() + imageAmount * imagePlaceholder.size();

	qsizetype repliesFinished   = 0;
	QString processingIndicator = "<center><i>%1 image %2/%3 for message...</i></center>";
	ChatbarTextEdit *chatbar    = Global::get().mw->qteChat;
	auto getProcessingFirstWord = [](QString url) {
		return QLatin1String(url.startsWith("file:///") ? "Loading" : "Downloading");
	};
	QObject::connect(
		&networkManager, &QNetworkAccessManager::finished, &loop,
		[&base64Images, &repliesFinished, &imageAmount, &processingIndicator, &remainingImageMessageLength, &loop,
		 &chatbar](QNetworkReply *reply) {
			if (reply->error() == QNetworkReply::NoError) {
				QByteArray imageBa = reply->readAll();
				QImage image;
				if (image.loadFromData(imageBa)) {
					int imageIndex = reply->property("index").toInt();

					qsizetype imageSize       = imageBa.size();
					qsizetype base64ImageSize = imageSize * 4 / 3;
					QString imageHeader       = imageSize > 2 ? qvariant_cast< QString >(imageBa.first(3)) : "";
					bool isAnimation          = imageHeader == "GIF";
					QString img               = isAnimation ? qvariant_cast< QString >(imageBa.toBase64())
											  : Log::imageToImg(image, (int) remainingImageMessageLength);

					base64Images[imageIndex] = std::make_tuple(img, image.size(), isAnimation ? imageHeader : "JPEG");
					remainingImageMessageLength -= isAnimation ? base64ImageSize : img.size();
				}
			}

			QString processingFirstWord = reply->property("processingFirstWord").toString();
			reply->close();
			reply->deleteLater();
			if (++repliesFinished == imageAmount) {
				loop.quit();
			} else {
				chatbar->setHtml(processingIndicator.arg(processingFirstWord).arg(repliesFinished).arg(imageAmount));
			}
		});
	for (int i = 0; i < imageAmount; ++i) {
		Markdown::Image &image = images[i];
		QString &url           = image.url;
		if (url.startsWith(referencePlaceholder)) {
			static const qsizetype referencePlaceholderSize = referencePlaceholder.size();
			url.remove(0, referencePlaceholderSize);
			std::tie(url, image.title) = items.references.value(url);
		}
		QNetworkReply *reply = networkManager.get(QNetworkRequest(url));
		reply->setProperty("index", i);
		reply->setProperty("processingFirstWord", getProcessingFirstWord(url));
	}

	chatbar->setEnabled(false);
	chatbar->setAcceptDrops(false);
	chatbar->setHtml(processingIndicator.arg(getProcessingFirstWord(images[0].url)).arg(1).arg(imageAmount));
	loop.exec();

	for (int i = 0; i < imageAmount; ++i) {
		auto [_, altText, title, width, height] = images[i];
		auto [base64ImageData, imgSize, imgFmt] = base64Images[i];

		bool isWidth  = !width.isEmpty();
		bool isHeight = !height.isEmpty();
		if (isWidth || isHeight) {
			// Use the max size if the set image resize is too large, which with a max message area size of
			// 2048x2048 somehow becomes around 1442x1442 (approximately 70% of the max message area size) for images:
			static const int allowedEvenLength = (int) round(2048 * 0.7041);
			static const int allowedSize       = allowedEvenLength * allowedEvenLength;
			static const int maxBaseWidth      = 600;
			static const int maxBaseHeight     = 400;
			if (imgSize.width() > maxBaseWidth || imgSize.height() > maxBaseHeight) {
				imgSize.scale(maxBaseWidth, maxBaseHeight, Qt::KeepAspectRatio);
			}
			int widthInt  = width.toInt();
			int heightInt = height.toInt();
			if (!isWidth) {
				double widthToHeightRatio = imgSize.width() / (double) imgSize.height();
				widthInt                  = (int) round(heightInt * widthToHeightRatio);
			} else if (!isHeight) {
				double heightToWidthRatio = imgSize.height() / (double) imgSize.width();
				heightInt                 = (int) round(widthInt * heightToWidthRatio);
			}

			int imageSize = widthInt * heightInt;
			if (imageSize > allowedSize) {
				bool isWidthGreaterThanHeight = widthInt > heightInt;
				int &lengthAxisOne            = isWidthGreaterThanHeight ? widthInt : heightInt;
				int &lengthAxisTwo            = isWidthGreaterThanHeight ? heightInt : widthInt;
				QString &lengthAxisOneStr     = isWidthGreaterThanHeight ? width : height;
				QString &lengthAxisTwoStr     = isWidthGreaterThanHeight ? height : width;

				while (--lengthAxisOne * lengthAxisTwo > allowedSize && lengthAxisOne > allowedEvenLength) {
				}
				lengthAxisOneStr = QString::number(lengthAxisOne);
				if (lengthAxisOne * lengthAxisTwo > allowedSize) {
					while (lengthAxisOne * --lengthAxisTwo > allowedSize && lengthAxisTwo > allowedEvenLength) {
					}
					lengthAxisTwoStr = QString::number(lengthAxisTwo);
				}
			}
			isWidth  = !width.isEmpty();
			isHeight = !height.isEmpty();
		}
		bool isTitle                    = !title.isEmpty();
		QString imgAdditionalAttributes = QLatin1String("%1%2%3").arg(
			QLatin1String(isTitle ? "title=\"%1\"" : "").arg(title),
			QLatin1String(isWidth ? "%2width=\"%1\"" : "").arg(width, QLatin1String(isWidth && isTitle ? " " : "")),
			QLatin1String(isHeight ? "%2height=\"%1\"" : "")
				.arg(height, QLatin1String(isHeight && (isTitle || isWidth) ? " " : "")));

		qsizetype imageOffset     = htmlStr.indexOf(imagePlaceholder);
		qsizetype placeholderSize = imagePlaceholder.size();
		QString img               = base64ImageData.startsWith("<img")
						  ? base64ImageData
						  : QLatin1String("<img src=\"data:image/%2;base64,%1\"/>").arg(base64ImageData, imgFmt);
		bool isImage = !base64ImageData.isEmpty();

		// Do not apply width and height attributes to the broken image icon too:
		if (isImage) {
			img.insert(img.size() - 2, imgAdditionalAttributes);
		}
		// Remove a-element wrapper when the image failed to load so that the broken image and alt text may be visible:
		if (!isImage && imageOffset > 0 && htmlStr[imageOffset - 1] == '>') {
			// Start with the constant size of the end tag `</a>`:
			int sizeIncrease = 4;
			do {
				++sizeIncrease;
			} while (imageOffset > 0 && htmlStr[--imageOffset] != '<');
			placeholderSize += sizeIncrease;
		}

		htmlStr.replace(imageOffset, placeholderSize, isImage ? img : !altText.isEmpty() ? img.append(altText) : "");
	}
	// Enable chat input again, where the status text is cleared elsewhere when the message is sent:
	chatbar->setEnabled(true);
	chatbar->setAcceptDrops(true);
	if (QApplication::focusWidget() == nullptr) {
		chatbar->setFocus(Qt::OtherFocusReason);
	}
}

/// Tries to match and replace a Markdown link or plain link at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processLink(Markdown::Items &items) {
	return processMarkdownLink(items) || processPlainLink(items);
}

/// Tries to match and replace a Markdown reference definition, image, link or plain link
/// at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processImageOrLink(Markdown::Items &items) {
	return processMarkdownReferenceDefinition(items) || processMarkdownImage(items) || processLink(items);
}

bool processMarkdownStrikethrough(Markdown::Items &items);

bool processMarkdownUnderline(Markdown::Items &items);

bool processMarkdownLine(Markdown::Items &items);

bool processScript(Markdown::Items &items);

/// Tries to match and replace a Markdown italic-text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownItalic(Markdown::Items &items) {
	// Italic text is marked as *italic*
	if (items.inputStr[items.offset] != '*') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("\\*(?!\\h+\\*)([^*]+)\\*"));
	return regexMatchAndReplace(items, s_regex, "<i>%1</i>",
								[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString content = match.captured(1);
									processFormatting(content, items,
													  { processMarkdownLine, processScript, processLink, processColor,
														processLinebreak, processHTMLFixedSpace, processHTMLComment });
									return QLatin1String(contentWrapper).arg(content);
								});
}

/// Tries to match and replace a Markdown bold-text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownBold(Markdown::Items &items) {
	// Bold text is marked as **bold**
	if (items.inputStr[items.offset] != '*') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("\\*{2}(?!\\h*\\*{2,})(.+?\\*?)\\*{2}"));
	return regexMatchAndReplace(
		items, s_regex, "<b>%1</b>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			QString content = match.captured(1);
			processFormatting(content, items,
							  { processMarkdownLine, processMarkdownItalic, processScript, processLink, processColor,
								processLinebreak, processHTMLFixedSpace, processHTMLComment });
			return QLatin1String(contentWrapper).arg(content);
		});
}

/// Tries to match and replace a Markdown bold italic, bold or italic text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownBoldOrItalic(Markdown::Items &items) {
	return processMarkdownBold(items) || processMarkdownItalic(items);
}

/// Tries to match and replace a Markdown strikethrough-text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownStrikethrough(Markdown::Items &items) {
	// Strikethrough text is marked as ~~text~~ with no more surrounding tildes
	if (items.inputStr[items.offset] != '~') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("(?<!~)~~(?!\\h+~|~)(.+?)(?<!~)~~(?!~)"));
	return regexMatchAndReplace(
		items, s_regex, "<s>%1</s>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			QString content = match.captured(1);
			processFormatting(content, items,
							  { processMarkdownUnderline, processMarkdownBoldOrItalic, processScript, processLink,
								processColor, processLinebreak, processHTMLFixedSpace, processHTMLComment });
			return QLatin1String(contentWrapper).arg(content);
		});
}

/// Tries to match and replace a Markdown underline-text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownUnderline(Markdown::Items &items) {
	// Underline text is marked as __text__
	if (items.inputStr[items.offset] != '_') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("__(?!\\h+_)(.+?)__"));
	return regexMatchAndReplace(
		items, s_regex, "<u>%1</u>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			QString content = match.captured(1);
			processFormatting(content, items,
							  { processMarkdownStrikethrough, processMarkdownBoldOrItalic, processScript, processLink,
								processColor, processLinebreak, processHTMLFixedSpace, processHTMLComment });
			return QLatin1String(contentWrapper).arg(content);
		});
}

/// Tries to match and replace a Markdown strikethrough or underline text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownLine(Markdown::Items &items) {
	return processMarkdownStrikethrough(items) || processMarkdownUnderline(items);
}

/// Tries to match and replace a Markdown strikethrough, underline, bold italic, bold or italic text
/// at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownEmphasis(Markdown::Items &items) {
	return processMarkdownLine(items) || processMarkdownBoldOrItalic(items);
}

/// Tries to match and replace a Markdown superscript-text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownSuperscript(Markdown::Items &items) {
	// Superscript text is marked as ^text^
	if (items.inputStr[items.offset] != '^') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("\\^(?!\\h+\\^)([^\\^]+)\\^"));
	return regexMatchAndReplace(items, s_regex, "<sup>%1</sup>",
								[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString content = match.captured(1);
									processFormatting(content, items,
													  { processMarkdownEmphasis, processLink, processColor,
														processHTMLFixedSpace, processHTMLComment });
									return QLatin1String(contentWrapper).arg(content);
								});
}

/// Tries to match and replace a Markdown subscript-text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownSubscript(Markdown::Items &items) {
	// Subscript text is marked as ~text~
	if (items.inputStr[items.offset] != '~') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("~(?!\\h+~)([^~]+)~"));
	return regexMatchAndReplace(items, s_regex, "<sub>%1</sub>",
								[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString content = match.captured(1);
									processFormatting(content, items,
													  { processMarkdownEmphasis, processLink, processColor,
														processHTMLFixedSpace, processHTMLComment });
									return QLatin1String(contentWrapper).arg(content);
								});
}

/// Tries to match and replace Markdown superscript or subscript text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processScript(Markdown::Items &items) {
	return processMarkdownSuperscript(items) || processMarkdownSubscript(items);
}

/// Tries to match and replace Markdown emphasis or script text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownEmphasisOrScript(Markdown::Items &items) {
	return processMarkdownEmphasis(items) || processScript(items);
}

/// Tries to match and replace a Markdown section header at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownHeader(Markdown::Items &items) {
	// Match a Markdown section heading in one of the following formats, where the "#" may repeat up to 6 times
	// to generate an h1-h6 element accordingly. If instead triple equals or triple hyphens on the following
	// line is used, or more of the same character in a row, then the former is h1 and the latter is h2.
	// If a number of "#" in a row are used at least one space after a header starting with "#" then these are consumed
	// Also eat up a potential following newline in order to not create a huge spacing after the heading
	// # header
	// or
	// header
	// ===
	// or
	// header
	// ---
	static const QRegularExpression s_regex(
		QLatin1String("^\\h{,3}(?:(#+)\\h(?:(.+)\\h+#+\\h*|(.+))|(.+)\n\\h{,3}(={3,}|-{3,})\\h*$)\n?"),
		QRegularExpression::MultilineOption);
	return regexMatchAndReplace(
		items, s_regex, "<h%1>%2</h%1>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			const auto sectionLevel =
				match.captured(1).isEmpty() ? (match.captured(5).contains('=') ? 1 : 2) : match.captured(1).size();
			QString sectionName = getFirstMatchedGroup(match, 2, 4).trimmed();

			processFormatting(sectionName, items,
							  { processMarkdownEmphasisOrScript, processMarkdownInlineCode, processImageOrLink,
								processColor, processLinebreak, processHTMLFixedSpace, processHTMLComment });
			return QString::fromLatin1(contentWrapper).arg(sectionLevel).arg(sectionName);
		});
}

/// Tries to match and replace a Markdown thematic-break at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownThematicBreak(Markdown::Items &items) {
	// Thematic break is marked as *** or --- or ___ and the character used may repeat more than 3 times,
	// as well as with spaces between if the first one is placed 3 spaces in at most
	QChar matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '*' && matchStartCharacter != '-' && matchStartCharacter != '_') {
		return false;
	}
	static const QRegularExpression s_regex(
		QLatin1String("^\\h{,3}(\\*(?:\\h*\\*\\h*){2,}|-(?:\\h*-\\h*){2,}|_(?:\\h*_\\h*){2,})(?:\n$|$)"),
		QRegularExpression::MultilineOption);
	return regexMatchAndReplace(items, s_regex, "<hr/>");
}

/// Tries to match and replace a Markdown inline-code-snippet at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownInlineCode(Markdown::Items &items) {
	// Inline code is marked as `code`{.lang} where the backticks may use additional pairs of backticks
	// to escape more backticks in the code and specifying the language is optional
	if (items.inputStr[items.offset] != '`') {
		return false;
	}
	static const QRegularExpression s_regex(
		QLatin1String("(`+)(?!\\h*\\1)(.+?)\\1(?!`)(?:\\{\\h?(?:\\.([a-z]+))?\\h?\\})?"),
		QRegularExpression::CaseInsensitiveOption);
	return regexMatchAndReplace(items, s_regex, "<code%2>%1</code>",
								[](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString content  = match.captured(2).toHtmlEscaped();
									QString language = match.captured(3);
									bool isLanguage  = !language.isEmpty();
									language = isLanguage ? language.toLower().prepend("lang-").toHtmlEscaped() : "";

									return QLatin1String(contentWrapper)
										.arg(QLatin1String("&#xFFA0;%1&#xFFA0;").arg(content),
											 isLanguage ? QLatin1String(" class=\"%1\"").arg(language) : "");
								});
}

/// Tries to match and replace a Markdown code-block at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownCodeBlock(Markdown::Items &items) {
	// Code blocks are marked as ```code``` or ~~~code~~~ or consecutive lines of [4+ spaces]code
	// Fenced code blocks may use additional signature characters to include more such characters in the text
	// without indenting it 4 spaces or having another non-whitespace character before it on the same line.
	QChar matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '`' && matchStartCharacter != '~' && matchStartCharacter != ' ') {
		return false;
	}
	static const QRegularExpression s_regex(
		QLatin1String("^\\h{,3}(`{3,}|~{3,})([a-z]*)\n((?:.|\n)+?)\n\\h{,3}\\1(?!`)|(?:(?:\n|^)\\h{4,}.*)++"),
		QRegularExpression::MultilineOption | QRegularExpression::CaseInsensitiveOption);
	return regexMatchAndReplace(
		items, s_regex, "<table class=\"codeblock%2\" width=\"100%\"><tr><td><pre>%1</pre></td></tr></table>",
		[](const QRegularExpressionMatch &match, const char *contentWrapper) {
			auto trim = [](QString text, qsizetype indentToCut = 0) -> QString {
				// Trim away leading linebreaks:
				while (text.size() > 0 && (text[0] == QLatin1Char('\n') || text[0] == QLatin1Char('\r'))) {
					text.remove(0, 1);
				}
				// Trim end of string:
				while (text.size() > 0 && text[text.size() - 1].isSpace()) {
					text.chop(1);
				}
				// Remove initial spaces on each line as specified:
				if (indentToCut > 0) {
					QStringList lines = text.split('\n');
					for (QString &line : lines) {
						line.remove(0, std::min(indentToCut, line.size()));
					}
					text = lines.join('\n');
				}
				return text;
			};

			bool isLanguage   = !match.captured(2).isEmpty();
			bool isCodeFenced = !match.captured(3).isEmpty();
			QString language  = isLanguage ? trim(match.captured(2).toLower().prepend("lang-")).toHtmlEscaped() : "";
			QString code      = (isCodeFenced ? trim(match.captured(3)) : trim(match.captured(0), 4)).toHtmlEscaped();
			return QLatin1String(contentWrapper).arg(code, isLanguage ? QLatin1String(" %1").arg(language) : "");
		});
}

/// Tries to match and replace a Markdown inline code snippet or code block at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processCode(Markdown::Items &items) {
	return processMarkdownInlineCode(items) || processMarkdownCodeBlock(items);
}

bool processMarkdownBlockQuote(Markdown::Items &items);

/// Tries to match and replace a Markdown list at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownList(Markdown::Items &items) {
	// List in one of the following formats, where ordered lists may start from any non-negative number up to 9 digits
	// and any combination of unordered lists and ordered lists may be nested with 2 additional spaces
	// past the start of the previous list item marker for each sublist on top of it. Any further spaces will be reduced
	// to that of the nearest next sublist.
	// The period in ordered lists may be a closing parenthesis. When another marker is used for a list item
	// the items will be separated for unordered lists and merged with the first list for ordered lists.
	// Lists always come separately when there are one or more newlines between them.
	// Task list checkboxes may be added as `[ ]` or `[x]` (case insensitive) to generate
	// an unchecked- or checked checkbox respectively.
	// - one item
	// - another item
	// or
	// + one item
	// + another item
	// or
	// * one item
	// * another item
	// or
	// 1. first item
	// 2. second item
	// or
	// i. first item
	// ii. second item
	// or
	// a. first item
	// b. second item
	QChar matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != ' ' && matchStartCharacter != '-' && matchStartCharacter != '+'
		&& matchStartCharacter != '*' && !matchStartCharacter.isLetter() && !matchStartCharacter.isDigit()) {
		return false;
	}
	static const QRegularExpression s_regex(
		QLatin1String("(?:^\\h*(?:-|\\+|\\*|(?:[a-z]+|\\d{1,9})(?:\\.|\\)))(?:\\h+.*|$)\n?)++"),
		QRegularExpression::MultilineOption | QRegularExpression::CaseInsensitiveOption);
	return regexMatchAndReplace(
		items, s_regex, "<li>%1</li>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			auto getLetterOrder = [](const QString &letters, bool isUppercase) -> int {
				int letterOrder        = 0;
				char startLetter       = isUppercase ? 'A' : 'a';
				char endLetter         = isUppercase ? 'Z' : 'z';
				int pastEndLetterOrder = endLetter - startLetter + 1;

				// Get the letter order, where each letter except the last one represents
				// how many times the range a-z has been repeated, and in turn how many times
				// the repeats have wrapped, up to the current count:
				qsizetype lastLetterIndex = letters.size() - 1;
				std::string lettersStd    = letters.toStdString();
				for (int i = 0; i <= lastLetterIndex; ++i) {
					int order = lettersStd[(long unsigned int) i] - startLetter + 1;
					letterOrder += ((int) std::pow(pastEndLetterOrder, lastLetterIndex - i)) * order;
				}
				return letterOrder;
			};
			auto romanToInt = [](QString roman) -> int {
				static const QHash< QChar, int > romanToIntMap = { { 'i', 1 },   { 'v', 5 },   { 'x', 10 },
																   { 'l', 50 },  { 'c', 100 }, { 'd', 500 },
																   { 'm', 1000 } };
				roman                                          = roman.toLower();

				// Get the roman numerals as a number, adding the value of each letter,
				// where cases of the next letter being greater than the current one are handled
				// by subtracting the next letter by the current letter and then moving past both.
				// If any letter is invalid -1 is returned:
				int sum                           = 0;
				qsizetype lastRomanCharacterIndex = roman.size() - 1;
				for (int i = 0; i <= lastRomanCharacterIndex; ++i) {
					int romanAsInt            = romanToIntMap.value(roman[i]);
					bool isNextCharacter      = i < lastRomanCharacterIndex;
					bool isMainCharacterRoman = romanAsInt != 0;
					bool isNextCharacterRoman = false;

					if (isMainCharacterRoman && isNextCharacter) {
						int nextRomanAsInt   = romanToIntMap.value(roman[i + 1]);
						isNextCharacterRoman = nextRomanAsInt != 0;
						if (isNextCharacterRoman && nextRomanAsInt > romanAsInt) {
							romanAsInt = nextRomanAsInt - romanAsInt;
							++i;
						}
					}
					if (!isMainCharacterRoman || (isNextCharacter && !isNextCharacterRoman)) {
						return -1;
					}
					sum += romanAsInt;
				}
				return sum;
			};
			auto getSeparatorIndex = [](const QString &item) -> int {
				qsizetype separatorIndex = item.indexOf('.');
				if (separatorIndex == -1) {
					separatorIndex = item.indexOf(')');
				}
				return (int) separatorIndex;
			};
			auto getIndent = [](QString &item) -> int {
				int indent = -1;
				while (++indent < item.size() && item[indent].isSpace()) {
				}
				return indent;
			};
			static const int indentPerSublist = 2;

			QStringList allListItems  = match.captured(0).split('\n', Qt::SkipEmptyParts);
			qsizetype totalItemAmount = allListItems.size();
			QList< std::tuple< QStringList, int, int, QChar > > lists;

			int itemIndex              = 0;
			int currentSublistNumber   = 0;
			int nextSublistNumber      = 0;
			int firstRomanAsIntInList  = -2;
			QChar firstCharacterInList = ' ';
			bool isAnyListItemAdded    = false;
			while (itemIndex < totalItemAmount) {
				bool isOrderedList   = false;
				bool isRomanNumerals = false;
				bool isAlphabetical  = false;
				bool isUppercase     = false;

				bool isFirstIteration = true;
				QStringList list;
				int listStartNumber    = 1;
				int listStartNumberAlt = 1;
				do {
					QString item      = allListItems.at(itemIndex);
					nextSublistNumber = isAnyListItemAdded ? getIndent(item) / indentPerSublist : 0;
					if (currentSublistNumber != nextSublistNumber) {
						break;
					}
					isAnyListItemAdded = true;

					item                        = item.trimmed();
					QString listStartCharacters = item.first(getSeparatorIndex(item));
					QChar firstCharacter        = listStartCharacters[0];

					bool isItemForOrderedList = firstCharacter != '-' && firstCharacter != '+' && firstCharacter != '*';
					int romanAsInt            = romanToInt(listStartCharacters);
					bool isItemRomanNumerals =
						isItemForOrderedList && romanAsInt > 0
						&& (firstRomanAsIntInList == -2 || romanAsInt - 1 == firstRomanAsIntInList);
					bool isItemAlphabetical = isItemForOrderedList && firstCharacter.isLetter();
					bool isItemUppercase    = firstCharacter.isUpper();

					int letterOrder = isItemAlphabetical ? getLetterOrder(listStartCharacters, isItemUppercase) : 1;
					int itemNumber  = !isItemForOrderedList || !isFirstIteration
										 ? 1
										 : isItemRomanNumerals
											   ? romanAsInt
											   : isItemAlphabetical ? letterOrder : listStartCharacters.toInt();

					if (isFirstIteration) {
						isOrderedList   = isItemForOrderedList;
						isRomanNumerals = isItemRomanNumerals;
						isAlphabetical  = isItemAlphabetical;
						isUppercase     = isItemUppercase;

						firstCharacterInList  = firstCharacter;
						firstRomanAsIntInList = romanAsInt;
						listStartNumber       = itemNumber;
						listStartNumberAlt    = letterOrder;
						isFirstIteration      = false;
					} else {
						if ((isOrderedList && !isItemForOrderedList) || (!isOrderedList && isItemForOrderedList)
							|| (isUppercase && !isItemUppercase) || (!isUppercase && isItemUppercase)
							|| (!isOrderedList && firstCharacterInList != firstCharacter)) {
							break;
						}
						if (isRomanNumerals && !isItemRomanNumerals && firstRomanAsIntInList != -2) {
							listStartNumber = listStartNumberAlt;
							isRomanNumerals = false;
						}
						firstRomanAsIntInList = -2;
					}
					list.append(item);
				} while (++itemIndex < totalItemAmount);

				if (!list.isEmpty()) {
					QChar orderedListType =
						!isOrderedList
							? 'u'
							: isRomanNumerals && isUppercase
								  ? 'I'
								  : isRomanNumerals ? 'i'
													: isAlphabetical && isUppercase ? 'A' : isAlphabetical ? 'a' : '1';
					lists.append(std::make_tuple(list, currentSublistNumber, listStartNumber, orderedListType));
				}
				int currentSublistNumberPlusOne = currentSublistNumber + 1;
				int sublistOverflow             = nextSublistNumber - currentSublistNumberPlusOne;
				bool isSublistOverflow          = sublistOverflow > 0;
				currentSublistNumber            = isSublistOverflow ? currentSublistNumberPlusOne : nextSublistNumber;
				if (!isSublistOverflow) {
					continue;
				}

				int indentOverflow         = sublistOverflow * indentPerSublist;
				int minTargetSublistIndent = -1;
				for (int i = itemIndex; i < totalItemAmount; ++i) {
					QString &item = allListItems[i];
					int indent    = getIndent(item);
					if (minTargetSublistIndent == -1) {
						minTargetSublistIndent = indent;
					} else if (indent < minTargetSublistIndent) {
						break;
					}
					item.remove(0, indentOverflow);
				}
			}

			QStringList htmlListItems;
			int previousSublistNumber = 0;
			for (auto [list, sublistNumber, listStartNumber, orderedListType] : lists) {
				QString unorderedListType(
					QLatin1String(sublistNumber < 1 ? "disc" : sublistNumber < 2 ? "circle" : "square"));
				bool isSublist     = sublistNumber > 0;
				bool isOrderedList = orderedListType != 'u';

				for (QString &item : list) {
					int contentStartIndex = isOrderedList ? getSeparatorIndex(item) + 2 : 2;
					QString itemContent   = item.size() > contentStartIndex ? item.remove(0, contentStartIndex) : "";

					if (itemContent.startsWith("[ ]")) {
						itemContent.replace(0, 3, "&#xFFA0;☐");
					}
					itemContent.replace("[ ]", "☐");
					itemContent.replace("[x]", "✅", Qt::CaseInsensitive);
					processFormatting(itemContent, items,
									  { processMarkdownEmphasisOrScript, processMarkdownInlineCode,
										processMarkdownHeader, processImageOrLink, processMarkdownBlockQuote,
										processColor, processLinebreak, processHTMLFixedSpace, processHTMLComment });
					item = QLatin1String(contentWrapper).arg(!itemContent.isEmpty() ? itemContent : "&#xFFA0;");
				}
				QString listStyle(QLatin1String(sublistNumber > 0 ? "margin-left:-10;"
																  : previousSublistNumber > 0 ? "margin-top:0;" : ""));
				list.prepend((isOrderedList
								  ? QLatin1String("<ol type=\"%1\" start=\"%2\" style=\"%3\">")
										.arg(orderedListType)
										.arg(listStartNumber)
										.arg(listStyle)
								  : QLatin1String("<ul type=\"%1\" style=\"%2\">").arg(unorderedListType, listStyle)));
				list.append(QLatin1String("</%1l>").arg(isOrderedList ? "o" : "u"));

				if (isSublist) {
					int listStartOffsetFromEnd = 1 + indentPerSublist * (sublistNumber - 1);
					qsizetype listStartIndex   = htmlListItems.size() - listStartOffsetFromEnd;
					list.prepend("<li>");
					list.append("</li>");
					for (int i = 0; i < listStartOffsetFromEnd; ++i) {
						list.append(htmlListItems.takeAt(listStartIndex));
					}
				}
				htmlListItems.append(list);
				previousSublistNumber = sublistNumber;
			}
			return QLatin1String("%1").arg(htmlListItems.join(""));
		});
}

/// Tries to match and replace a Markdown description list at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownDescriptionList(Markdown::Items &items) {
	// Description list in the following format, where there may be
	// multiple terms and details on consecutive rows
	// term
	// : details
	static const QRegularExpression s_regex(QLatin1String("(?:(?:(?:\n|^)(?!:|\\s).+)++(?:\n:\\h+.*$)++)++"),
											QRegularExpression::MultilineOption);
	return regexMatchAndReplace(
		items, s_regex, "<dl>%1</dl>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			QStringList termsAndDetails = match.captured(0).split('\n');
			QStringList htmlListItems;

			QString headerStyle("margin-bottom:0;");
			bool isFirstIteration = true;
			for (QString &content : termsAndDetails) {
				bool isDetail = content.startsWith(':');
				if (isDetail) {
					content.remove(0, 2);
				}

				processFormatting(content, items,
								  { processMarkdownEmphasisOrScript, processMarkdownInlineCode, processImageOrLink,
									processMarkdownBlockQuote, processColor, processLinebreak, processHTMLFixedSpace,
									processHTMLComment });
				htmlListItems.append(
					isDetail ? QLatin1String("<dd>%1</dd>").arg(content)
							 : QLatin1String("<dt><h3 style=\"%2\">%1</h3></dt>").arg(content, headerStyle));
				if (isFirstIteration) {
					headerStyle.append("margin-top:0;");
					isFirstIteration = false;
				}
			}
			return QLatin1String(contentWrapper).arg(htmlListItems.join(""));
		});
}

/// Tries to match and replace a Markdown list or description list at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processList(Markdown::Items &items) {
	return processMarkdownList(items) || processMarkdownDescriptionList(items);
}

/// Tries to match and replace a Markdown table at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownTable(Markdown::Items &items) {
	// Table in the following format, where there may be a colon to the left and to the right of
	// each column's hyphen(s) on the row right after the table headers which determines if the
	// text of the column is aligned to the left, right or center accordingly
	// first column | second column
	// -|-
	// first item | first item
	// second item | second item
	static const QRegularExpression s_regex(
		QLatin1String("^.+\\|+.*\n\\|*\\h*:?-+(?::?\\h*\\|\\h*:?-*:?)++\\h*\\|*(?:\n.+)*"),
		QRegularExpression::MultilineOption);
	return regexMatchAndReplace(
		items, s_regex, "<table class=\"table\">%1</table>",
		[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			static const QRegularExpression s_unescapedPipes(QLatin1String("(?<!\\\\)\\|"));
			QStringList rowsMarkdown = match.captured(0).split('\n');
			QStringList columnAlignments;
			QStringList tableRows;

			QStringList columnAlignmentsMarkdown = rowsMarkdown.takeAt(1).split(s_unescapedPipes, Qt::SkipEmptyParts);
			for (QString &column : columnAlignmentsMarkdown) {
				column = column.trimmed();
				if (column.isEmpty()) {
					continue;
				}
				bool isToTheLeft  = column.startsWith(':');
				bool isToTheRight = column.endsWith(':');
				columnAlignments.append(QLatin1String(
					isToTheLeft && isToTheRight ? "center" : isToTheLeft ? "left" : isToTheRight ? "right" : ""));
			}

			qsizetype columnAmount = columnAlignments.size();
			bool isHeaderRow       = true;
			for (QString &rowMarkdown : rowsMarkdown) {
				QStringList columnsMarkdown = rowMarkdown.split(s_unescapedPipes, Qt::SkipEmptyParts);
				qsizetype columnEntryAmount = 0;

				QStringList columns;
				for (int i = 0; i < columnsMarkdown.size(); ++i) {
					QString column = columnsMarkdown[i].trimmed();
					if (column.isEmpty()) {
						continue;
					}
					if (++columnEntryAmount > columnAmount) {
						return doNoReplacementSign;
					}
					processFormatting(column, items,
									  { processMarkdownEmphasisOrScript, processMarkdownInlineCode, processImageOrLink,
										processColor, processLinebreak, processHTMLFixedSpace, processHTMLComment });
					columns.append(QLatin1String("<t%3 align=\"%2\">%1</t%3>")
									   .arg(column, columnAlignments[i], isHeaderRow ? "h" : "d"));
				}
				if (isHeaderRow && columnEntryAmount != columnAmount) {
					return doNoReplacementSign;
				}

				tableRows.append(QLatin1String("<tr>%1</tr>").arg(columns.join("")));
				isHeaderRow = false;
			}
			return QLatin1String(contentWrapper).arg(tableRows.join(""));
		});
}

/// Tries to match and replace a Markdown quote (block quote) at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownBlockQuote(Markdown::Items &items) {
	// Block quotes are (consecutive) lines starting with ">" or "> " where each nested
	// block quote has an additional ">" and ends on the first otherwise empty line with less ">"
	// The start may be preceded by 3 spaces at most
	QChar matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '>' && matchStartCharacter != ' ') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("^\\h{,3}>.*(?:\n.+)*\n?"),
											QRegularExpression::MultilineOption);
	return regexMatchAndReplace(
		items, s_regex, "<table class=\"blockquote\" width=\"100%\"><tr><td>%1</td></tr></table>",
		[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			QString quote     = match.captured(0);
			QStringList lines = quote.split('\n', Qt::SkipEmptyParts);

			qsizetype lastLineIndex = lines.size() - 1;
			quote.clear();
			for (int i = 0; i <= lastLineIndex; ++i) {
				QString line = lines[i].trimmed();
				// Remove the leading ">" or "> " and add linebreak back in:
				if (line.startsWith('>')) {
					int indicatorLength = line.size() > 1 && line[1].isSpace() ? 2 : 1;
					line.remove(0, indicatorLength);
				}
				quote += line.append(QLatin1String(i != lastLineIndex ? "\n" : ""));
			}

			processFormatting(quote, items,
							  { processMarkdownEmphasisOrScript, processCode, processMarkdownThematicBreak,
								processMarkdownHeader, processList, processImageOrLink, processMarkdownBlockQuote,
								processMarkdownTable, processColor, processLinebreak, processHTMLFixedSpace,
								processHTMLComment });
			// Remove one newline before each block-element used by other formats
			// and replace the rest with effectual linebreaks in HTML:
			quote.replace(QRegularExpression(QLatin1String("\n(<(?:table|(?:o|u|d)l))")), "\\1");
			return QLatin1String(contentWrapper).arg(quote.replace('\n', "<br/>"));
		});
}

/// Tries to match and replace a Markdown centered-block at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownCenteredBlock(Markdown::Items &items) {
	// Centered block is marked as -> centered <- where the spaces may be newlines
	QChar matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '-' && matchStartCharacter != ' ') {
		return false;
	}
	static const QRegularExpression s_regex(QLatin1String("-> ((?:.|\n)+) <-|^\\h{,3}->\n((?:.|\n)+)\n<-"),
											QRegularExpression::MultilineOption);
	return regexMatchAndReplace(items, s_regex, "<center>%1</center>",
								[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString content = getFirstMatchedGroup(match);
									processFormatting(content, items,
													  {
														  processMarkdownEmphasisOrScript,
														  processCode,
														  processMarkdownThematicBreak,
														  processMarkdownHeader,
														  processList,
														  processImageOrLink,
														  processMarkdownBlockQuote,
														  processMarkdownTable,
														  processColor,
														  processLinebreak,
														  processHTMLFixedSpace,
														  processHTMLComment,
													  });
									return QLatin1String(contentWrapper).arg(content);
								});
}

QString markdownToHTML(const QString &markdownInput) {
	QList< Markdown::Image > images;
	QHash< QString, std::tuple< QString, QString > > references;
	Markdown::Items items = { markdownInput, (qsizetype) 0, QLatin1String(), images, references };

	processFormatting(items, { processMarkdownEmphasisOrScript, processCode, processMarkdownThematicBreak,
							   processMarkdownHeader, processList, processImageOrLink, processMarkdownBlockQuote,
							   processMarkdownTable, processColor, processLinebreak, processHTMLFixedSpace,
							   processHTMLComment, processMarkdownCenteredBlock });
	processQueuedMarkdownReferences(items);
	processQueuedMarkdownImages(items);

	return items.htmlStr;
}
} // namespace Markdown
