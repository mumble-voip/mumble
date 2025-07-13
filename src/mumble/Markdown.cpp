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
	QString alt;
	QString title;
	int width;
	int height;
};
struct Items {
	const QString &inputStr;
	qsizetype offset;
	QString html;
	QList< Markdown::Image > &images;
	QHash< QString, std::tuple< QString, QString > > &references;
};

// Placeholder constants
const QLatin1String imagePlaceholder("%<\\!!image!!//>@");
const QLatin1String referencePlaceholder("%<\\!!reference!!//>@");

// Sign constant for use in a function where this value cancels the replacement
const QString doNoReplacementSign("\\");

/// Just a wrapper for QRegularExpression::match() where it will only match
/// if the match starts at the given offset
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
/// @returns The first non-empty group or else an empty string
QString findFirstMatchedGroup(const QRegularExpressionMatch &match, int groupStartIndex = 1, int groupEndIndex = -1) {
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
							  : QLatin1String(contentWrapper).arg(findFirstMatchedGroup(match).toHtmlEscaped());
	bool isReplacement = replacement != doNoReplacementSign;
	if (isReplacement) {
		items.html += replacement;
		items.offset = match.capturedEnd();
	}
	return isReplacement;
}

/// Tries to escape a character at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool escapeCharacter(Markdown::Items &items) {
	qsizetype &offset      = items.offset;
	const QChar &charToEsc = items.inputStr[offset];
	QString escapedChar    = QString(charToEsc).toHtmlEscaped();
	if (escapedChar.size() == 1 && escapedChar[0] == charToEsc) {
		// Nothing to escape
		return false;
	}

	items.html += escapedChar;
	++offset;
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

/// Makes the given text compatible as either a reference or a URL
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

	bool isSquareBracketsInsteadOfParentheses = endBracket == "]";
	QString startBracketOfThisType(isSquareBracketsInsteadOfParentheses ? "[" : "(");
	QString endBracketOfThisType(isSquareBracketsInsteadOfParentheses ? "]" : ")");

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
	static const QRegularExpression regex(QLatin1String("\\\\(.)"));
	return regexMatchAndReplace(items, regex, "%1");
}

/// Tries to match and replace plain linebreaks at exactly the given offset in the string.
/// Double linebreaks are filled in with `br` elements and single linebreaks are removed.
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processPlainLinebreaks(Markdown::Items &items) {
	const QChar &matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '\n' && matchStartCharacter != '\r') {
		return false;
	}
	static const QRegularExpression regex(QLatin1String("\\R{2}"));
	return regexMatchAndReplace(items, regex, "<br/><br/>",
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
/// @param formats The functions to try to replace patterns in the input text with
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
			items.html += inputStr[offset++];
		}
	}
}

/// Tries to match and replace characters at every offset in the string with the given formats
///
/// @param inputStr A reference to the input text to work on. This will be modified to contain the HTML output.
/// @param items A reference to the items to work on
/// @param formats The functions to try to replace patterns in the input text with
void processFormatting(QString &inputStr, Markdown::Items &items,
					   QList< std::function< bool(Markdown::Items &) > > formats) {
	Markdown::Items substringItems = { inputStr, (qsizetype) 0, QLatin1String(), items.images, items.references };
	substringItems.html.reserve(inputStr.size());
	processFormatting(substringItems, formats);
	substringItems.html.squeeze();
	inputStr = substringItems.html;
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
	static const QRegularExpression regex(QLatin1String("<!--((?:.|\n)*?)-->"));
	return regexMatchAndReplace(items, regex, "");
}

/// Tries to match and replace an HTML fixed space at exactly the given offset in the string
/// Note: Halfwidth Hangul Filler (`&#xFFA0;`) is used instead of the regular non-breaking space (`&nbsp;`) in order
/// to avoid breaking the detection of image messages which results in using the wrong message character limit.
/// It is a somewhat longer space but works the same way in preserving each space instead of collapsing
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
	static const QRegularExpression regex(QLatin1String("&nbsp;|&#xFFA0;"));
	return regexMatchAndReplace(items, regex, "&#xFFA0;");
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
	static const QRegularExpression regex(QLatin1String("<br\\h?/?>"));
	return regexMatchAndReplace(items, regex, "<br/>");
}

