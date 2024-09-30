// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QBuffer>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QtCore>
#include <QtTest>

#include "XMLTools.h"

class TestXMLTools : public QObject {
	Q_OBJECT
public:
	TestXMLTools(QObject *parent = 0);
private slots:
	void testParseExtractBody();
	void testParseConvertStyleBold();
	void testParseConvertStyleUnderline();
	void testParseConvertStyleItalic();
	void testParseKeepTags();
	void testParseDropEmptyFirstParagraphs();
	void testUndupNoopB();
	void testUndupNoopU();
	void testUndupNoopI();
	void testUndupNoopA();
	void testUndupSimpleB();
	void testUndupSimpleU();
	void testUndupSimpleI();
	void testUndupSimpleA();
	void testUndupMixedBU();
	void testUndupMixedBUIA();
	void testUndupWithSpace();
	void testUndupAnchor();
	void testUndupLinkSameHref();
	void testUndupLinkDifferingHref();
	void testUndupNoRootUndupTag();
	void testUndupUndupTagsRemoval();
	void testUndupStandard();
	//		void testUndupInvalidXml1();
private:
	QString doParse(QString input);
	QString doUnduplicate(QString input);
};

TestXMLTools::TestXMLTools(QObject *parent) : QObject(parent) {
}

QString TestXMLTools::doParse(QString input) {
	QXmlStreamReader reader(input);
	QBuffer outBuf;
	QXmlStreamWriter writerOut(&outBuf);
	outBuf.open(QIODevice::WriteOnly);
	int paragraphs = 0;
	QMap< QString, QString > pstyle;
	XMLTools::recurseParse(reader, writerOut, paragraphs, pstyle);
	outBuf.close();
	return QString(outBuf.data());
}

void TestXMLTools::testParseExtractBody() {
	QCOMPARE(doParse(QLatin1String("<html><head></head><body>one</body></html>")), QLatin1String("one"));
	QCOMPARE(doParse(QLatin1String("<!DOCTYPE invalid><html><head></head><body>one</body></html>")),
			 QLatin1String("one"));
	QCOMPARE(doParse(QLatin1String("<html><head><title>ttt</title></head><body>one</body></html>")),
			 QLatin1String("one"));
	QCOMPARE(doParse(QLatin1String("<html><head></head><body><b>one</b></body></html>")), QLatin1String("<b>one</b>"));
}

void TestXMLTools::testParseConvertStyleBold() {
	// The numeric values for font-weight provide a more granular option to specify boldness.
	// On fonts with two boldness levels (normal and bold) this is fine (100-500, 600-900).
	// For fonts wiht more boldness levels, the conversion we do here from a weight value to a <b> tag is problematic.
	// http://doc.qt.io/qt-4.8/stylesheet-reference.html#list-of-property-types
	// https://developer.mozilla.org/en-US/docs/Web/CSS/font-weight
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-weight:100\">one</span></body></html>")),
		QLatin1String("<span style=\"font-weight:100\">one</span>"));
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-weight:100;\">one</span></body></html>")),
		QLatin1String("<span style=\"font-weight:100\">one</span>"));
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-weight:200;\">one</span></body></html>")),
		QLatin1String("<span style=\"font-weight:200\">one</span>"));
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-weight:300;\">one</span></body></html>")),
		QLatin1String("<span style=\"font-weight:300\">one</span>"));
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-weight:400;\">one</span></body></html>")),
		QLatin1String("<span style=\"font-weight:400\">one</span>"));
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-weight:500;\">one</span></body></html>")),
		QLatin1String("<span style=\"font-weight:500\">one</span>"));
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-weight:600\">one</span></body></html>")),
		QLatin1String("<b>one</b>"));
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-weight:600;\">one</span></body></html>")),
		QLatin1String("<b>one</b>"));
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-weight: 600;\">one</span></body></html>")),
		QLatin1String("<b>one</b>"));
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-weight: 600\">one</span></body></html>")),
		QLatin1String("<b>one</b>"));
	// These currently fail
	//	QCOMPARE(doParse(QLatin1String("<html><head></head><body><span
	// style=\"font-weight:700;\">one</span></body></html>")), QLatin1String("<b>one</b>"));
	//	QCOMPARE(doParse(QLatin1String("<html><head></head><body><span
	// style=\"font-weight:800;\">one</span></body></html>")), QLatin1String("<b>one</b>"));
	//	QCOMPARE(doParse(QLatin1String("<html><head></head><body><span
	// style=\"font-weight:900;\">one</span></body></html>")), QLatin1String("<b>one</b>"));
	//	QCOMPARE(doParse(QLatin1String("<html><head></head><body><span
	// style=\"font-weight:bold;\">one</span></body></html>")), QLatin1String("<b>one</b>"));
}

