// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SHAREDMEMORY_H_
#define MUMBLE_MUMBLE_SHAREDMEMORY_H_

#include <QtCore/QObject>
#include <QtCore/QString>

struct SharedMemory2Private;
class SharedMemory2 : QObject {
private:
	Q_DISABLE_COPY(SharedMemory2)
	Q_OBJECT
protected:
	QString qsName;
	SharedMemory2Private *d;
	unsigned char *a_ucData;
	unsigned int uiSize;
	static unsigned int uiIndex;

public:
	SharedMemory2(QObject *p, unsigned int minsize, const QString &name = QString());
	~SharedMemory2() Q_DECL_OVERRIDE;

	void erase();
	void systemRelease();

	QString name() const;
	unsigned int size() const;
	unsigned char *data();
	const unsigned char *data() const;
};

#endif