/// Tries to match and replace a Markdown linebreak at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownLinebreak(Markdown::Items &items) {
	// Linebreak in the following format
	// text\ or text[2+ spaces]
	// one or more non-whitespace characters somewhere on this line right after the previous one
	const QChar &matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '\\' && matchStartCharacter != ' ') {
		return false;
	}
	static const QRegularExpression regex(QLatin1String("(?:\\\\|\\h{2,})(?=\n.*\\S)"));
	return regexMatchAndReplace(items, regex, "<br/>");
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
	static const QRegularExpression regex(
		QLatin1String("<span style=(?:\"color:\\h?(.+?);\"|'color:\\h?(.+?);')>(.+?)</span>"));
	return regexMatchAndReplace(items, regex, "<span style=\"color: %1;\">%2</span>",
								[](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString color = findFirstMatchedGroup(match, 1, 2).toHtmlEscaped();
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
	static const QRegularExpression regex(QLatin1String("<font color=(?:\"(.+?)\"|'(.+?)')>(.+?)</font>"));
	return regexMatchAndReplace(items, regex, "<font color=\"%1\">%2</font>",
								[](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString color = findFirstMatchedGroup(match, 1, 2).toHtmlEscaped();
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
	static const QRegularExpression regex(QLatin1String("\\$\\\\color\\{(.+?)\\}(?:\\\\text\\{(.+?)\\}"
														"|\\{\\\\text\\{(.+?)\\}\\})\\$"));
	return regexMatchAndReplace(items, regex, "<font color=\"%1\">%2</font>",
								[](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString color = match.captured(1).toHtmlEscaped();
									QString text  = findFirstMatchedGroup(match, 2).toHtmlEscaped();
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
	static const QRegularExpression regex(
		QLatin1String("([a-zA-Z]{1,10}://|[wW]{3}\\.)([A-Za-z0-9-._~:/?#\\[\\]@!$&'()*+,;=]|%[a-fA-F0-9]{2})+"));
	return regexMatchAndReplace(items, regex, "<a href=\"%1\">%2</a>",
								[](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString url = match.captured(0).toHtmlEscaped();

									if (url.startsWith("www", Qt::CaseInsensitive)) {
										// Link is missing a protocol specification - use https as the default
										url.prepend("https://");
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
	static const QRegularExpression regex(
		QLatin1String("(\\[([^\\]\n]+)\\])?(\\(|\\[)(?:<(.+?)>|((?<=\\()[^\\)\\h\n]+|[^\\]\\h\n]+))"
					  "(?:\\h(?:\"([^\"\n]+)\"|'([^'\n]+)'|\\(([^\\)\n]+)\\)))?(\\)|\\])"));
	return regexMatchAndReplace(items, regex, "<a href=\"%1\"%3>%2</a>",
								[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString textWithBrackets = match.captured(1);
									QString text             = match.captured(2);
									QString urlStartBracket  = match.captured(3);
									QString url              = findFirstMatchedGroup(match, 4, 5).toHtmlEscaped();
									QString title            = findFirstMatchedGroup(match, 6, 8).toHtmlEscaped();
									QString urlEndBracket    = match.captured(9);
									if (!isSameBracketType(urlStartBracket, urlEndBracket)) {
										return doNoReplacementSign;
									}

									bool isReference = urlEndBracket == "]";
									if (!isReference && !url.startsWith("http", Qt::CaseInsensitive)) {
										// For a Markdown link to work, it has to start with the protocol specification,
										// e.g. http or https As we can't know for sure that the given website supports
										// https, we'll have to fall back to http Most browsers will upgrade the request
										// to https whenever possible anyways though, so this shouldn't be too much of a
										// problem.
										url.prepend("http://");
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
	const QChar &matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '[' && matchStartCharacter != ' ' && matchStartCharacter != '\n') {
		return false;
	}
	static const QRegularExpression regex(
		QLatin1String("\n*\\h{,3}\\[([^\\]\n]+)\\]:\\h(?:<(.+?)>|([^\\h\n]+))"
					  "(?:\\h(?:\"([^\"\n]+)\"|'([^'\n]+)'|\\(([^\\)\n]+)\\)))?(?:\\h*\n)*\\h*$"),
		QRegularExpression::MultilineOption);
	return regexMatchAndReplace(
		items, regex, "", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			QString reference = match.captured(1).toHtmlEscaped();
			QString url       = findFirstMatchedGroup(match, 2, 3).toHtmlEscaped();
			QString title     = findFirstMatchedGroup(match, 4, 6).toHtmlEscaped();

			if (!url.startsWith("http", Qt::CaseInsensitive) && !url.startsWith("file", Qt::CaseInsensitive)) {
				url.prepend("http://");
			}

			QHash< QString, std::tuple< QString, QString > > &references = items.references;
			if (!references.contains(reference)) {
				references[reference] = { unescapeURL(url), title };
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
	QString &html                     = items.html;
	QString regexReferencePlaceholder = QString(referencePlaceholder).replace("\\", "\\\\");
	QRegularExpression regex(QLatin1String("<a\\h.*?href=\"%1(.+?)\".*?>(.+?)</a>").arg(regexReferencePlaceholder));
	qsizetype offset = 0;
	do {
		QRegularExpressionMatch match = regex.match(html, offset);
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
		html.replace(matchStartIndex, match.capturedLength(), replacement);
		offset = matchStartIndex + replacement.size();
	} while (true);
}

/// Tries to match, replace and queue a Markdown image with a placeholder at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been queued
bool processMarkdownImage(Markdown::Items &items) {
	// Image in format ![alt text](URL "tooltip"){width=px-amount height=px-amount}
	// where the URL may include spaces if wrapped in angular brackets (<>) or be a file URI
	// (local file path starting with "file:///"), the double quotes may be single quotes or parentheses and
	// the alt text is recommended for accessibility but not required
	// A broken image will only be shown if the alt text contains at least one character
	// The tooltip and size attributes are also optional, where in the latter spaces may be commas and
	// each integer may have up to four digits
	// The parentheses containing the URL may be square brackets containing a reference instead
	// The image may be wrapt in a Markdown link
	const QChar &matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '!' && matchStartCharacter != '[') {
		return false;
	}
	static const QRegularExpression regex(
		QLatin1String("\\[?!\\[([^\\]\n]*)\\](\\(|\\[)(?:<(.+?)>|((?<=\\()[^\\)\\h\n]+|[^\\]\\h\n]+))"
					  "(?:\\h(?:\"([^\"\n]+)\"|'([^'\n]+)'|\\(([^\\)\n]+)\\)))?(\\)|\\])"
					  "(?:\\{\\h?(?:(?:width=(\\d{1,4})\\d*(?:\\h|,\\h?))?height=(\\d{1,4})\\d*"
					  "|(?:height=(\\d{1,4})\\d*(?:\\h|,\\h?))?width=(\\d{1,4})\\d*\\h?)?\\})?"
					  "(?:\\](\\(|\\[)(?:<(.+?)>|((?<=\\()[^\\)\\h\n]+|[^\\]\\h\n]+))"
					  "(?:\\h(?:\"([^\"\n]+)\"|'([^'\n]+)'|\\(([^\\)\n]+)\\)))?(\\)|\\]))?"));
	return regexMatchAndReplace(
		items, regex, "<a href=\"%1\"%3>%2</a>",
		[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			QString alt                  = match.captured(1).toHtmlEscaped();
			QString imageUrlStartBracket = match.captured(2);
			QString url                  = findFirstMatchedGroup(match, 3, 4).toHtmlEscaped();
			QString title                = findFirstMatchedGroup(match, 5, 7).toHtmlEscaped();
			QString imageUrlEndBracket   = match.captured(8);

			int width  = (match.captured(9).isEmpty() ? match.captured(12) : match.captured(9)).toInt();
			int height = findFirstMatchedGroup(match, 10, 11).toInt();

			QString linkStartBracket = match.captured(13);
			QString link             = findFirstMatchedGroup(match, 14, 15).toHtmlEscaped();
			QString linkTitle        = findFirstMatchedGroup(match, 16, 18).toHtmlEscaped();
			QString linkEndBracket   = match.captured(19);

			bool hasLink                 = !link.isEmpty();
			bool startsWithSquareBracket = match.captured(0).startsWith('[');
			if (hasLink != startsWithSquareBracket || !isSameBracketType(imageUrlStartBracket, imageUrlEndBracket)
				|| !isSameBracketType(linkStartBracket, linkEndBracket)) {
				return doNoReplacementSign;
			}

			bool isReferenceImage = imageUrlEndBracket == "]";
			bool isReferenceLink  = linkEndBracket == "]";
			if (!isReferenceImage && !url.startsWith("http", Qt::CaseInsensitive)
				&& !url.startsWith("file", Qt::CaseInsensitive)) {
				url.prepend("http://");
			}
			if (!isReferenceLink && !link.startsWith("http", Qt::CaseInsensitive)) {
				link.prepend("http://");
			}

			items.images.append({ referenceOrUnescapeURL(url, isReferenceImage), alt, title, width, height });
			QString linkHrefValue      = referenceOrUnescapeURL(link, isReferenceLink);
			QString linkTitleAttribute = !linkTitle.isEmpty() ? QLatin1String(" title=\"%1\"").arg(linkTitle) : "";
			return hasLink ? QLatin1String(contentWrapper).arg(linkHrefValue, imagePlaceholder, linkTitleAttribute)
						   : imagePlaceholder;
		});
}

/// Tries to request and insert queued Markdown images, replacing their placeholders in the string
/// by order of appearance from start to end. Downloads and loading of local files will be awaited in parallel.
///
/// @param items A reference to the items to work on
void processQueuedMarkdownImages(Markdown::Items &items) {
	QString &html                    = items.html;
	QList< Markdown::Image > &images = items.images;
	qsizetype imageAmount            = images.size();
	if (imageAmount == 0) {
		return;
	}

	QList< std::tuple< QString, QSize, QString > > base64Images(imageAmount);
	QEventLoop loop;
	QNetworkAccessManager networkManager;
	int imageMessageLength                = static_cast< int >(Global::get().uiImageLength);
	qsizetype remainingImageMessageLength = imageMessageLength - html.size() + imageAmount * imagePlaceholder.size();

	qsizetype repliesFinished   = 0;
	QString processingIndicator = "<center><i>%1 image %2/%3...</i></center>";
	ChatbarTextEdit *chatbar    = Global::get().mw->qteChat;
	auto getProcessingFirstWord = [](QString url) {
		return QLatin1String(url.startsWith("file") ? "Loading" : "Downloading");
	};
	QObject::connect(&networkManager, &QNetworkAccessManager::finished, &loop,
					 [&base64Images, &repliesFinished, &imageAmount, &processingIndicator, &remainingImageMessageLength,
					  &loop, &chatbar](QNetworkReply *reply) {
						 if (reply->error() == QNetworkReply::NoError) {
							 QByteArray imageBa = reply->readAll();
							 QImage image;
							 if (image.loadFromData(imageBa)) {
								 int imageIndex = reply->property("index").toInt();

								 qsizetype imageSize       = imageBa.size();
								 qsizetype base64ImageSize = imageSize * 4 / 3;
								 QString imageHeader = imageSize > 2 ? qvariant_cast< QString >(imageBa.first(3)) : "";
								 bool isAnimation    = imageHeader == "GIF";
								 QString img         = isAnimation ? qvariant_cast< QString >(imageBa.toBase64())
														   : Log::imageToImg(image, (int) remainingImageMessageLength);

								 base64Images[imageIndex] = { img, image.size(), isAnimation ? imageHeader : "JPEG" };
								 remainingImageMessageLength -= isAnimation ? base64ImageSize : img.size();
							 }
						 }

						 QString firstWord = reply->property("processingFirstWord").toString();
						 reply->close();
						 reply->deleteLater();
						 if (++repliesFinished == imageAmount) {
							 loop.quit();
							 return;
						 }
						 chatbar->setHtml(processingIndicator.arg(firstWord).arg(repliesFinished).arg(imageAmount));
					 });
	for (int i = 0; i < imageAmount; ++i) {
		Markdown::Image &image = images[i];
		QString &url           = image.url;
		if (url.startsWith(referencePlaceholder)) {
			url.remove(0, referencePlaceholder.size());
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
		auto [_, alt, title, width, height] = images[i];
		auto [base64Image, imgSize, imgFmt] = base64Images[i];

		bool isWidth  = width > 0;
		bool isHeight = height > 0;
		if (isWidth || isHeight) {
			// Use the max size if the set image resize is too large, which with a max message area size of
			// 2048x2048 somehow becomes around 1442x1442 (approximately 70% of the max message area size) for images:
			int allowedEvenLength              = (int) round(Global::get().uiMessageEvenAreaLength * 0.7041);
			int allowedSize                    = allowedEvenLength * allowedEvenLength;
			auto [maxBaseWidth, maxBaseHeight] = Global::get().uiImageAreaSize;
			if (imgSize.width() > maxBaseWidth || imgSize.height() > maxBaseHeight) {
				imgSize.scale(maxBaseWidth, maxBaseHeight, Qt::KeepAspectRatio);
			}
			if (!isWidth) {
				double widthToHeightRatio = imgSize.width() / (double) imgSize.height();
				width                     = (int) round(height * widthToHeightRatio);
			} else if (!isHeight) {
				double heightToWidthRatio = imgSize.height() / (double) imgSize.width();
				height                    = (int) round(width * heightToWidthRatio);
			}

			int imageSize = width * height;
			if (imageSize > allowedSize) {
				bool isWidthGreaterThanHeight = width > height;
				int &lengthAxis1              = isWidthGreaterThanHeight ? width : height;
				int &lengthAxis2              = isWidthGreaterThanHeight ? height : width;

				while (--lengthAxis1 * lengthAxis2 > allowedSize && lengthAxis1 > allowedEvenLength) {
				}
				if (lengthAxis1 * lengthAxis2 > allowedSize) {
					while (lengthAxis1 * --lengthAxis2 > allowedSize && lengthAxis2 > allowedEvenLength) {
					}
				}
				isWidth  = width > 0;
				isHeight = height > 0;
			}
		}
		bool isTitle                            = !title.isEmpty();
		QStringList imgAdditionalAttributesList = {
			isTitle ? QLatin1String("title=\"%1\"").arg(title) : "",
			isWidth ? QLatin1String("width=\"%1\"").arg(QString::number(width)) : "",
			isHeight ? QLatin1String("height=\"%1\"").arg(QString::number(height)) : ""
		};
		imgAdditionalAttributesList.removeAll("");

		qsizetype imageOffset     = html.indexOf(imagePlaceholder);
		qsizetype placeholderSize = imagePlaceholder.size();
		QString img               = base64Image.startsWith("<img ")
						  ? base64Image
						  : QLatin1String("<img src=\"data:image/%2;base64,%1\" />").arg(base64Image, imgFmt);
		bool isImage = !base64Image.isEmpty();

		// Do not apply width and height attributes to the broken image icon too:
		if (isImage) {
			img.insert(img.size() - 2, imgAdditionalAttributesList.join(' '));
		}
		// Remove a-element wrapper if the image failed to load so that the broken image and alt text may be visible:
		if (!isImage && imageOffset > 0 && html[imageOffset - 1] == '>') {
			// Start with the constant size of the end tag `</a>`:
			int sizeIncrease = 4;
			do {
				++sizeIncrease;
			} while (imageOffset > 0 && html[--imageOffset] != '<');
			placeholderSize += sizeIncrease;
		}

		html.replace(imageOffset, placeholderSize, isImage ? img : !alt.isEmpty() ? img + alt : "");
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

bool processMarkdownScript(Markdown::Items &items);

/// Tries to match and replace a Markdown italic-text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownItalic(Markdown::Items &items) {
	// Italic text in format *italic*
	if (items.inputStr[items.offset] != '*') {
		return false;
	}
	static const QRegularExpression regex(QLatin1String("\\*(?!\\h+\\*)([^*\n]+)\\*"));
	return regexMatchAndReplace(
		items, regex, "<i>%1</i>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			QString content = match.captured(1);
			processFormatting(content, items,
							  { processMarkdownLine, processMarkdownScript, processLink, processColor, processLinebreak,
								processHTMLFixedSpace, processHTMLComment });
			return QLatin1String(contentWrapper).arg(content);
		});
}

/// Tries to match and replace a Markdown bold-text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownBold(Markdown::Items &items) {
	// Bold text in format **bold**
	if (items.inputStr[items.offset] != '*') {
		return false;
	}
	static const QRegularExpression regex(QLatin1String("\\*{2}(?!\\h*\\*{2,})(.+?\\*?)\\*{2}"));
	return regexMatchAndReplace(
		items, regex, "<b>%1</b>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			QString content = match.captured(1);
			processFormatting(content, items,
							  { processMarkdownLine, processMarkdownItalic, processMarkdownScript, processLink,
								processColor, processLinebreak, processHTMLFixedSpace, processHTMLComment });
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
	// Strikethrough text in format ~~text~~ with no more surrounding tildes
	if (items.inputStr[items.offset] != '~') {
		return false;
	}
	static const QRegularExpression regex(QLatin1String("(?<!~)~~(?!\\h+~|~)(.+?)(?<!~)~~(?!~)"));
	return regexMatchAndReplace(items, regex, "<s>%1</s>",
								[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString content = match.captured(1);
									processFormatting(content, items,
													  { processMarkdownUnderline, processMarkdownBoldOrItalic,
														processMarkdownScript, processLink, processColor,
														processLinebreak, processHTMLFixedSpace, processHTMLComment });
									return QLatin1String(contentWrapper).arg(content);
								});
}

/// Tries to match and replace a Markdown underline-text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownUnderline(Markdown::Items &items) {
	// Underline text in format __text__
	if (items.inputStr[items.offset] != '_') {
		return false;
	}
	static const QRegularExpression regex(QLatin1String("__(?!\\h+_)(.+?)__"));
	return regexMatchAndReplace(items, regex, "<u>%1</u>",
								[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString content = match.captured(1);
									processFormatting(content, items,
													  { processMarkdownStrikethrough, processMarkdownBoldOrItalic,
														processMarkdownScript, processLink, processColor,
														processLinebreak, processHTMLFixedSpace, processHTMLComment });
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
	// Superscript text in format ^text^
	if (items.inputStr[items.offset] != '^') {
		return false;
	}
	static const QRegularExpression regex(QLatin1String("\\^(?!\\h+\\^)([^\\^\n]+)\\^"));
	return regexMatchAndReplace(items, regex, "<sup>%1</sup>",
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
	// Subscript text in format ~text~
	if (items.inputStr[items.offset] != '~') {
		return false;
	}
	static const QRegularExpression regex(QLatin1String("~(?!\\h+~)([^~\n]+)~"));
	return regexMatchAndReplace(items, regex, "<sub>%1</sub>",
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
bool processMarkdownScript(Markdown::Items &items) {
	return processMarkdownSuperscript(items) || processMarkdownSubscript(items);
}

/// Tries to match and replace Markdown emphasis or script text at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownEmphasisOrScript(Markdown::Items &items) {
	return processMarkdownEmphasis(items) || processMarkdownScript(items);
}

/// Tries to match and replace a Markdown section header at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownHeader(Markdown::Items &items) {
	// Match a Markdown section heading in one of the following formats, where the "#" may repeat up to 6 times
	// to generate an h1-h6 element accordingly. If instead triple equals or triple hyphens on the following
	// line is used, or more of the same character in a row, then the former is h1 and the latter is h2.
	// If a number of "#" in a row are used at least one space after a header starting with "#" then these are consumed.
	// Also eat up a potential following newline in order to not create a huge spacing after the heading.
	// # header
	// or
	// header
	// ===
	// or
	// header
	// ---
	static const QRegularExpression regex(
		QLatin1String("^\\h{,3}(?:(#+)\\h(?:(.+)\\h+#+\\h*|(.+))|(.+)\n\\h{,3}(={3,}|-{3,})\\h*$)\n?"),
		QRegularExpression::MultilineOption);
	return regexMatchAndReplace(
		items, regex, "<h%1>%2</h%1>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			const auto sectionLevel =
				match.captured(1).isEmpty() ? (match.captured(5).contains('=') ? 1 : 2) : match.captured(1).size();
			QString sectionName = findFirstMatchedGroup(match, 2, 4).trimmed();

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
	// Thematic break in format *** or --- or ___ and the character used may repeat more than 3 times,
	// as well as with spaces between if the first one is placed 3 spaces in at most
	const QChar &matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '*' && matchStartCharacter != '-' && matchStartCharacter != '_') {
		return false;
	}
	static const QRegularExpression regex(
		QLatin1String("^\\h{,3}(\\*(?:\\h*\\*\\h*){2,}|-(?:\\h*-\\h*){2,}|_(?:\\h*_\\h*){2,})(?:\n$|$)"),
		QRegularExpression::MultilineOption);
	return regexMatchAndReplace(items, regex, "<hr/>");
}

/// Tries to match and replace a Markdown inline-code-snippet at exactly the given offset in the string
///
/// @param items A reference to the items to work on
/// @returns Whether a replacement has been made
bool processMarkdownInlineCode(Markdown::Items &items) {
	// Inline code in format `code`{.lang} where the backticks may use additional pairs of backticks
	// to escape more backticks in the code and specifying the language is optional
	if (items.inputStr[items.offset] != '`') {
		return false;
	}
	static const QRegularExpression regex(
		QLatin1String("(`+)(?!\\h*\\1)(.+?)\\1(?!`)(?:\\{\\h?(?:\\.([a-z]+))?\\h?\\})?"),
		QRegularExpression::CaseInsensitiveOption);
	return regexMatchAndReplace(items, regex, "<code%2>%1</code>",
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
	const QChar &matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '`' && matchStartCharacter != '~' && matchStartCharacter != ' ') {
		return false;
	}
	static const QRegularExpression regex(
		QLatin1String("^\\h{,3}(`{3,}|~{3,})([a-z]*)\n((?:.|\n)+?)\n\\h{,3}\\1(?!`)|(?:(?:\n|^)\\h{4,}.*)++"),
		QRegularExpression::MultilineOption | QRegularExpression::CaseInsensitiveOption);
	return regexMatchAndReplace(
		items, regex, "<table class=\"codeblock%2\" width=\"100%\"><tr><td><pre>%1</pre></td></tr></table>",
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
			return QLatin1String(contentWrapper).arg(code, isLanguage ? language.prepend(' ') : "");
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
	// List in one of the following formats (case insensitive), where ordered lists may start from
	// any non-negative integer up to 9 digits and any combination of unordered lists and ordered lists
	// may be nested with 2 additional spaces past the start of the previous list item marker for each
	// sublist on top of it. Any further spaces will be reduced to that of the nearest next sublist.
	// The period in ordered lists may be a closing parenthesis. When another marker is used for a list item the items
	// will be separated. Lists always come separately when there is at least one line without a list item between them.
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
	static const QRegularExpression regex(
		QLatin1String("(?:^\\h*(?:-|\\+|\\*|(?:[a-z]+|\\d{1,9})(?:\\.|\\)))(?:\\h+.*|$)\n?)++"),
		QRegularExpression::MultilineOption | QRegularExpression::CaseInsensitiveOption);
	return regexMatchAndReplace(
		items, regex, "<li>%1</li>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			auto calcLetterOrder = [](const QString &letters, bool isUppercase) -> int {
				int letterOrder         = 0;
				char startLetter        = isUppercase ? 'A' : 'a';
				char endLetter          = isUppercase ? 'Z' : 'z';
				int afterEndLetterOrder = endLetter - startLetter + 1;

				// Calculate the letter order, where each letter except the last one represents
				// how many times the range a-z has been repeated, and in turn how many times
				// the repeats have wrapped, up to the current count:
				qsizetype lastLetterIndex = letters.size() - 1;
				std::string lettersStd    = letters.toStdString();
				for (int i = 0; i <= lastLetterIndex; ++i) {
					int order = lettersStd[(long unsigned int) i] - startLetter + 1;
					letterOrder += ((int) std::pow(afterEndLetterOrder, lastLetterIndex - i)) * order;
				}
				return letterOrder;
			};
			auto romanToInt = [](QString roman) -> int {
				QHash< QChar, int > romanToIntMap = { { 'i', 1 },   { 'v', 5 },   { 'x', 10 },  { 'l', 50 },
													  { 'c', 100 }, { 'd', 500 }, { 'm', 1000 } };
				roman                             = roman.toLower();

				// Convert the roman numerals to an arabic numeral, adding the value of each letter,
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
			auto findSeparatorIndex = [](const QString &item, bool *isOrderedItem = nullptr) -> int {
				if (isOrderedItem != nullptr) {
					const QChar &firstCharacter = item[0];
					bool &refIsOrderedItem      = *isOrderedItem;
					refIsOrderedItem = firstCharacter != '-' && firstCharacter != '+' && firstCharacter != '*';
					if (!refIsOrderedItem) {
						return 0;
					}
				}

				qsizetype separatorIndex = item.indexOf('.');
				if (separatorIndex == -1) {
					separatorIndex = item.indexOf(')');
				}
				return (int) separatorIndex;
			};
			auto findIndent = [](const QString &item) -> int {
				int indent = -1;
				while (++indent < item.size() && item[indent].isSpace()) {
				}
				return indent;
			};

			const int indentPerSublist = 2;
			QStringList allListItems   = match.captured(0).split('\n', Qt::SkipEmptyParts);
			qsizetype totalItemAmount  = allListItems.size();
			QList< std::tuple< QStringList, int, int, QChar > > lists;

			int itemIndex              = 0;
			int currentSublistNumber   = 0;
			int nextSublistNumber      = 0;
			int firstRomanAsIntInList  = -2;
			QChar firstSeparatorInList = ' ';
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
					QString item      = allListItems[itemIndex];
					nextSublistNumber = isAnyListItemAdded ? findIndent(item) / indentPerSublist : 0;
					if (currentSublistNumber != nextSublistNumber) {
						break;
					}
					isAnyListItemAdded = true;

					item                        = item.trimmed();
					bool isOrderedItem          = false;
					QString listStartCharacters = item.first(findSeparatorIndex(item, &isOrderedItem) + 1);
					QChar separator             = listStartCharacters.back();
					const QChar &firstCharacter = listStartCharacters[0];
					listStartCharacters.chop(1);

					int romanAsInt = romanToInt(listStartCharacters);
					bool isItemRomanNumerals =
						isOrderedItem && romanAsInt > 0
						&& (firstRomanAsIntInList == -2 || romanAsInt - 1 == firstRomanAsIntInList);
					bool isItemAlphabetical = isOrderedItem && firstCharacter.isLetter();
					bool isItemUppercase    = firstCharacter.isUpper();

					int letterOrder = isItemAlphabetical ? calcLetterOrder(listStartCharacters, isItemUppercase) : 1;
					int itemNumber  = !isOrderedItem || !isFirstIteration
										 ? 1
										 : isItemRomanNumerals
											   ? romanAsInt
											   : isItemAlphabetical ? letterOrder : listStartCharacters.toInt();

					if (isFirstIteration) {
						isOrderedList   = isOrderedItem;
						isRomanNumerals = isItemRomanNumerals;
						isAlphabetical  = isItemAlphabetical;
						isUppercase     = isItemUppercase;

						firstSeparatorInList  = separator;
						firstRomanAsIntInList = romanAsInt;
						listStartNumber       = itemNumber;
						listStartNumberAlt    = letterOrder;
						isFirstIteration      = false;
					} else {
						if (isOrderedList != isOrderedItem || isUppercase != isItemUppercase
							|| firstSeparatorInList != separator) {
							break;
						}
						if (isRomanNumerals && !isItemRomanNumerals && firstRomanAsIntInList != -2) {
							listStartNumber = listStartNumberAlt;
							isRomanNumerals = false;
						}
						// Set list items after the second one to be corrected as roman numerals if the first one was
						// unless it was shown to be alphabetical by the second item:
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
					lists.append({ list, currentSublistNumber, listStartNumber, orderedListType });
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
					int indent    = findIndent(item);
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
				QString unorderedListType =
					QLatin1String(sublistNumber < 1 ? "disc" : sublistNumber < 2 ? "circle" : "square");
				bool isSublist     = sublistNumber > 0;
				bool isOrderedList = orderedListType != 'u';

				for (QString &item : list) {
					int contentStartIndex = isOrderedList ? findSeparatorIndex(item) + 2 : 2;
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
				QString listStyle = QLatin1String(sublistNumber > 0 ? "margin-left:-10;"
																	: previousSublistNumber > 0 ? "margin-top:0;" : "");
				QString listStyleAttribute(!listStyle.isEmpty() ? QLatin1String(" style=\"%1\"").arg(listStyle) : "");
				list.prepend((isOrderedList
								  ? QLatin1String("<ol type=\"%1\" start=\"%2\"%3>")
										.arg(orderedListType)
										.arg(listStartNumber)
										.arg(listStyleAttribute)
								  : QLatin1String("<ul type=\"%1\"%2>").arg(unorderedListType, listStyleAttribute)));
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
	static const QRegularExpression regex(QLatin1String("(?:(?:(?:\n|^)(?!:|\\s).+)++(?:\n:\\h+.*$)++)++"),
										  QRegularExpression::MultilineOption);
	return regexMatchAndReplace(
		items, regex, "<dl>%1</dl>", [&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
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
					headerStyle += "margin-top:0;";
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
	static const QRegularExpression regex(
		QLatin1String("^.+\\|+.*\n\\|*\\h*:?-+(?::?\\h*\\|\\h*:?-*:?)++\\h*\\|*(?:\n.+)*"),
		QRegularExpression::MultilineOption);
	return regexMatchAndReplace(
		items, regex, "<table class=\"table\" cellspacing=\"0\">%1</table>",
		[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
			static const QRegularExpression regexUnescapedPipes(QLatin1String("(?<!\\\\)\\|"));
			QStringList rowsMarkdown = match.captured(0).split('\n');
			QStringList columnAlignments;
			QStringList tableRows;

			QStringList columnAlignmentsMarkdown =
				rowsMarkdown.takeAt(1).split(regexUnescapedPipes, Qt::SkipEmptyParts);
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
				QStringList columnsMarkdown = rowMarkdown.split(regexUnescapedPipes, Qt::SkipEmptyParts);
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
					QString &align         = columnAlignments[i];
					QString alignAttribute = !align.isEmpty() ? QLatin1String(" align=\"%1\"").arg(align) : "";
					QChar cellType         = isHeaderRow ? 'h' : 'd';
					processFormatting(column, items,
									  { processMarkdownEmphasisOrScript, processMarkdownInlineCode, processImageOrLink,
										processColor, processLinebreak, processHTMLFixedSpace, processHTMLComment });
					columns.append(QLatin1String("<t%3%2>%1</t%3>").arg(column, alignAttribute, cellType));
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
	const QChar &matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '>' && matchStartCharacter != ' ') {
		return false;
	}
	static const QRegularExpression regex(QLatin1String("^\\h{,3}>.*(?:\n.+)*\n?"),
										  QRegularExpression::MultilineOption);
	return regexMatchAndReplace(
		items, regex, "<table class=\"blockquote\" width=\"100%\"><tr><td>%1</td></tr></table>",
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
				quote += line + QLatin1String(i != lastLineIndex ? "\n" : "");
			}

			processFormatting(quote, items,
							  { processCode, processMarkdownThematicBreak, processMarkdownHeader, processList,
								processMarkdownEmphasisOrScript, processImageOrLink, processMarkdownBlockQuote,
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
	// Centered block in format -> centered <- where the spaces may be newlines
	const QChar &matchStartCharacter = items.inputStr[items.offset];
	if (matchStartCharacter != '-' && matchStartCharacter != ' ') {
		return false;
	}
	static const QRegularExpression regex(QLatin1String("-> ((?:.|\n)+) <-|^\\h{,3}->\n((?:.|\n)+)\n<-"),
										  QRegularExpression::MultilineOption);
	return regexMatchAndReplace(items, regex, "<center>%1</center>",
								[&items](const QRegularExpressionMatch &match, const char *contentWrapper) {
									QString content = findFirstMatchedGroup(match);
									processFormatting(content, items,
													  {
														  processCode,
														  processMarkdownThematicBreak,
														  processMarkdownHeader,
														  processList,
														  processMarkdownEmphasisOrScript,
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
	items.html.reserve(markdownInput.size());

	processFormatting(items, { processCode, processMarkdownThematicBreak, processMarkdownHeader, processList,
							   processMarkdownEmphasisOrScript, processImageOrLink, processMarkdownBlockQuote,
							   processMarkdownTable, processColor, processLinebreak, processHTMLFixedSpace,
							   processHTMLComment, processMarkdownCenteredBlock });
	processQueuedMarkdownReferences(items);
	processQueuedMarkdownImages(items);

	items.html.squeeze();
	return items.html;
}
} // namespace Markdown
