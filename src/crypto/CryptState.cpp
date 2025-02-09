// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "CryptState.h"

void CryptState::updateRollingStats() {
	if (!m_rollingStatsEnabled) {
		return;
	}

	std::chrono::time_point< std::chrono::steady_clock > now = std::chrono::steady_clock::now();

	// Update no more than every few seconds
	if ((now - m_rollingLastSampleTime) < m_rollingScanInterval) {
		return;
	}

	m_rollingLastSampleTime = now;

	PacketStatsSnapshot snapshotLocal;
	snapshotLocal.stats     = m_statsLocal;
	snapshotLocal.timestamp = now;
	m_statsLocalReference.push(snapshotLocal);

	PacketStatsSnapshot snapshotRemote;
	snapshotRemote.stats     = m_statsRemote;
	snapshotRemote.timestamp = now;
	m_statsRemoteReference.push(snapshotRemote);

	while (!m_statsLocalReference.empty() && (now - m_statsLocalReference.front().timestamp) > m_rollingWindow) {
		m_statsLocalReference.pop();
	}

	while (!m_statsRemoteReference.empty() && (now - m_statsRemoteReference.front().timestamp) > m_rollingWindow) {
		m_statsRemoteReference.pop();
	}

	if (!m_statsLocalReference.empty()) {
		m_statsLocalRolling.good   = m_statsLocal.good - m_statsLocalReference.front().stats.good;
		m_statsLocalRolling.late   = m_statsLocal.late - m_statsLocalReference.front().stats.late;
		m_statsLocalRolling.lost   = m_statsLocal.lost - m_statsLocalReference.front().stats.lost;
		m_statsLocalRolling.resync = m_statsLocal.resync - m_statsLocalReference.front().stats.resync;
	}

	if (!m_statsRemoteReference.empty()) {
		m_statsRemoteRolling.good   = m_statsRemote.good - m_statsRemoteReference.front().stats.good;
		m_statsRemoteRolling.late   = m_statsRemote.late - m_statsRemoteReference.front().stats.late;
		m_statsRemoteRolling.lost   = m_statsRemote.lost - m_statsRemoteReference.front().stats.lost;
		m_statsRemoteRolling.resync = m_statsRemote.resync - m_statsRemoteReference.front().stats.resync;
	}
}
