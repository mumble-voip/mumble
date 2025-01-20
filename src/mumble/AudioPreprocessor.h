// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_AUDIOPREPROCESSOR_H_
#define MUMBLE_MUMBLE_AUDIOPREPROCESSOR_H_

#include <cstdint>
#include <vector>

struct SpeexEchoState_;
struct SpeexPreprocessState_;

class AudioPreprocessor {
public:
	using psd_t = std::vector< std::int32_t >;

	/**
	 * \return Whether the object is initialized and all its methods can be safely called.
	 */
	constexpr explicit operator bool() const { return m_handle != nullptr; }

	AudioPreprocessor() = default;
	AudioPreprocessor(AudioPreprocessor &&other);
	~AudioPreprocessor();

	AudioPreprocessor &operator=(AudioPreprocessor &&other);

	/**
	 * Initializes the object. This must be called at least once before any other function!
	 *
	 * \param sampleRate The sample rate in Hz.
	 *
	 * \param quantum The number of samples that are expected for each preprocess cycle.
	 *                Should correspond to 10-20 ms (480-960 with a sample rate of 48000).
	 *
	 * \return Whether initialization succeeded.
	 */
	bool init(std::uint32_t sampleRate, std::uint32_t quantum);
	/**
	 * Deinitializes the object, doesn't do anything if it's not initialized.
	 * Once that is done no function other than init() can be called.
	 */
	void deinit();

	/**
	 * Runs the preprocessor, which analyzes and modifies the samples.
	 *
	 * \param samples The samples that should be processed (modified in-place).
	 *
	 * \return Whether speech was detected, if VAD is enabled. Always 'true' otherwise.
	 */
	bool run(std::int16_t &samples);

	/**
	 * \return Handle to the echo state object.
	 */
	SpeexEchoState_ *getEchoState();
	/**
	 * Set the echo state object, to toggle the echo canceller.
	 *
	 * \param handle Handle to the echo state. Passing 'nullptr' disables the feature.
	 *
	 * \return Whether the operation succeeded.
	 */
	bool setEchoState(SpeexEchoState_ *handle);

	/**
	 * \return Whether automatic gain control is enabled.
	 */
	bool usesAGC() const;
	/**
	 * Toggles automatic gain control.
	 *
	 * \param enable 'true' to enable the feature, otherwise 'false'.
	 *
	 * \return Whether the operation succeeded.
	 */
	bool setAGC(bool enable);

	/**
	 * \return The maximal gain decrease in dB/second.
	 */
	std::int32_t getAGCDecrement() const;
	/**
	 * Set the maximal gain decrease in dB/second.
	 *
	 * \param value Maximal gain decrease in dB/second.
	 *
	 * \return Whether the operation succeeded.
	 */
	bool setAGCDecrement(std::int32_t value);
	/**
	 * \return The current gain in percentual scale (0-100).
	 */
	std::int32_t getAGCGain() const;

	/**
	 * \return The maximal gain increase in dB/second.
	 */
	std::int32_t getAGCIncrement() const;
	/**
	 * Set the maximal gain increase in dB/second.
	 *
	 * \param value Maximal gain increase in dB/second.
	 *
	 * \return Whether the operation succeeded.
	 */
	bool setAGCIncrement(std::int32_t value);
	/**
	 * \return The maximal gain in dB.
	 */
	std::int32_t getAGCMaxGain() const;
	/**
	 * Set the maximal gain in dB.
	 *
	 * \param value Maximal gain in dB.
	 *
	 * \return Whether the operation succeeded.
	 */
	bool setAGCMaxGain(std::int32_t value);
	/**
	 * \return The automatic gain control level, in a scale from 1 to 32768.
	 */
	std::int32_t getAGCTarget() const;
	/**
	 * Set the automatic gain control level.
	 *
	 * \param value Automatic gain control level, in a scale from 1 to 32768.
	 *
	 * \return Whether the operation succeeded.
	 */
	bool setAGCTarget(std::int32_t value);

	/**
	 * \return Whether denoise is enabled.
	 */
	bool usesDenoise() const;
	/**
	 * Toggles denoise.
	 *
	 * \param enable 'true' to enable the feature, otherwise 'false'.
	 *
	 * \return Whether the operation succeeded.
	 */
	bool setDenoise(bool enable);

	/**
	 * \return Whether dereverb is enabled.
	 */
	bool usesDereverb() const;
	/**
	 * Toggles dereverb.
	 *
	 * \param enable 'true' to enable the feature, otherwise 'false'.
	 *
	 * \return Whether the operation succeeded.
	 */
	bool setDereverb(bool enable);

	/**
	 * \return The maximum attenuation of the noise in dB (negative number).
	 */
	std::int32_t getNoiseSuppress() const;
	/**
	 * Set the maximum attenuation of the noise in dB.
	 *
	 * \param value Maximum attenuation of the noise in dB (negative number).
	 *
	 * \return Whether the operation succeeded.
	 */
	bool setNoiseSuppress(std::int32_t value);

	/**
	 * \return The power spectrum (vector of squared values).
	 */
	psd_t getPSD() const;
	/**
	 * \return The noise estimate (vector of squared values).
	 */
	psd_t getNoisePSD() const;

	/**
	 * \return The amount of probability there was speech in the last processed frame, in percentual scale (0-100).
	 */
	std::int32_t getSpeechProb() const;

	/**
	 * \return Whether voice activity detection is enabled.
	 */
	bool usesVAD() const;
	/**
	 * Toggles voice activity detection.
	 *
	 * \param enable 'true' to enable the feature, otherwise 'false'.
	 *
	 * \return Whether the operation succeeded.
	 */
	bool setVAD(bool enable);

private:
	AudioPreprocessor(const AudioPreprocessor &) = delete;
	AudioPreprocessor &operator=(const AudioPreprocessor &) = delete;

	bool getBool(int op) const;
	bool setBool(int op, bool value);

	std::int32_t getInt32(int op) const;
	bool setInt32(int op, std::int32_t value);

	SpeexPreprocessState_ *m_handle = nullptr;
};

#endif
