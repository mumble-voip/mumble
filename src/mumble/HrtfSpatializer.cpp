// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HrtfSpatializer.h"

#include <mysofa.h>
#include "FFTConvolver.h"

#include <QFileInfo>
#include <QStandardPaths>

#include <algorithm>
#include <cmath>
#include <cstring>

// Dot product threshold for triggering a crossfade (~8 degrees of movement).
static constexpr float kDirChangeDotThreshold = 0.99f;

HrtfSpatializer::HrtfSpatializer(int mixerFreq, int blockSize)
	: m_mixerFreq(mixerFreq), m_blockSize(blockSize) {
}

HrtfSpatializer::~HrtfSpatializer() {
	if (m_sofa) {
		mysofa_close(m_sofa);
		m_sofa = nullptr;
	}
}

bool HrtfSpatializer::loadHRTF(const QString &sofaPath) {
	if (m_sofa) {
		mysofa_close(m_sofa);
		m_sofa = nullptr;
	}
	m_filterLength = 0;

	QStringList candidates;
	if (!sofaPath.isEmpty()) {
		candidates << sofaPath;
	}

	// Installed data directory (e.g. /usr/share/mumble/hrtf/default.sofa)
	const QString installed = QStandardPaths::locate(QStandardPaths::AppDataLocation,
	                                                  QStringLiteral("hrtf/default.sofa"));
	if (!installed.isEmpty()) {
		candidates << installed;
	}

#ifdef MUMBLE_HRTF_DEFAULT_SOFA
	// Source-tree path baked in at build time (developer / uninstalled builds)
	candidates << QString::fromUtf8(MUMBLE_HRTF_DEFAULT_SOFA);
#endif

	for (const QString &path : candidates) {
		if (!QFileInfo::exists(path)) {
			continue;
		}

		int err = MYSOFA_OK;
		struct MYSOFA_EASY *sofa =
			mysofa_open(path.toUtf8().constData(), static_cast< float >(m_mixerFreq), &m_filterLength, &err);
		if (sofa && err == MYSOFA_OK) {
			m_sofa = sofa;
			m_irLeft.resize(static_cast< size_t >(m_filterLength));
			m_irRight.resize(static_cast< size_t >(m_filterLength));
			qDebug("HrtfSpatializer: Loaded SOFA file \"%s\" (filterLen=%d @ %d Hz)",
			       qPrintable(path), m_filterLength, m_mixerFreq);
			return true;
		}
		qWarning("HrtfSpatializer: Failed to load \"%s\" (err=%d)", qPrintable(path), err);
	}

	qWarning("HrtfSpatializer: No usable SOFA file found");
	return false;
}

bool HrtfSpatializer::isLoaded() const {
	return m_sofa != nullptr;
}

bool HrtfSpatializer::directionChanged(const SourceState &state, float dx, float dy, float dz) const {
	// Sentinel: {2,0,0} is outside the unit sphere → treat as always-changed
	if (state.curDirX > 1.5f) {
		return true;
	}
	const float dot = state.curDirX * dx + state.curDirY * dy + state.curDirZ * dz;
	return dot < kDirChangeDotThreshold;
}

void HrtfSpatializer::initConvolverSet(ConvolverSet &set, float dx, float dy, float dz) {
	// Convert from listener-local to SOFA Cartesian:
	//   Listener-local: +X=right, +Y=up, +Z=forward
	//   SOFA Cartesian: +X=front, +Y=left, +Z=up
	const float sofaX = dz;   // forward → front
	const float sofaY = -dx;  // right → left (negated)
	const float sofaZ = dy;   // up → up

	float delayL, delayR;
	mysofa_getfilter_float(m_sofa, sofaX, sofaY, sofaZ,
	                       m_irLeft.data(), m_irRight.data(), &delayL, &delayR);

	if (!set.convL) {
		set.convL = std::make_unique< fftconvolver::FFTConvolver >();
		set.convR = std::make_unique< fftconvolver::FFTConvolver >();
	}

	set.convL->init(static_cast< size_t >(m_blockSize),
	                m_irLeft.data(), static_cast< size_t >(m_filterLength));
	set.convR->init(static_cast< size_t >(m_blockSize),
	                m_irRight.data(), static_cast< size_t >(m_filterLength));
	set.initialized = true;
}

