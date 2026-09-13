// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_HRTFSPATIALIZER_H_
#define MUMBLE_MUMBLE_HRTFSPATIALIZER_H_

#include <QString>

#include <memory>
#include <unordered_map>
#include <vector>

struct MYSOFA_EASY;

namespace fftconvolver {
class FFTConvolver;
}

/// HRTF-based binaural spatializer for positional audio.
///
/// Uses libmysofa for SOFA file loading and HRIR interpolation, and
/// FFTConvolver for real-time partitioned convolution.
///
/// Direction changes are handled by crossfading between two sets of convolvers
/// (double-buffer ping-pong) over one block duration, so the active convolver's
/// overlap-add accumulator is never reset mid-stream.
///
/// Not thread-safe: all methods must be called from the same thread
/// (the audio mixing thread), except loadHRTF() which must be called
/// before mixing begins.
class HrtfSpatializer {
public:
	explicit HrtfSpatializer(int mixerFreq, int blockSize);
	~HrtfSpatializer();

	/// Load HRTF from a SOFA file.
	/// @param sofaPath Path to .sofa file, or empty to use the default.
	/// @return true on success.
	bool loadHRTF(const QString &sofaPath = {});

	/// Spatialize mono input into interleaved stereo output.
	/// @param sourceId   Unique ID for this audio source (e.g. session ID).
	/// @param monoIn     Mono input samples.
	/// @param outStereo  Interleaved stereo output, must hold frameCount*2 floats.
	/// @param frameCount Number of frames to process.
	/// @param dirX       X component of unit direction vector (listener-local, +X=right).
	/// @param dirY       Y component of unit direction vector (listener-local, +Y=up).
	/// @param dirZ       Z component of unit direction vector (listener-local, +Z=forward).
	void spatialize(unsigned int sourceId, const float *monoIn, float *outStereo,
	                unsigned int frameCount, float dirX, float dirY, float dirZ);

	/// Remove state for a source that is no longer active.
	void removeSource(unsigned int sourceId);

	bool isLoaded() const;

private:
	/// A single stereo convolver pair initialised for one HRTF direction.
	struct ConvolverSet {
		std::unique_ptr< fftconvolver::FFTConvolver > convL;
		std::unique_ptr< fftconvolver::FFTConvolver > convR;
		bool initialized = false;
	};

	/// Per-source state.  Two ConvolverSets alternate as current/next (ping-pong).
	struct SourceState {
		ConvolverSet sets[2];
		int currentSet = 0; ///< Index of the set currently faded-in (0 or 1)

		/// Crossfade bookkeeping (in samples).
		/// Only the active convolver set is ever re-inited; the old one
		/// keeps running until the crossfade finishes, then is swapped out.
		bool crossfading             = false;
		int crossfadeProgress        = 0; ///< Samples elapsed so far
		int crossfadeTotalSamples    = 0;

		/// Direction of the HRTF loaded into sets[currentSet].
		/// Sentinel: {2,0,0} means not yet initialised.
		float curDirX = 2.0f;
		float curDirY = 0.0f;
		float curDirZ = 0.0f;

		/// Direction loaded into the inactive (next) set when a crossfade was started.
		/// At crossfade completion, curDir is set to this — not to the latest caller
		/// direction, which may have drifted if mix() was called with variable frameCount.
		float nxtDirX = 0.0f;
		float nxtDirY = 0.0f;
		float nxtDirZ = 0.0f;
	};

	struct MYSOFA_EASY *m_sofa = nullptr;
	int m_filterLength          = 0;
	int m_mixerFreq;
	int m_blockSize;

	std::vector< float > m_irLeft;
	std::vector< float > m_irRight;

	std::unordered_map< unsigned int, std::unique_ptr< SourceState > > m_sources;

	bool directionChanged(const SourceState &state, float dx, float dy, float dz) const;
	void initConvolverSet(ConvolverSet &set, float dx, float dy, float dz);
};

#endif // MUMBLE_MUMBLE_HRTFSPATIALIZER_H_
