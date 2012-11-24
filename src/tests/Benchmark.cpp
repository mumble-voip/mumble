#include <QtCore>
#include <QtNetwork>

#ifndef Q_OS_WIN
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#ifndef Q_OS_WIN
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/utsname.h>
#endif
#include <errno.h>

#include "PacketDataStream.h"
#include "Timer.h"
#include "Message.h"
#include "CryptState.h"
#include "Mumble.pb.h"

class Client : public QThread {
		Q_OBJECT
	public:
		bool udp;
		bool sender;
		struct sockaddr_in srv;
		unsigned int uiSession;
		CryptState crypt;
		int rcvd;
		int socket;
		int seq;
		void run();
		void ping();
		void sendVoice();
		int numbytes;
		int ptype;
		QSslSocket *ssl;
		Client(QObject *parent, QHostAddress srvaddr, unsigned short prt, bool send, bool tcponly);
		void doUdp(const unsigned char *buffer, int size);
		void sendMessage(const ::google::protobuf::Message &msg, unsigned int msgType);
		~Client();
	public slots:
		void readyRead();
		void disconnected();
};

Client::Client(QObject *p, QHostAddress qha, unsigned short prt, bool send, bool tcponly) : QThread(p) {
	srv.sin_family = AF_INET;
	srv.sin_addr.s_addr = htonl(qha.toIPv4Address());
	srv.sin_port = htons(prt);

	udp = ! tcponly;
	sender = send;

	ssl = new QSslSocket(this);

	connect(ssl, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(ssl, SIGNAL(disconnected()), this, SLOT(disconnected()));

	ssl->setProtocol(QSsl::TlsV1);
	ssl->connectToHostEncrypted(qha.toString(), prt);
	ssl->ignoreSslErrors();
	if (! ssl->waitForEncrypted())
		qFatal("Connection failure");

	static int ctr = 1;

#ifdef Q_OS_WIN
	DWORD pid = GetCurrentProcessId();
	wchar_t buf[64];
	DWORD bufsize = 64;
	GetComputerName(buf, &bufsize);

	QString name = QString("%1.%2").arg(QString::fromWCharArray(buf)).arg(pid * 1000 + ctr);
#else
	struct utsname uts;
	uname(&uts);

	QString name = QString("%1.%2").arg(uts.nodename).arg(getpid() * 1000 + ctr);
#endif

	ctr++;

	MumbleProto::Version mpv;
	mpv.set_release(u8(QLatin1String("1.2.1 Benchmark")));
	mpv.set_version(0x010200);

	sendMessage(mpv, MessageHandler::Version);

	MumbleProto::Authenticate mpa;
	mpa.set_username(u8(name));

	sendMessage(mpa, MessageHandler::Authenticate);

	if (udp)
		socket = ::socket(PF_INET, SOCK_DGRAM, 0);

	seq = 0;
	rcvd = 0;
	numbytes = -1;
}

Client::~Client() {
	terminate();
	wait();
}

void Client::sendMessage(const ::google::protobuf::Message &msg, unsigned int msgType) {
	unsigned char uc[4096];
	int len = msg.ByteSize();
	Q_ASSERT(len < 4090);

	* reinterpret_cast<quint16 *>(& uc[0]) = qToBigEndian(static_cast<quint16>(msgType));
	* reinterpret_cast<quint32 *>(& uc[2]) = qToBigEndian(static_cast<quint32>(len));

	msg.SerializeToArray(uc + 6, len);

	ssl->write(reinterpret_cast<const char *>(uc), len+6);
}

void Client::ping() {
	unsigned char buffer[256];
	buffer[0] = MessageHandler::UDPPing << 5;
	PacketDataStream pds(buffer + 1, 255);
	pds << 123;

	doUdp(buffer, pds.size() + 1);

	MumbleProto::Ping mpp;
	mpp.set_timestamp(123);
	sendMessage(mpp, MessageHandler::Ping);
}

void Client::sendVoice() {
	unsigned char buffer[1024];
	int len = 32 + (qrand() & 0x3f);

	// Regular voice, nothing special
	buffer[0] = 0;

	PacketDataStream ods(buffer + 1, 1024);
	ods << 1;
	ods.append(len);
	ods.skip(len);

	doUdp(buffer, ods.size() + 1);
}

void Client::doUdp(const unsigned char *buffer, int size) {
	if (! udp || ! crypt.isValid())
		return;

	unsigned char crypted[2048];

	crypt.encrypt(reinterpret_cast<const unsigned char *>(buffer), crypted, size);
	::sendto(socket, reinterpret_cast<const char *>(crypted), size+4, 0, reinterpret_cast<struct sockaddr *>(&srv), sizeof(srv));
}


void Client::run() {
	unsigned char buffer[1024];
	struct sockaddr_in addr;
	socklen_t sz;
	int len;

	if (! udp)
		return;

	forever {
		sz = sizeof(addr);
		len = ::recvfrom(socket, reinterpret_cast<char *>(buffer), 1024, 0, reinterpret_cast<struct sockaddr *>(&addr), &sz);
		if (len <= 0)
			break;
		if (len >= 32)
			rcvd++;
	}
}

void Client::readyRead() {
	forever {
		int avail = ssl->bytesAvailable();
		if (numbytes == -1) {
			if (avail < 6)
				break;
			unsigned char b[6];
			ssl->read(reinterpret_cast<char *>(b), 6);

			ptype = qFromBigEndian(* reinterpret_cast<quint16 *>(& b[0]));
			numbytes = qFromBigEndian(* reinterpret_cast<quint32 *>(& b[2]));

			avail = ssl->bytesAvailable();
		}
		if ((numbytes >= 0) && (avail >= numbytes)) {
			int want = numbytes;
			numbytes = -1;
			unsigned char buff[65536];
			Q_ASSERT(want < 65536);
			ssl->read(reinterpret_cast<char *>(buff), want);


			avail = ssl->bytesAvailable();

			switch (ptype) {
				case MessageHandler::CryptSetup: {
						MumbleProto::CryptSetup msg;
						if (! msg.ParseFromArray(buff, want))
							qFatal("Failed parse crypt");

						if (msg.has_key() && msg.has_client_nonce() && msg.has_server_nonce()) {
							const std::string &key = msg.key();
							const std::string &client_nonce = msg.client_nonce();
							const std::string &server_nonce = msg.server_nonce();
							if (key.size() == AES_BLOCK_SIZE && client_nonce.size() == AES_BLOCK_SIZE && server_nonce.size() == AES_BLOCK_SIZE)
								crypt.setKey(reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(client_nonce.data()), reinterpret_cast<const unsigned char *>(server_nonce.data()));
						} else if (msg.has_server_nonce()) {
							const std::string &server_nonce = msg.server_nonce();
							if (server_nonce.size() == AES_BLOCK_SIZE) {
								crypt.uiResync++;
								memcpy(crypt.decrypt_iv, server_nonce.data(), AES_BLOCK_SIZE);
							}
						} else {
							MumbleProto::CryptSetup mpcs;
							mpcs.set_client_nonce(std::string(reinterpret_cast<const char *>(crypt.encrypt_iv), AES_BLOCK_SIZE));
							sendMessage(mpcs, MessageHandler::CryptSetup);
						}
						break;
					}
				case MessageHandler::ServerSync: {
						MumbleProto::ServerSync msg;
						if (! msg.ParseFromArray(buff, want))
							qFatal("Failed parse sync");
						uiSession = msg.session();
						break;
					}
				case MessageHandler::UDPTunnel: {
						unsigned int msgUDPType = (buff[0] >> 5) & 0x7;
						if (msgUDPType == MessageHandler::UDPVoiceCELTAlpha)
							rcvd++;
						break;
					}
			}
		} else {
			break;
		}
	}
}

void Client::disconnected() {
	qWarning("SSL Socket disconnected");
	QCoreApplication::instance()->quit();
}

class Container : public QObject {
		Q_OBJECT
	public:
		int sent;
		int nsend, nudp, ntcp;
		int isend, iudp, itcp;
		bool live, forceping;
		QHostAddress qha;
		unsigned short port;
		QTimer qtTick;
		Timer tickPing, tickVoice, tickGo, tickSpawn;
		QList<Client *> speakers;
		QList<Client *> clients;
		Container(QHostAddress srvaddr, unsigned short port, int nsend, int nudp, int ntcp);
	public slots:
		void tick();
		void go();
};

Container::Container(QHostAddress qha, unsigned short port, int numsend, int numudp, int numtcp) {
	Timer t;

	qWarning("Spawning %d speakers and %d listeners (%d UDP, %d TCP)", numsend, numudp+numtcp, numudp, numtcp);

	nsend=numsend;
	nudp=numudp;
	ntcp=numtcp;

	isend=iudp=itcp=0;

	this->qha = qha;
	this->port = port;

	connect(&qtTick, SIGNAL(timeout()), this, SLOT(tick()));
	qtTick.start(0);

	live = false;
	forceping = false;

	tickSpawn.restart();
	sent = 0;

}

void Container::tick() {
	if (forceping || tickPing.isElapsed(5000000ULL)) {
		forceping = false;

		foreach(Client *c, clients)
			c->ping();

		if (live) {

			int lost = 0;
			quint64 totrcv = 0;
			int nrcv = 0;
			foreach(Client *c, clients) {
				if (! c->sender) {
					totrcv += c->rcvd;
					lost += sent - c->rcvd;
					nrcv++;
				}
			}
			qWarning("Sent: %8d  Rcvd: %8lld  Lost: %8d   BW: %6.1fMbit/s", sent, totrcv / nrcv, (lost + nrcv - 1) / nrcv, (totrcv * 8.0 * 123.0) / (tickGo.elapsed() * 1.0));
		} else {
			qWarning("Spawned %3d/%3d", isend+iudp+itcp,nsend+nudp+ntcp);
		}
	}

	if (live && tickVoice.isElapsed(10000ULL)) {
		foreach(Client *c, speakers) {
			sent++;
			c->sendVoice();
		}
	}
	if (! live) {
		if (isend < nsend) {
			Client *c = new Client(this, qha, port, true, false);
			speakers << c;
			c->start();
			clients << c;
			isend++;
		} else if (iudp < nudp) {
			Client *c = new Client(this, qha, port, false, false);
			c->start();
			clients << c;
			iudp++;
		} else if (itcp < ntcp) {
			Client *c = new Client(this, qha, port, false, true);
			c->start();
			clients << c;
			itcp++;
		} else {
			live = true;
			quint64 elapsed = tickSpawn.elapsed();
			qWarning("Spawning took %lld ms (%lld us per client)", elapsed / 1000ULL, elapsed / (nsend+nudp+ntcp));
			foreach(Client *c, clients)
				c->rcvd = 0;
			sent = 0;
			forceping = true;
			qtTick.start(10);
		}
	}
}

void Container::go() {
	foreach(Client *c, clients)
		c->start();
	qtTick.start(10);
	tickGo.restart();
}

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);

	qWarning("Maximum # sockets is %d", FD_SETSIZE);

	if (argc != 6)
		qFatal("Invalid # args");

	QHostAddress qha = QHostAddress(argv[1]);
	int port = atoi(argv[2]);
	int numsend = atoi(argv[3]);
	int numudp = atoi(argv[4]);
	int numtcp = atoi(argv[5]);

	Container c(qha, port, numsend, numudp, numtcp);
	c.go();
	a.exec();
}

#include "Benchmark.moc"
