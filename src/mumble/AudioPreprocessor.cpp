// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AudioPreprocessor.h"

#include <utility>

#include <speex/speex_preprocess.h>

AudioPreprocessor::AudioPreprocessor(AudioPreprocessor &&other) : m_handle(std::exchange(other.m_handle, nullptr)) {
}

AudioPreprocessor::~AudioPreprocessor() {
	deinit();
}

AudioPreprocessor &AudioPreprocessor::operator=(AudioPreprocessor &&other) {
	m_handle = std::exchange(other.m_handle, nullptr);
	return *this;
}

bool AudioPreprocessor::init(const std::uint32_t sampleRate, const std::uint32_t quantum) {
	deinit();

	m_handle = speex_preprocess_state_init(static_cast< int >(quantum), static_cast< int >(sampleRate));
	return m_handle != nullptr;
}

void AudioPreprocessor::deinit() {
	if (m_handle) {
		speex_preprocess_state_destroy(m_handle);
		m_handle = nullptr;
	}
}

bool AudioPreprocessor::run(std::int16_t &samples) {
	return speex_preprocess_run(m_handle, &samples);
}

SpeexEchoState_ *AudioPreprocessor::getEchoState() {
	SpeexEchoState_ *handle;
	return speex_preprocess_ctl(m_handle, SPEEX_PREPROCESS_GET_ECHO_STATE, &handle) == 0 ? handle : nullptr;
}

bool AudioPreprocessor::setEchoState(SpeexEchoState_ *handle) {
	return speex_preprocess_ctl(m_handle, SPEEX_PREPROCESS_SET_ECHO_STATE, handle) == 0;
}

bool AudioPreprocessor::usesAGC() const {
	return getBool(SPEEX_PREPROCESS_GET_AGC);
}

bool AudioPreprocessor::setAGC(const bool enable) {
	return setBool(SPEEX_PREPROCESS_SET_AGC, enable);
}

std::int32_t AudioPreprocessor::getAGCDecrement() const {
	return getInt32(SPEEX_PREPROCESS_GET_AGC_DECREMENT);
}

bool AudioPreprocessor::setAGCDecrement(const std::int32_t value) {
	return setInt32(SPEEX_PREPROCESS_SET_AGC_DECREMENT, value);
}

std::int32_t AudioPreprocessor::getAGCGain() const {
	return getInt32(SPEEX_PREPROCESS_GET_AGC_GAIN);
}

std::int32_t AudioPreprocessor::getAGCIncrement() const {
	return getInt32(SPEEX_PREPROCESS_GET_AGC_INCREMENT);
}

bool AudioPreprocessor::setAGCIncrement(const std::int32_t value) {
	return setInt32(SPEEX_PREPROCESS_SET_AGC_INCREMENT, value);
}

std::int32_t AudioPreprocessor::getAGCMaxGain() const {
	return getInt32(SPEEX_PREPROCESS_GET_AGC_MAX_GAIN);
}

bool AudioPreprocessor::setAGCMaxGain(const std::int32_t value) {
	return setInt32(SPEEX_PREPROCESS_SET_AGC_MAX_GAIN, value);
}

std::int32_t AudioPreprocessor::getAGCTarget() const {
	return getInt32(SPEEX_PREPROCESS_GET_AGC_TARGET);
}

bool AudioPreprocessor::setAGCTarget(const std::int32_t value) {
	return setInt32(SPEEX_PREPROCESS_SET_AGC_TARGET, value);
}

bool AudioPreprocessor::usesDenoise() const {
	return getBool(SPEEX_PREPROCESS_GET_DENOISE);
}

bool AudioPreprocessor::setDenoise(const bool enable) {
	return setBool(SPEEX_PREPROCESS_SET_DENOISE, enable);
}

bool AudioPreprocessor::usesDereverb() const {
	return getBool(SPEEX_PREPROCESS_GET_DEREVERB);
}

bool AudioPreprocessor::setDereverb(const bool enable) {
	return setBool(SPEEX_PREPROCESS_SET_DEREVERB, enable);
}

std::int32_t AudioPreprocessor::getNoiseSuppress() const {
	return getInt32(SPEEX_PREPROCESS_GET_NOISE_SUPPRESS);
}

bool AudioPreprocessor::setNoiseSuppress(const std::int32_t value) {
	return setInt32(SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, value);
}

AudioPreprocessor::psd_t AudioPreprocessor::getPSD() const {
	const auto size = getInt32(SPEEX_PREPROCESS_GET_PSD_SIZE);
	if (!size) {
		return {};
	}

	psd_t ret(static_cast< size_t >(size));
	if (speex_preprocess_ctl(m_handle, SPEEX_PREPROCESS_GET_PSD, ret.data()) != 0) {
		return {};
	}

	return ret;
}

AudioPreprocessor::psd_t AudioPreprocessor::getNoisePSD() const {
	const auto size = getInt32(SPEEX_PREPROCESS_GET_PSD_SIZE);
	if (!size) {
		return {};
	}

	psd_t ret(static_cast< size_t >(size));
	if (speex_preprocess_ctl(m_handle, SPEEX_PREPROCESS_GET_NOISE_PSD, ret.data()) != 0) {
		return {};
	}

	return ret;
}

std::int32_t AudioPreprocessor::getSpeechProb() const {
	return getInt32(SPEEX_PREPROCESS_GET_PROB);
}

bool AudioPreprocessor::usesVAD() const {
	return getBool(SPEEX_PREPROCESS_GET_VAD);
}

bool AudioPreprocessor::setVAD(const bool enable) {
	return setBool(SPEEX_PREPROCESS_SET_VAD, enable);
}

bool AudioPreprocessor::getBool(const int op) const {
	const auto val = getInt32(op);
	return static_cast< bool >(val);
}

bool AudioPreprocessor::setBool(const int op, const bool value) {
	return setInt32(op, value);
}

std::int32_t AudioPreprocessor::getInt32(const int op) const {
	spx_int32_t value;
	if (speex_preprocess_ctl(m_handle, op, &value) != 0) {
		return 0;
	}

	return value;
}

bool AudioPreprocessor::setInt32(const int op, std::int32_t value) {
	return speex_preprocess_ctl(m_handle, op, &value) == 0;
}
