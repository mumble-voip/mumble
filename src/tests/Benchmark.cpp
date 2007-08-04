#include <QtCore>
#include <QtNetwork>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/utsname.h>
#include <errno.h>

#include "PacketDataStream.h"
#include "Timer.h"
#include "Message.h"

class Client : public QThread {
		Q_OBJECT
	public:
		bool udp;
		bool sender;
		struct sockaddr_in srv;
		unsigned int uiSession;
		int rcvd;
		int socket;
		int seq;
		void run();
		void ping();
		void sendVoice();
		int numbytes;
		QSslSocket *ssl;
		Client(QObject *parent, QHostAddress srvaddr, unsigned short prt, bool send, bool tcponly);
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

	ssl->connectToHostEncrypted(qha.toString(), prt);
	ssl->ignoreSslErrors();
	if (! ssl->waitForEncrypted())
		qFatal("Connection failure");

	struct utsname uts;
	uname(&uts);
	
	static int ctr = 1;

	QString name = QString("%1.%2").arg(uts.nodename).arg(getpid() * 1000 + ctr);
	
	ctr++;

	unsigned char buffer[65535];
	PacketDataStream ods(buffer + 3, 5000);

	ods << Message::ServerAuthenticate;
	ods << 0;
	ods << MESSAGE_STREAM_VERSION;
	ods << 100;
	ods << name;
	ods << QString();

	int msize = ods.size();

	buffer[0] = (msize >> 16) & 0xFF;
	buffer[1] = (msize >> 8) & 0xFF;
	buffer[2] = msize & 0xFF;

	ssl->write(reinterpret_cast<const char *>(buffer), 3 + msize);

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

void Client::ping() {
	unsigned char buffer[65535];
	unsigned char *bp = buffer + 3;
	PacketDataStream ods(bp, 5000);
	ods << Message::Ping;
	ods << uiSession;
	ods << 123;

	if (udp)
		::sendto(socket, bp, ods.size(), 0, reinterpret_cast<struct sockaddr *>(&srv), sizeof(srv));

	int msize = ods.size();

	buffer[0] = (msize >> 16) & 0xFF;
	buffer[1] = (msize >> 8) & 0xFF;
	buffer[2] = msize & 0xFF;

	ssl->write(reinterpret_cast<const char *>(buffer), 3 + msize);
}

void Client::sendVoice() {
	unsigned char buffer[65535];
	PacketDataStream ods(buffer, 65535);

	char spx[100];
	spx[0] = MessageSpeex::AltSpeak;
	for (int i=1;i<100;i++)
		spx[i]=i;

	ods << Message::Speex;
	ods << uiSession;
	ods << seq++;
	ods.append(spx, 100);
	::sendto(socket, buffer, ods.size(), 0, reinterpret_cast<struct sockaddr *>(&srv), sizeof(srv));
}

void Client::run() {
	unsigned char buffer[1000];
	struct sockaddr_in addr;
	socklen_t sz;

	if (! udp)
		return;

	forever {
		sz = sizeof(addr);
		if (::recvfrom(socket, reinterpret_cast<char *>(buffer), 1000, 0, reinterpret_cast<struct sockaddr *>(&addr), &sz) <= 0)
			break;
		if (buffer[0] == Message::Speex)
			rcvd++;
	}
}

void Client::readyRead() {
	forever {
		int avail = ssl->bytesAvailable();
		if (numbytes == -1) {
			if (avail < 3)
				break;
			unsigned char b[3];
			ssl->read(reinterpret_cast<char *>(b), 3);
			numbytes = (b[0] << 16) + (b[1] << 8) + b[2];
			avail = ssl->bytesAvailable();
		}
		if ((numbytes >= 0) && (avail >= numbytes)) {
			int want = numbytes;
			numbytes = -1;
			unsigned char buff[10000];
			ssl->read(reinterpret_cast<char *>(buff), want);
			avail = ssl->bytesAvailable();

			PacketDataStream ids(buff, want);
			unsigned int ptype;
			unsigned int sess;
			ids >> ptype;
			ids >> sess;
			if (ptype == Message::ServerSync) {
				uiSession = sess;
			} else if (ptype == Message::Speex) {
				rcvd++;
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
			qWarning("Sent: %8d  Rcvd: %8lld  Lost: %8d   BW: %6.1fMbit/s", sent, totrcv / nrcv, lost, (totrcv * 8.0 * 123.0) / (tickGo.elapsed() * 1.0));
		} else {
			qWarning("Spawned %3d/%3d", isend+iudp+itcp,nsend+nudp+ntcp);
		}
	}

	if (live && tickVoice.isElapsed(20000ULL)) {
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
