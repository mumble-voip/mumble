#include "SampleBuffer.hpp"
#include <emmintrin.h>
#include "helpers.hpp"

void SampleBufferT<short>::applyStereoGain(float gainFrontLeft, float gainFrontRight) {
    auto sampleCount = getSampleCount();
    auto channels = getChannels();
    size_t leftOver = sampleCount * channels;
    if (CAN_USE_SSE_ON(begin())) {
        //Can use SSE and memory is correctly aligned
        leftOver = (sampleCount * channels) % 8;
        __m128 xmm3;
        float multiplier[4] = { gainFrontLeft, gainFrontRight, gainFrontLeft, gainFrontRight };
        //This is limiting to 4 channels max. But If we implement surround sound we don't really need a center
        xmm3 = _mm_loadu_ps(multiplier);
        helpers::shortFloatMultEx(begin(), (sampleCount * channels) - leftOver, xmm3);
    }
    for (size_t i = sampleCount * channels - leftOver; i < sampleCount * channels; i += channels) {
        (*this)[i] = static_cast<short>((*this)[i] * gainFrontLeft);
        (*this)[i + 1] = static_cast<short>((*this)[i + 1] * gainFrontRight);
    }
}

void SampleBufferT<short>::applyMonoGain(float gain) {
    auto sampleCount = getSampleCount();
    auto channels = getChannels();
    size_t leftOver = sampleCount * channels;
    if (CAN_USE_SSE_ON(begin())) {
        //Can use SSE and memory is correctly aligned
        leftOver = (sampleCount * channels) % 8;
        float multiplier[4] = { gain, gain, gain, gain };
        //This is limiting to 4 channels max. But If we implement surround sound we don't really need a center
        helpers::shortFloatMultEx(begin(), (sampleCount * channels) - leftOver, _mm_loadu_ps(multiplier));
    }
    for (size_t i = sampleCount * channels - leftOver; i < sampleCount * channels; i += 1)
        (*this)[i] = static_cast<short>((*this)[i] * gain);
}