void TestXMLTools::testParseConvertStyleUnderline() {
	QCOMPARE(doParse(QLatin1String(
				 "<html><head></head><body><span style=\"text-decoration:underline\">one</span></body></html>")),
			 QLatin1String("<u>one</u>"));
	// We only convert spans, so this will not create a u tag
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><b style=\"text-decoration:underline\">one</b></body></html>")),
		QLatin1String("<b style=\"text-decoration:underline\">one</b>"));
}

void TestXMLTools::testParseConvertStyleItalic() {
	QCOMPARE(
		doParse(QLatin1String("<html><head></head><body><span style=\"font-style:italic\">one</span></body></html>")),
		QLatin1String("<i>one</i>"));
}

void TestXMLTools::testParseKeepTags() {
	QCOMPARE(doParse(QLatin1String("<html><head></head><body><b>one</b><i>two</i><u>three</u></body></html>")),
			 QLatin1String("<b>one</b><i>two</i><u>three</u>"));
	QCOMPARE(doParse(QLatin1String("<html><head></head><body><i><b><u>one</u></b></i></body></html>")),
			 QLatin1String("<i><b><u>one</u></b></i>"));
}

void TestXMLTools::testParseDropEmptyFirstParagraphs() {
	QCOMPARE(doParse(QLatin1String("<html><head></head><body><p></p><p>one</p></body></html>")),
			 QLatin1String("<p>one</p>"));
	QCOMPARE(doParse(QLatin1String("<html><head></head><body><p></p><p>one</p><p></p></body></html>")),
			 QLatin1String("<p>one</p><p/>"));
	QCOMPARE(doParse(QLatin1String("<html><head></head><body><p></p><p></p><p>one</p><p></p></body></html>")),
			 QLatin1String("<p/><p>one</p><p/>"));
	QCOMPARE(doParse(QLatin1String("<html><head></head><body><p></p><div>one</div></body></html>")),
			 QLatin1String("<div>one</div>"));
	// <br> would be invalid XML. It is not dropped "correctly".
	//	QCOMPARE(doParse(QLatin1String("<html><head></head><body><p><br></p><p>one</p></body></html>")),
	// QLatin1String("<p>one</p>"));
	// This is different from what is expected from a comment in the method being tested. It says
	// contents of empty paragraph would be dropped as well, but the br/space persists.
	QCOMPARE(doParse(QLatin1String("<html><head></head><body><p><br/></p><p>one</p></body></html>")),
			 QLatin1String("<br/><p>one</p>"));
	QCOMPARE(doParse(QLatin1String("<html><head></head><body><p><br /></p><p>one</p></body></html>")),
			 QLatin1String("<br/><p>one</p>"));
	QCOMPARE(doParse(QLatin1String("<html><head></head><body><p> </p><p>one</p></body></html>")),
			 QLatin1String(" <p>one</p>"));
}

QString TestXMLTools::doUnduplicate(QString input) {
	QXmlStreamReader reader(input);
	QBuffer outBuf;
	QXmlStreamWriter writerOut(&outBuf);
	outBuf.open(QIODevice::WriteOnly);
	XMLTools::unduplicateTags(reader, writerOut);
	outBuf.close();
	return QString(outBuf.data());
}

void TestXMLTools::testUndupNoopB() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><b>test</b></unduplicate>")), QLatin1String("<b>test</b>"));
}

void TestXMLTools::testUndupNoopU() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><u>test</u></unduplicate>")), QLatin1String("<u>test</u>"));
}

void TestXMLTools::testUndupNoopI() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><i>test</i></unduplicate>")), QLatin1String("<i>test</i>"));
}

void TestXMLTools::testUndupNoopA() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><a>test</a></unduplicate>")), QLatin1String("<a>test</a>"));
}

void TestXMLTools::testUndupSimpleB() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><b>one</b><b>two</b></unduplicate>")),
			 QLatin1String("<b>onetwo</b>"));
}

