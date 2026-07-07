#pragma once
#include "DspFilters\Butterworth.h"
#include "DspFilters\RBJ.h"
#include "helpers.hpp"
#include <simpleSource/SimpleComp.h>
#include <numeric>
#define SAMPLE_RATE 48000


#define PI_2     1.57079632679489661923f
#define DELAY_SAMPLES (SAMPLE_RATE / 20)

class RadioEffect {
public:

    virtual void process(float* buffer, int samplesNumber) = 0;

    //void delay(float* buffer, size_t samplesNumber) {
    //
    //    //copy from delayBuffer into buffer.
    //
    //    size_t lengthTillEnd = std::min(static_cast<size_t>(std::distance(delayLine.end(), delayPosition)), samplesNumber);
    //    size_t leftOver = samplesNumber - lengthTillEnd;
    //
    //    delayPosition = std::copy(buffer, buffer + lengthTillEnd, delayPosition);
    //    if (delayPosition > delayLine.end())
    //        delayPosition = delayLine.begin();
    //    if (leftOver)
    //        delayPosition = std::copy(buffer + lengthTillEnd, buffer + samplesNumber, delayPosition);
    //}

    float delay(float input) {
        delayLine[delayPosition] = input;
        delayPosition = (delayPosition + 1) % DELAY_SAMPLES;
        return delayLine[delayPosition];
    }


    template<class T>
    static void processFilter(T& filter, float* buffer, int samplesNumber) {
        filter.template process<float>(samplesNumber, &buffer); //need a float** here so we use &buffer
    }

    virtual void setErrorLeveL(float errorLevel) = 0;

    virtual ~RadioEffect() = default;

    static inline std::function<void(std::string)> onError;

private:
    std::array<float, DELAY_SAMPLES> delayLine{0.f};
    //std::array<float, DELAY_SAMPLES>::iterator delayPosition{ delayLine.begin() };
    size_t delayPosition{ 0 };
};

class UnderWaterRadioEffect : public RadioEffect {
public:
    UnderWaterRadioEffect() {
        filterDD.setup(2, 48000, 1000, 400);
        errorLevel = 0.0f;
        errorLessThan = 0;
    }

    void process(float* buffer, int samplesNumber) override {
        for (auto q = 0; q < samplesNumber; q++) {
            if (rand() < errorLessThan) {
                buffer[q] = 0.0f;
            }
        }
        processFilter(filterDD, buffer, samplesNumber);
        for (auto q = 0; q < samplesNumber; q++) buffer[q] *= 30;
    }

    void setErrorLeveL(float errorLevel) override {
        this->errorLevel = errorLevel;
        errorLessThan = static_cast<int>(RAND_MAX * errorLevel);
    }

private:
    Dsp::SimpleFilter<Dsp::Butterworth::BandPass<2>, 1> filterDD;
    float errorLevel;
    int errorLessThan;
};


class SimpleRadioEffect : public RadioEffect {
public:
    SimpleRadioEffect() {
        phase = 0;
        errorLevel = 0;
    }

    void process(float* buffer, int samplesNumber) override {
        auto acc = 0.0;
        for (auto q = 0; q < samplesNumber; q++) acc += fabs(buffer[q]);
        const auto avg = acc / samplesNumber;
        const auto base = 0.005;

        const auto x = avg / base;

        //for (int q = 0; q < samplesNumber; q++) buffer[q] *= 30.f; //preamplify

        //for (int q = 0; q < samplesNumber; q++) buffer[q] = delay(buffer[q]);
        //for (int q = 0; q < samplesNumber; q++) buffer[q] = ringmodulation(buffer[q], errorLevel);
        //for (int q = 0; q < samplesNumber; q++) buffer[q] = foldback(buffer[q], static_cast<float>(0.3f * (1.0f - errorLevel) * x));
        //if (errorLevel > 1.f || errorLevel < 0.f) onError("errorLevel out of bounds");


        for (int q = 0; q < samplesNumber; q++) buffer[q] = foldback(buffer[q], static_cast<float>(0.3f * (1.0f - errorLevel) * x));
        for (int q = 0; q < samplesNumber; q++) buffer[q] = ringmodulation(delay(buffer[q] *= 30.f), errorLevel);

        processFilter(filterSpeakerHP, buffer, samplesNumber);
        processFilter(filterSpeakerLP, buffer, samplesNumber);
    }


    void setErrorLeveL(float errorLevel) override {
        this->errorLevel = calcErrorLevel(errorLevel);
    }


protected:


    /*
    0.0 0.0
    0.1 0.150000006
    0.2 0.300000012
    0.3 0.600000024
    0.4 0.899999976
    0.5 0.950000048
    0.6 0.960000038
    0.7 0.970000029
    0.8 0.980000019
    0.9 0.995000005
    1.0 0.997799993*/
    static float calcErrorLevel(float errorLevel) {
        static const std::array<float,13> levels{ 0.0, 0.150000006, 0.300000012, 0.600000024, 0.899999976, 0.950000048, 0.960000038, 0.970000029, 0.980000019, 0.995000005, 0.997799993, 0.998799993, 0.99999 };

        const auto part = std::clamp(static_cast<size_t>(errorLevel * 10.f), size_t{0}, levels.size()-2);
        const auto from = levels[part];
        const auto to = levels[part + 1];

        const auto result = from + (to - from) * (errorLevel - part / 10.f);
        //if (result > levels.back() || result < levels.front()) { //#Release disable on release
        //    onError(std::string("TFAR: errorLevel out of bounds! err:")+std::to_string(errorLevel)+"/"+std::to_string(result));
        //}
        return result;
    }


