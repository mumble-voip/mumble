#include <QTemporaryDir>
#include <QtTest>

#include "Channel.h"
#include "Meta.h"
#include "SSL.h"
#include "Server.h"
#include "database/SQLiteConnectionParameter.h"

class TestNestedTemporaryChannels : public QObject {
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();
	void testPreserveParentWhileChildExists();
	void testCascadePruningWhenChildBecomesEmpty();
	void testDeepNestingCascade();
	void testBranchPreservation();

private:
	QTemporaryDir m_tempDir;
	std::unique_ptr< mumble::db::SQLiteConnectionParameter > m_dbParam;
	Server *m_server = nullptr;
	Channel *m_root  = nullptr;
};

void TestNestedTemporaryChannels::initTestCase() {
	QVERIFY(m_tempDir.isValid());

	// 1. Initialize OpenSSL subsystem
	MumbleSSL::initialize();

	// 2. Initialize global MetaParams without binding network ports
	Meta::mp = std::make_unique< MetaParams >();
	Meta::mp->qlBind.clear();
	Meta::mp->bBonjour      = false;
	Meta::mp->kdfIterations = 1000;

	// 3. Initialize isolated SQLite database in temporary directory
	m_dbParam = std::make_unique< mumble::db::SQLiteConnectionParameter >(
		m_tempDir.filePath(QStringLiteral("test_murmur.sqlite")).toStdString());

	// 4. Initialize global Meta instance (sets up DB schema)
	meta = new Meta(*m_dbParam);

	// 5. Boot default server instance
	meta->bootAll(*m_dbParam, true);

	QVERIFY(!meta->qhServers.isEmpty());
	m_server = meta->qhServers.values().first();
	QVERIFY(m_server != nullptr);

	if (m_server->qhChannels.contains(0)) {
		m_root = m_server->qhChannels.value(0);
	} else {
		m_root             = new Channel(0, QStringLiteral("Root"), nullptr);
		m_root->bTemporary = false;
		m_server->qhChannels.insert(0, m_root);
	}
	QVERIFY(m_root != nullptr);
}

void TestNestedTemporaryChannels::cleanupTestCase() {
	delete meta;
	meta     = nullptr;
	m_server = nullptr;
	Meta::mp.reset();
}

void TestNestedTemporaryChannels::testPreserveParentWhileChildExists() {
	Channel *parentTemp = m_server->createNewChannel(m_root, QStringLiteral("TempParent"), true);
	QVERIFY(parentTemp != nullptr);
	QVERIFY(parentTemp->bTemporary);
	QVERIFY(m_server->qhChannels.contains(parentTemp->iId));

	Channel *childTemp = m_server->createNewChannel(parentTemp, QStringLiteral("TempChild"), true);
	QVERIFY(childTemp != nullptr);
	QVERIFY(childTemp->bTemporary);
	QVERIFY(parentTemp->qlChannels.contains(childTemp));

	// Parent has 0 users, but has an active child channel.
	// Pruning the parent MUST NOT remove it!
	m_server->removeTemporaryChannelIfEmpty(parentTemp->iId);
	QVERIFY(m_server->qhChannels.contains(parentTemp->iId));
	QVERIFY(m_server->qhChannels.contains(childTemp->iId));

	const unsigned int parentId = parentTemp->iId;
	const unsigned int childId  = childTemp->iId;

	// Clean up child, which cascades to clean up parent
	m_server->removeTemporaryChannelIfEmpty(childId);
	QVERIFY(!m_server->qhChannels.contains(childId));
	QVERIFY(!m_server->qhChannels.contains(parentId));
}

void TestNestedTemporaryChannels::testCascadePruningWhenChildBecomesEmpty() {
	Channel *parentTemp = m_server->createNewChannel(m_root, QStringLiteral("CascadeParent"), true);
	QVERIFY(parentTemp != nullptr);
	unsigned int parentId = parentTemp->iId;

	Channel *childTemp = m_server->createNewChannel(parentTemp, QStringLiteral("CascadeChild"), true);
	QVERIFY(childTemp != nullptr);
	unsigned int childId = childTemp->iId;

	// Both have 0 users. Triggering prune on the child cascades bottom-up:
	m_server->removeTemporaryChannelIfEmpty(childId);

	QVERIFY(!m_server->qhChannels.contains(childId));
	QVERIFY(!m_server->qhChannels.contains(parentId));
	QVERIFY(m_server->qhChannels.contains(m_root->iId));
}

void TestNestedTemporaryChannels::testDeepNestingCascade() {
	// Hierarchy: Root -> T1 -> T2 -> T3 (all temporary)
	Channel *t1 = m_server->createNewChannel(m_root, QStringLiteral("T1"), true);
	Channel *t2 = m_server->createNewChannel(t1, QStringLiteral("T2"), true);
	Channel *t3 = m_server->createNewChannel(t2, QStringLiteral("T3"), true);

	unsigned int id1 = t1->iId;
	unsigned int id2 = t2->iId;
	unsigned int id3 = t3->iId;

	// Pruning middle channel must not delete it while child T3 exists
	m_server->removeTemporaryChannelIfEmpty(id2);
	QVERIFY(m_server->qhChannels.contains(id1));
	QVERIFY(m_server->qhChannels.contains(id2));
	QVERIFY(m_server->qhChannels.contains(id3));

	// Pruning leaf T3 cascades all the way up to T1
	m_server->removeTemporaryChannelIfEmpty(id3);
	QVERIFY(!m_server->qhChannels.contains(id3));
	QVERIFY(!m_server->qhChannels.contains(id2));
	QVERIFY(!m_server->qhChannels.contains(id1));
	QVERIFY(m_server->qhChannels.contains(m_root->iId));
}

void TestNestedTemporaryChannels::testBranchPreservation() {
	// Hierarchy: Root -> Parent -> (ChildA, ChildB)
	Channel *parent = m_server->createNewChannel(m_root, QStringLiteral("BranchParent"), true);
	Channel *childA = m_server->createNewChannel(parent, QStringLiteral("ChildA"), true);
	Channel *childB = m_server->createNewChannel(parent, QStringLiteral("ChildB"), true);

	unsigned int pId = parent->iId;
	unsigned int aId = childA->iId;
	unsigned int bId = childB->iId;

	// Pruning ChildA removes only ChildA, because ChildB is still under Parent
	m_server->removeTemporaryChannelIfEmpty(aId);
	QVERIFY(!m_server->qhChannels.contains(aId));
	QVERIFY(m_server->qhChannels.contains(bId));
	QVERIFY(m_server->qhChannels.contains(pId));

	// Pruning ChildB removes ChildB and now cascades to remove Parent
	m_server->removeTemporaryChannelIfEmpty(bId);
	QVERIFY(!m_server->qhChannels.contains(bId));
	QVERIFY(!m_server->qhChannels.contains(pId));
	QVERIFY(m_server->qhChannels.contains(m_root->iId));
}

QTEST_MAIN(TestNestedTemporaryChannels)
#include "TestNestedTemporaryChannels.moc"
