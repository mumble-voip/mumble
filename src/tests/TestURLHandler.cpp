
#include <QObject>
#include <QtTest>
#include <memory>

#include "URLHandler.h"

class TestURLHandler : public QObject {
	Q_OBJECT
private:
	class ChannelBuilder {
	private:
		std::shared_ptr<Channel> root;
		Channel* current;
	public:
		ChannelBuilder& operator()(QString name) {
			if (!root) {
				root.reset(new Channel(0, name));
				current = root.get();
			} else {
				current = new Channel(current->iPosition, name, current);
			}
			return *this;
		}
		Channel* ret() {
			return current;
		}
	};

private slots:
	void rootOnly1() {
		ChannelBuilder b;
		b("r");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/"));
	}
	void rootOnly2() {
		ChannelBuilder b;
		b("%");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/"));
	}
	void rootOnlyEmpty() {
		ChannelBuilder b;
		b("");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/"));
	}
	void pathOneSimple1() {
		ChannelBuilder b;
		b("")("a");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/a"));
	}
	void pathOneSimple1Uppercase() {
		ChannelBuilder b;
		b("")("A");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/A"));
	}
	void pathOneSimple2() {
		ChannelBuilder b;
		b("")("fewdSAfvcs");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/fewdSAfvcs"));
	}
	void pathOneSimple3() {
		ChannelBuilder b;
		b("")("r78365t65743287w4z7rf687s6df657865678das8");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/r78365t65743287w4z7rf687s6df657865678das8"));
	}
	void pathOneSpecial1() {
		ChannelBuilder b;
		b("")("ß");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/%C3%9F"));
	}
	void pathOneSpecial2() {
		ChannelBuilder b;
		b("")("öäÖÄüÜ");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/%C3%B6%C3%A4%C3%96%C3%84%C3%BC%C3%9C"));
	}
	void pathOneEncodedPercent1() {
		ChannelBuilder b;
		b("")("%");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/%25"));
	}
	void pathOneEncodedPercent2() {
		ChannelBuilder b;
		b("")("%25");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/%2525"));
	}
	void pathOneEncodedSlash() {
		ChannelBuilder b;
		b("")("/");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/%2F"));
	}
	void pathOneEncodedSpace() {
		ChannelBuilder b;
		b("")(" ");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/%20"));
	}
	void pathTwo1() {
		ChannelBuilder b;
		b("")(" ")(" ");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/%20/%20"));
	}
	void pathTwo2() {
		ChannelBuilder b;
		b("")("syyx123123123")("hgbre");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/syyx123123123/hgbre"));
	}
	void pathTwo3() {
		ChannelBuilder b;
		b("")(" ")("hgbre");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/%20/hgbre"));
	}
	void pathSeven() {
		ChannelBuilder b;
		b("")("., &%/da-_s76FASD~")("hgbre")("as")("vb")("0")("")("888");
		QCOMPARE(ChannelUrlPathHandler::getUrlPathForChannel(b.ret()), QString("/.%2C%20%26%25%2Fda-_s76FASD~/hgbre/as/vb/0//888"));
	}
};

QTEST_MAIN(TestURLHandler)
#include "TestURLHandler.moc"