    static float foldback(float in, float threshold) {
        if (threshold < 0.00001) return 0.0f;
        if (in > threshold || in < -threshold) {
            in = fabs(fabs(fmod(in - threshold, threshold * 4)) - threshold * 2) - threshold;
        }
        return in;
    }

    float ringmodulation(float in, float mix) {
        const auto multiple = in * sin(phase * PI_2);
        phase += (90.0f * 1.0f / static_cast<float>(SAMPLE_RATE));
        if (phase > 1.0f) phase = 0.0f;
        return in * (1.0f - mix) + multiple * mix;
    }

    //template <size_t len>
    //class staticRingmodSin {
    //public:
    //    constexpr staticRingmodSin() {}
    //
    //    static constexpr float generate(size_t offs) {
    //        float phase = offs * (90.0f * 1.0f / static_cast<float>(len));
    //        return sin(phase * PI_2);
    //    }
    //
    //    std::array<float,len> arr { generate(1), ..., generate(len) };
    //};


    //static constexpr staticRingmodSin<SAMPLE_RATE> ringmodSin{};

    float errorLevel;
    float phase;

    Dsp::SimpleFilter<Dsp::RBJ::HighPass, 1> filterSpeakerHP;
    Dsp::SimpleFilter<Dsp::RBJ::LowPass, 1> filterSpeakerLP;
};

class LongRangeRadioEffect : public SimpleRadioEffect {
public:
    LongRangeRadioEffect() {
        filterSpeakerHP.setup(SAMPLE_RATE, 520, 0.97);
        filterSpeakerLP.setup(SAMPLE_RATE, 1300, 1.0);
    }
};

class AirborneRadioEffect : public SimpleRadioEffect {
public:
    AirborneRadioEffect() {
        filterSpeakerHP.setup(SAMPLE_RATE, 1000, 1.0);
        filterSpeakerLP.setup(SAMPLE_RATE, 4000, 1.0);
    }
};

class PersonalRadioEffect : public SimpleRadioEffect {
public:
    PersonalRadioEffect() {
        //filterSpeakerHP.setup(SAMPLE_RATE, 520, 0.97);
        //filterSpeakerLP.setup(SAMPLE_RATE, 4300, 2.0);
        filterSpeakerHP.setup(SAMPLE_RATE, 900, 0.85);
        filterSpeakerLP.setup(SAMPLE_RATE, 3000, 2.0);

        //filterMicHP.setup(SAMPLE_RATE, 900, 0.85);
        //filterMicLP.setup(SAMPLE_RATE, 3000, 2.0);
    }

    void process(float* buffer, int samplesNumber) override {
        //processFilter(filterMicHP, buffer, samplesNumber);
        //processFilter(filterMicLP, buffer, samplesNumber);
        SimpleRadioEffect::process(buffer, samplesNumber);
    }

private:
    //Dsp::SimpleFilter<Dsp::RBJ::HighPass, 1> filterMicHP;
    //Dsp::SimpleFilter<Dsp::RBJ::LowPass, 1> filterMicLP;
};

template<class T>
void processRadioEffect(SampleBuffer& samples, float gain, T* effect, stereoMode stereoMode) {
    auto sampleCount = samples.getSampleCount();
    auto channels = samples.getChannels();
    if (stereoMode != stereoMode::stereo) {
        gain *= 1.5f;
    }
    std::vector<float> buffer;
    buffer.resize(sampleCount);
    //((monoCombined / channels) * gain) / 32766
    float monoCombToGainFlt = 1/static_cast<float>(channels) * gain * (1.f/32766.f);
    for (auto i = 0u; i < sampleCount * channels; i += channels) {
        // prepare mono for radio
        auto monoCombined = 0.f;
        for (auto j = 0; j < channels; j++) {
            monoCombined += samples[i + j];
        }

        buffer[i / channels] = monoCombined * monoCombToGainFlt;
        //buffer[i / channels] = (static_cast<float>(monoCombined) * gain) / (32766.f * channels);
    }
    effect->process(buffer.data(), sampleCount);

    //#TODO trust the optimizer
    if (stereoMode == stereoMode::stereo) {
        // put mixed output to stream
        for (auto i = 0u; i < sampleCount * channels; i += channels) {
            float sample = buffer[i / channels];
            const auto newValue = static_cast<short>(std::clamp(sample, -1.f, 1.f) * 32766.f);
            for (int j = 0; j < channels; j++) {
                samples[i + j] = newValue;
            }
        }
    } else if (stereoMode == stereoMode::leftOnly) {
        // put mixed output to stream
        for (auto i = 0u; i < sampleCount * channels; i += channels) {
            float sample = buffer[i / channels];
            const auto newValue = static_cast<short>(std::clamp(sample, -1.f, 1.f) * 32766.f);
            samples[i] = newValue;
            for (int j = 1; j < channels; j++) {
                samples[i + j] = 0;
            }
        }
    } else if (stereoMode == stereoMode::rightOnly) {
        // put mixed output to stream
        for (auto i = 0u; i < sampleCount * channels; i += channels) {
            float sample = buffer[i / channels];
            const auto newValue = static_cast<short>(std::clamp(sample, -1.f, 1.f) * 32766.f);
            samples[i] = 0;
            samples[i+1] = newValue;
            for (int j = 2; j < channels; j++) {
                samples[i + j] = 0;
            }
        }
    }    
}

inline void processCompressor(chunkware_simple::SimpleComp* compressor, SampleBuffer& samples) {
    if (samples.getChannels() >= 2) {
        for (auto& [Sleft,Sright] : samples.iterateStereo()) {
            auto Fleft = static_cast<double>(Sleft);
            auto Fright = static_cast<double>(Sright);
            compressor->process(Fleft, Fright);
            Sleft = static_cast<short>(Fleft);
            Sright = static_cast<short>(Fright);
        }
    }
}
