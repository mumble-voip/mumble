// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIORESTART_H_
#define MUMBLE_MUMBLE_AUDIORESTART_H_

#include <QObject>

#include <atomic>
#include <utility>

/// Coalesces notifications from audio threads into one restart on the context's thread.
/// Store with the context object, or keep alive until that context is destroyed and cancels pending work.
class AudioRestart {
private:
	std::atomic_bool m_pending{ false };

public:
	template< typename Restart > void schedule(QObject &context, Restart restart) {
		if (m_pending.exchange(true)) {
			return;
		}

		if (!QMetaObject::invokeMethod(
				&context,
				[this, restart = std::move(restart)]() {
					// Allow notifications during the restart to queue a follow-up. The restart may destroy us.
					m_pending = false;
					restart();
				},
				Qt::QueuedConnection)) {
			m_pending = false;
		}
	}
};

#endif // MUMBLE_MUMBLE_AUDIORESTART_H_
