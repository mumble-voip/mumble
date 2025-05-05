// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/**
 * Protobuf binding benchmarks (serialisation, deserialization, ...).
 */

#include "Message.h"
#include "PacketDataStream.h"
#include "ProtoBuf.pb.h"
#include "Timer.h"
#include <QtCore>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);
	Timer t;

	unsigned char buff[1024];

	Authenticate apb;

	QString n = QLatin1String("My Name");
	QString p = QLatin1String("My Password");

	int v;

	t.restart();
	for (int i = 0; i < 1000000; ++i) {
		MessageServerAuthenticate ma;
		Message *m    = &ma;
		ma.uiSession  = 1;
		ma.iVersion   = -1;
		ma.qsUsername = n;
		ma.qsPassword = p;

		PacketDataStream out(buff, 256);
		m->messageToNetwork(out);

		PacketDataStream in(buff, out.size());
		delete m->networkToMessage(in);

		v = out.size();
	}
	printf("MsgBased : %8lld %d\n", t.elapsed(), v);

	t.restart();
	for (int i = 0; i < 1000000; ++i) {
		Authenticate apb;
		apb.set_version(-1);
		apb.set_session(1);
		apb.set_name(n.toStdString());
		apb.set_pw(p.toStdString());

		std::string str;

		Universal u;
		*(u.mutable_authenticate()) = apb;

		u.SerializeToString(&str);
		u.ParseFromString(str);

		const Authenticate &a = u.authenticate();

		QString qn = QString::fromStdString(a.name());
		QString qp = QString::fromStdString(a.pw());


		v = str.length();
	}
	printf("ProtoBuf : %8lld %d\n", t.elapsed(), v);

	t.restart();
	for (int i = 0; i < 1000000; ++i) {
		Universal u;
		Authenticate *apb = u.mutable_authenticate();
		apb->set_version(-1);
		apb->set_session(1);
		apb->set_name(n.toStdString());
		apb->set_pw(p.toStdString());

		std::string str;

		u.SerializeToString(&str);
		u.ParseFromString(str);

		const Authenticate &a = u.authenticate();

		QString qn = QString::fromStdString(a.name());
		QString qp = QString::fromStdString(a.pw());

		v = str.length();
	}
	printf("PBDirect : %8lld %d\n", t.elapsed(), v);

	std::string sn = n.toStdString();
	std::string sp = p.toStdString();

	t.restart();
	for (int i = 0; i < 1000000; ++i) {
		Universal u;
		Authenticate *apb = u.mutable_authenticate();
		apb->set_version(-1);
		apb->set_session(1);
		apb->set_name(sn);
		apb->set_pw(sp);

		std::string str;

		u.SerializeToString(&str);
		u.ParseFromString(str);

		const Authenticate &a = u.authenticate();

		std::string nn = a.name();
		std::string np = a.pw();

		v = str.length();
	}
	printf("PBStdStr : %8lld %d\n", t.elapsed(), v);

	t.restart();
	for (int i = 0; i < 1000000; ++i) {
		Universal u;
		Authenticate *apb = u.mutable_authenticate();
		apb->set_version(-1);
		apb->set_session(1);
		apb->set_name(n.toStdString());
		apb->set_pw(p.toStdString());

		std::string str;

		v = u.ByteSize();

		u.SerializeToArray(buff, v);
		u.ParseFromArray(buff, v);

		const Authenticate &a = u.authenticate();

		QString qn = QString::fromStdString(a.name());
		QString qp = QString::fromStdString(a.pw());
	}
	printf("PBArray  : %8lld %d\n", t.elapsed(), v);

	t.restart();
	for (int i = 0; i < 1000000; ++i) {
		Universal u;
		Authenticate *apb = u.MutableExtension(extauth);
		apb->set_version(-1);
		apb->set_session(1);
		apb->set_name(n.toStdString());
		apb->set_pw(p.toStdString());

		std::string str;

		v = u.ByteSize();

		u.SerializeToArray(buff, v);
		u.ParseFromArray(buff, v);

		const Authenticate &a = u.GetExtension(extauth);

		QString qn = QString::fromStdString(a.name());
		QString qp = QString::fromStdString(a.pw());
	}
	printf("PBExt    : %8lld %d\n", t.elapsed(), v);

	t.restart();
	for (int i = 0; i < 1000000; ++i) {
		Authenticate apb;
		apb.set_version(-1);
		apb.set_session(1);
		apb.set_name(n.toStdString());
		apb.set_pw(p.toStdString());

		v = apb.ByteSize();

		apb.SerializeToArray(buff, v);
		apb.ParseFromArray(buff, v);
		apb.DiscardUnknownFields();

		QString qn = QString::fromStdString(apb.name());
		QString qp = QString::fromStdString(apb.pw());
	}
	printf("PBNoWrap : %8lld %d\n", t.elapsed(), v);


	t.restart();
	for (int i = 0; i < 1000000; ++i) {
		LongUniversal u;
		Authenticate *apb = u.mutable_z();
		apb->set_version(-1);
		apb->set_session(1);
		apb->set_name(n.toStdString());
		apb->set_pw(p.toStdString());

		std::string str;

		v = u.ByteSize();

		u.SerializeToArray(buff, v);
		u.ParseFromArray(buff, v);

		const Authenticate &a = u.z();

		QString qn = QString::fromStdString(a.name());
		QString qp = QString::fromStdString(a.pw());
	}
	printf("PBLong   : %8lld %d\n", t.elapsed(), v);

	LongUniversal u;
	const LongUniversal::Reflection *r = u.GetReflection();
	t.restart();
	for (int i = 0; i < 1000000; ++i) {
		u.Clear();

		Authenticate *apb = u.mutable_z();
		apb->set_version(-1);
		apb->set_session(1);
		apb->set_name(n.toStdString());
		apb->set_pw(p.toStdString());

		//		std::string str;

		v = u.ByteSize();

		u.SerializeToArray(buff, v);
		u.Clear();
		u.ParseFromArray(buff, v);
		//		u.DiscardUnknownFields();

		//		vector<const google::protobuf::FieldDescriptor *> v;
		//		r->ListFields(u, &v);
		//		printf("%d\n", v.size());

		if (u.has_a() || u.has_b() || u.has_c() || u.has_d() || u.has_e() || u.has_f() || u.has_g() || u.has_h()
			|| u.has_i() || u.has_j() || u.has_k() || u.has_l() || u.has_m() || u.has_n() || u.has_o() || u.has_p()
			|| u.has_q())
			continue;


		Authenticate *a = u.mutable_z();
		a->DiscardUnknownFields();


		QString qn = QString::fromStdString(a->name());
		QString qp = QString::fromStdString(a->pw());
	}
	printf("PBCache  : %8lld %d\n", t.elapsed(), v);

	t.restart();
	for (int i = 0; i < 1000000; ++i) {
		QString nn = QString::fromStdString(n.toStdString());
		QString np = QString::fromStdString(p.toStdString());
	}
	printf("QString  : %8lld %d\n", t.elapsed(), v);
}
