// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QCoreApplication>
#include <QTextStream>
#include <QFileInfo>
#include <QThread>
#include <QDebug>

#ifdef Q_OS_WIN
#include "games_win32.h"
#else
#include "games_linux.h"
#endif

unsigned char buf[100*1024*1024]; // Big reservation

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	memset(&buf[0], 0, sizeof(buf));

	qDebug() << "[Rocket League] Process ID =" << a.applicationPid();

	int x;
	qDebug() << "[Rocket League] Process stack address =" << &x;

	uintptr_t base = getProcessAddress();
	qDebug() << "[Rocket League] Process base address =" << QString("%1").arg(base, 0, 16);

	if (base == 0) {
		qFatal("[Rocket League] getProcessAddress() returned 0!");
	}

#ifdef Q_OS_WIN
	const uintptr_t camera_pos_offset = 0x17428D8;
	const uintptr_t camera_front_offset = 0x17428C0;
	const uintptr_t camera_top_offset = 0x17428CC;
	const uintptr_t avatar_pos_offset = 0x0;
	const uintptr_t avatar_front_offset = 0xC;
	const uintptr_t avatar_base_offset = 0x174269C;
	const uintptr_t avatar_offset_0 = 0x448;
	const uintptr_t avatar_offset_1 = 0x440;
	const uintptr_t avatar_offset_2 = 0x0;
	const uintptr_t avatar_offset_3 = 0x1C;
#else
	const uintptr_t camera_pos_offset = 0x302AAB8;
	const uintptr_t camera_front_offset = 0x302AAA0;
	const uintptr_t camera_top_offset = 0x302AAAC;
	const uintptr_t avatar_pos_offset = 0x60;
	const uintptr_t avatar_front_offset = 0x6C;
	const uintptr_t avatar_base_offset = 0x302A4F0;
	const uintptr_t avatar_offset_0 = 0x6C8;
	const uintptr_t avatar_offset_1 = 0x2B0;
	const uintptr_t avatar_offset_2 = 0x38;
#endif

	unsigned char *scratch = (unsigned char *) malloc(avatar_pos_offset + sizeof(float) * 6);
	float *avatar_pos = (float *)(scratch + avatar_pos_offset);
	float *avatar_front = (float *)(scratch + avatar_front_offset);

#ifdef Q_OS_WIN
	void* avatar_2 = malloc(avatar_offset_3 + sizeof(void *));
	*((uintptr_t *)((char *)avatar_2 + avatar_offset_3)) = (uintptr_t) scratch;

	void* avatar_1 = malloc(avatar_offset_2 + sizeof(void *));
	*((uintptr_t *)((char *)avatar_1 + avatar_offset_2)) = (uintptr_t) avatar_2;
#else
	void* avatar_1 = malloc(avatar_offset_2 + sizeof(void *));
	*((uintptr_t *)((char *)avatar_1 + avatar_offset_2)) = (uintptr_t) scratch;
#endif

	void* avatar_0 = malloc(avatar_offset_1 + sizeof(void *));
	*((uintptr_t *)((char *)avatar_0 + avatar_offset_1)) = (uintptr_t) avatar_1;

	void* avatar_base = malloc(avatar_offset_0 + sizeof(void *));
	*((uintptr_t *)((char *)avatar_base + avatar_offset_0)) = (uintptr_t) avatar_0;

	uintptr_t avatar_base_ptr = base + avatar_base_offset;
	*(uintptr_t *)(avatar_base_ptr) = (uintptr_t) avatar_base;

	float *camera_pos = (float *)(base + camera_pos_offset);
	float *camera_front = (float *)(base + camera_front_offset);
	float *camera_top = (float *)(base + camera_top_offset);

	avatar_pos[0] = 100.0f;
	avatar_pos[1] = 200.0f;
	avatar_pos[2] = 300.0f;

	camera_pos[0] = 100.0f;
	camera_pos[1] = 200.0f;
	camera_pos[2] = 300.0f;

	avatar_front[0] = 1.0f;
	avatar_front[1] = 2.0f;
	avatar_front[2] = 3.0f;

	camera_front[0] = 1.0f;
	camera_front[1] = 2.0f;
	camera_front[2] = 3.0f;

	camera_top[0] = 1.0f;
	camera_top[1] = 2.0f;
	camera_top[2] = 3.0f;

	{
		uintptr_t avatar_base = *(uintptr_t *)(base + avatar_base_offset);
		uintptr_t avatar_0 = *(uintptr_t *)(avatar_base + avatar_offset_0);
		uintptr_t avatar_1 = *(uintptr_t *)(avatar_0 + avatar_offset_1);
#ifdef Q_OS_WIN
		uintptr_t avatar_2 = *(uintptr_t *)(avatar_1 + avatar_offset_2);
		uintptr_t avatar_ptr = *(uintptr_t *)(avatar_2 + avatar_offset_3);
#else
		uintptr_t avatar_ptr = *(uintptr_t *)(avatar_1 + avatar_offset_2);
#endif

		bool ok = true;

		// Avatar position vector
		if (ok) {
			float *avatar_position = (float *)(avatar_ptr + avatar_pos_offset);
			ok = (*avatar_position == 100.0f && *(avatar_position + 1) == 200.0f && *(avatar_position + 2) == 300.0f);
		}

		// Avatar front vector
		if (ok) {
			float *avatar_front = (float *)(avatar_ptr + avatar_front_offset);
			ok = (*avatar_front == 1.0f && *(avatar_front + 1) == 2.0f && *(avatar_front + 2) == 3.0f);
		}

		// Camera position vector
		if (ok) {
			float *camera_position = (float *)(base + camera_pos_offset);
			ok = (*camera_position == 100.0f && *(camera_position + 1) == 200.0f && *(camera_position + 2) == 300.0f);
		}

		// Camera front vector
		if (ok) {
			float *camera_front = (float *)(base + camera_front_offset);
			ok = (*camera_front == 1.0f && *(camera_front + 1) == 2.0f && *(camera_front + 2) == 3.0f);
		}

		// Camera top vector
		if (ok) {
			float *camera_top = (float *)(base + camera_top_offset);
			ok = (*camera_top == 1.0f && *(camera_top + 1) == 2.0f && *(camera_top + 2) == 3.0f);
		}

		if (!ok) {
			qFatal("[Rocket League] Self-test failed!");
		}
	}

	forever {
		QThread::sleep(60);
		qDebug() << "[Rocket League] Waiting for someone to close me...";
	}

	return a.exec();
}