void TestXMLTools::testUndupSimpleU() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><u>one</u><u>two</u></unduplicate>")),
			 QLatin1String("<u>onetwo</u>"));
}

void TestXMLTools::testUndupSimpleI() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><i>one</i><i>two</i></unduplicate>")),
			 QLatin1String("<i>onetwo</i>"));
}

void TestXMLTools::testUndupSimpleA() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><a>one</a><a>two</a></unduplicate>")),
			 QLatin1String("<a>onetwo</a>"));
}

void TestXMLTools::testUndupWithSpace() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><b>one</b><b> two</b></unduplicate>")),
			 QLatin1String("<b>one two</b>"));
}

void TestXMLTools::testUndupMixedBU() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><b>one</b><u>two</u></unduplicate>")),
			 QLatin1String("<b>one</b><u>two</u>"));
}

void TestXMLTools::testUndupMixedBUIA() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><a><i><b>one</b><u>two</u></i></a></unduplicate>")),
			 QLatin1String("<a><i><b>one</b><u>two</u></i></a>"));
}

void TestXMLTools::testUndupAnchor() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><a>one</a><a>two</a></unduplicate>")),
			 QLatin1String("<a>onetwo</a>"));
}

void TestXMLTools::testUndupLinkSameHref() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><a href=\"#\">one</a><a href=\"#\">two</a></unduplicate>")),
			 QLatin1String("<a href=\"#\">onetwo</a>"));
	QCOMPARE(
		doUnduplicate(QLatin1String(
			"<unduplicate><a href=\"http://example.org\">one</a><a href=\"http://example.org\">two</a></unduplicate>")),
		QLatin1String("<a href=\"http://example.org\">onetwo</a>"));
}

void TestXMLTools::testUndupLinkDifferingHref() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><a href=\"#\">one</a><a>two</a></unduplicate>")),
			 QLatin1String("<a href=\"#\">one</a><a>two</a>"));
	QCOMPARE(
		doUnduplicate(QLatin1String("<unduplicate><a href=\"http://example.org\">one</a><a>two</a></unduplicate>")),
		QLatin1String("<a href=\"http://example.org\">one</a><a>two</a>"));
	QCOMPARE(
		doUnduplicate(QLatin1String("<unduplicate><a>one</a><a href=\"http://example.org\">two</a></unduplicate>")),
		QLatin1String("<a>one</a><a href=\"http://example.org\">two</a>"));
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><a href=\"http://example.org/path1\">one</a><a "
										 "href=\"http://example.org/path2\">two</a></unduplicate>")),
			 QLatin1String("<a href=\"http://example.org/path1\">one</a><a href=\"http://example.org/path2\">two</a>"));
}

void TestXMLTools::testUndupNoRootUndupTag() {
	QCOMPARE(doUnduplicate(QLatin1String("<u><b>one</b><b>two</b></u>")), QLatin1String("<u><b>onetwo</b></u>"));
	QCOMPARE(doUnduplicate(QLatin1String("<i><b>one</b><b>two</b></i>")), QLatin1String("<i><b>onetwo</b></i>"));
}

void TestXMLTools::testUndupUndupTagsRemoval() {
	QCOMPARE(
		doUnduplicate(QLatin1String(
			"<unduplicate><b>one</b><unduplicate><unduplicate></unduplicate><b>two</b></unduplicate></unduplicate>")),
		QLatin1String("<b>onetwo</b>"));
}

void TestXMLTools::testUndupStandard() {
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><b>bold with </b><b><i>italic</i></b></unduplicate>")),
			 QLatin1String("<b>bold with <i>italic</i></b>"));
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><b>bold with </b><b><i></i><i>italic</i></b></unduplicate>")),
			 QLatin1String("<b>bold with <i>italic</i></b>"));
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><b>test</b></unduplicate>")), QLatin1String("<b>test</b>"));
	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><b>bold with </b><b><i></i><i>italic</i></b></unduplicate>")),
			 QLatin1String("<b>bold with <i>italic</i></b>"));
}

// Will warn and fail.
// void TestXMLTools::testUndupInvalidXml1() {
//	QCOMPARE(doUnduplicate(QLatin1String("<unduplicate><b>bold with <i></b>italic</i></unduplicate>")),
// QLatin1String("<b>bold with <i>italic</i></b>"));
//}

QTEST_MAIN(TestXMLTools)
#include "TestXMLTools.moc"