void HrtfSpatializer::spatialize(unsigned int sourceId, const float *monoIn, float *outStereo,
                                  unsigned int frameCount, float dirX, float dirY, float dirZ) {
	if (!m_sofa || frameCount == 0) {
		memset(outStereo, 0, sizeof(float) * frameCount * 2);
		return;
	}

	// Ensure direction is a unit vector. When the source is coincident with the listener
	// the direction vector is zero, which would produce an undefined HRTF lookup and cause
	// directionChanged() to fire every block (dot=0 < threshold) → click storm.
	// Fall back to "forward" (+Z in listener-local) as a stable, perceptually neutral direction.
	const float dirLen = std::sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
	if (dirLen < 1e-4f) {
		dirX = 0.0f;
		dirY = 0.0f;
		dirZ = 1.0f;
	} else if (dirLen < 0.99f || dirLen > 1.01f) {
		dirX /= dirLen;
		dirY /= dirLen;
		dirZ /= dirLen;
	}

	// Get or create per-source state
	auto &statePtr = m_sources[sourceId];
	if (!statePtr) {
		statePtr = std::make_unique< SourceState >();
	}
	SourceState *state = statePtr.get();

	ConvolverSet &cur = state->sets[state->currentSet];
	ConvolverSet &nxt = state->sets[1 - state->currentSet];

	if (!cur.initialized) {
		// First call for this source: initialise directly, no crossfade needed.
		initConvolverSet(cur, dirX, dirY, dirZ);
		state->curDirX = dirX;
		state->curDirY = dirY;
		state->curDirZ = dirZ;
	} else if (!state->crossfading && directionChanged(*state, dirX, dirY, dirZ)) {
		// Direction changed enough to warrant a new HRTF.
		// Initialise the inactive set (resets only its own accumulator, not cur's)
		// and start a linear crossfade from cur → nxt over one block.
		initConvolverSet(nxt, dirX, dirY, dirZ);
		state->nxtDirX               = dirX;
		state->nxtDirY               = dirY;
		state->nxtDirZ               = dirZ;
		state->crossfading           = true;
		state->crossfadeProgress     = 0;
		state->crossfadeTotalSamples = m_blockSize;
	}
	// If already crossfading, continue until it completes before triggering another.
	// At most one-block (~10 ms) lag before tracking a new direction — imperceptible.

	static thread_local std::vector< float > tmpL, tmpR, nxtL, nxtR;
	tmpL.resize(frameCount);
	tmpR.resize(frameCount);

	if (!state->crossfading) {
		// Fast path: single convolver set.
		cur.convL->process(monoIn, tmpL.data(), static_cast< size_t >(frameCount));
		cur.convR->process(monoIn, tmpR.data(), static_cast< size_t >(frameCount));

		for (unsigned int i = 0; i < frameCount; ++i) {
			outStereo[2 * i]     = tmpL[i];
			outStereo[2 * i + 1] = tmpR[i];
		}
	} else {
		// Crossfade path: run both convolver sets and blend.
		// cur keeps its accumulator intact (no re-init), so its tail decays naturally.
		nxtL.resize(frameCount);
		nxtR.resize(frameCount);

		cur.convL->process(monoIn, tmpL.data(), static_cast< size_t >(frameCount));
		cur.convR->process(monoIn, tmpR.data(), static_cast< size_t >(frameCount));
		nxt.convL->process(monoIn, nxtL.data(), static_cast< size_t >(frameCount));
		nxt.convR->process(monoIn, nxtR.data(), static_cast< size_t >(frameCount));

		const float invTotal = 1.0f / static_cast< float >(state->crossfadeTotalSamples);
		for (unsigned int i = 0; i < frameCount; ++i) {
			const float alpha = std::min(
				1.0f,
				static_cast< float >(state->crossfadeProgress + i) * invTotal);
			outStereo[2 * i]     = (1.0f - alpha) * tmpL[i] + alpha * nxtL[i];
			outStereo[2 * i + 1] = (1.0f - alpha) * tmpR[i] + alpha * nxtR[i];
		}

		state->crossfadeProgress += static_cast< int >(frameCount);
		if (state->crossfadeProgress >= state->crossfadeTotalSamples) {
			// Crossfade complete: nxt is now the active set.
			// Use the direction nxt was actually initialised with, not the current
			// caller direction (which may have drifted across variable-sized blocks).
			state->currentSet    = 1 - state->currentSet;
			state->crossfading   = false;
			state->curDirX       = state->nxtDirX;
			state->curDirY       = state->nxtDirY;
			state->curDirZ       = state->nxtDirZ;
		}
	}
}

void HrtfSpatializer::removeSource(unsigned int sourceId) {
	m_sources.erase(sourceId);
}
