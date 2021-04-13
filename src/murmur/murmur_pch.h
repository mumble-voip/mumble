// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef Q_MOC_RUN
#	ifndef MUMBLE_MURMUR_MURMUR_PCH_H_
#		define MUMBLE_MURMUR_MURMUR_PCH_H_

#		include <QtCore/QtGlobal>

#		ifdef Q_OS_WIN
#			include "win.h"
#		endif

#		ifdef __APPLE__
#			include <CoreFoundation/CoreFoundation.h>
#			include <CoreServices/CoreServices.h>
#			undef check
#			undef TYPE_BOOL
#		endif

#		include <QtCore/QtCore>
#		include <QtNetwork/QtNetwork>
#		include <QtSql/QtSql>
#		include <QtXml/QtXml>
#		ifdef USE_DBUS
#			include <QtDBus/QtDBus>
#		endif

#		ifdef Q_OS_WIN
#			include <QtGui/QtGui>

#			include <delayimp.h>
#			include <qos2.h>
#			include <shellapi.h>
#			include <ws2tcpip.h>

extern "C" {
void __cpuid(int a[4], int b);
};
#		endif

#		ifdef Q_OS_UNIX
#			include <fcntl.h>
#			include <stdio.h>
#			include <stdlib.h>
#			include <sys/resource.h>
#			include <sys/socket.h>
#			include <sys/stat.h>
#			include <sys/time.h>
#			include <sys/types.h>
#			include <syslog.h>
#			include <unistd.h>
#			ifdef Q_OS_LINUX
#				include <linux/types.h> // needed to work around evil magic stuff in capability.h
#				include <sys/capability.h>
#				include <sys/prctl.h>
#			endif
#			include <grp.h>
#			include <pwd.h>
#			if defined __FreeBSD__ || defined __OpenBSD__
#				include <netinet/in_systm.h>
#			endif
#			include <errno.h>
#			include <netinet/in.h>
#			include <netinet/ip.h>
#			include <poll.h>
#			include <signal.h>
#			ifdef Q_OS_DARWIN
#				include <poll.h>
#			endif
#		endif

#		include <cmath>

#		include <boost/bind.hpp>
#		include <boost/function.hpp>
#		include <boost/shared_ptr.hpp>
#		include <boost/weak_ptr.hpp>

#		ifdef USE_ZEROCONF
#			include <dns_sd.h>
#		endif

#		include <openssl/aes.h>
#		include <openssl/bn.h>
#		include <openssl/conf.h>
#		include <openssl/err.h>
#		include <openssl/opensslv.h>
#		include <openssl/pem.h>
#		include <openssl/rand.h>
#		include <openssl/ssl.h>
#		include <openssl/x509v3.h>
/* OpenSSL defines set_key. This breaks our protobuf-generated setters. */
#		undef set_key

#	endif
#endif
