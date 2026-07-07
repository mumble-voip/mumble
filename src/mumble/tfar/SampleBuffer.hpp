#pragma once
#include <algorithm>
#include <memory>
#include <variant>
#include <vector>

#define CAN_USE_SSE_ON(x) (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE) && (reinterpret_cast<uintptr_t>(x) % 16 == 0))

template<typename Type = short>
class SampleBufferT {
    class SampleBufferInternal {
        struct SampleStruct {
            Type* samples;
            uint32_t sampleCount;
        };
        //std::vector<Type>*,
        std::variant < std::vector<Type>, SampleStruct> samples;
        uint8_t channels;
    public:
        //SampleBufferInternal(std::vector<Type>& ref, uint8_t channels) : samples(&ref), channels(channels) {}
        SampleBufferInternal(Type* samples, uint32_t sampleCount, uint8_t channels) : samples(SampleStruct{ samples,sampleCount }), channels(channels) {}
        SampleBufferInternal(uint32_t sampleCount, uint8_t channels) : samples([sampleCount, channels]() {std::vector<Type> vec; vec.resize(sampleCount*channels); return vec; }()), channels(channels) {}
        Type* getSamples() {
            return begin();
        }
        uint32_t getSampleCount() const {
            //if (auto vec = std::get_if<std::vector<Type>*>(&samples)) {
            //    return (*vec)->size() / channels;
            //}
            if (auto vec = std::get_if<std::vector<Type>>(&samples)) {
                return vec->size() / channels;
            }
            if (auto strct = std::get_if<SampleStruct>(&samples)) {
                return strct->sampleCount;
            }
            return 0;
        }
        uint8_t getChannels() const { return channels; }

        //void setSampleCount(size_t size) {
        //    //if (auto vec = std::get_if<std::vector<Type>*>(&samples)) {
        //    //    return (*vec)->resize(size * channels);
        //    //}
        //    if (auto vec = std::get_if<std::vector<Type>>(&samples)) {
        //        vec->resize(size * channels);
        //    }
        //    if (auto strct = std::get_if<SampleStruct>(&samples)) {
        //        __debugbreak();
        //    }
        //}

        Type* begin() {
            //if (auto vec = std::get_if<std::vector<Type>*>(&samples)) {
            //    return (*vec)->begin()._Ptr;
            //}
            if (auto vec = std::get_if<std::vector<Type>>(&samples)) {
                return vec->begin()._Ptr;
            }
            if (auto strct = std::get_if<SampleStruct>(&samples)) {
                return strct->samples;
            }
            return nullptr;
        }

        Type* end() {
            //if (auto vec = std::get_if<std::vector<Type>*>(&samples)) {
            //    return (*vec)->end()._Ptr;
            //}
            if (auto vec = std::get_if<std::vector<Type>>(&samples)) {
                return vec->end()._Ptr;
            }
            if (auto strct = std::get_if<SampleStruct>(&samples)) {
                return strct->samples + strct->sampleCount * channels;
            }
            return nullptr;
        }
        void copyTo(std::shared_ptr<SampleBufferInternal> target) {
            auto targetSpace = target->getSampleCount();
            if (targetSpace != getSampleCount()) {
                //if (auto vec = std::get_if<std::vector<Type>*>(&target->samples)) {
                //    (*vec)->resize(getSampleCount() * 2);
                //}
                if (auto vec = std::get_if<std::vector<Type>>(&target->samples)) {
                    vec->resize(getSampleCount() * 2);
                }
                if (auto strct = std::get_if<SampleStruct>(&target->samples)) {
                    std::vector<Type> vec; 
                    vec.resize(getSampleCount() * 2);
                    target->samples = vec;
                }
            }
            std::copy(begin(), end(), target->begin());
        }
    };
    std::shared_ptr<SampleBufferInternal> samples;

public:
    SampleBufferT() {}
    SampleBufferT(const SampleBufferT& other) : samples(other.samples) {}
    SampleBufferT(SampleBufferT&& other) noexcept : samples(std::move(other.samples)) {}

    SampleBufferT& operator=(const SampleBufferT& other) { samples = other.samples; return *this; }
    SampleBufferT& operator=(SampleBufferT&& other) noexcept { samples = std::move(other.samples); return *this; }

    //SampleBufferT(std::vector<Type>& ref, uint8_t channels) : samples(std::make_shared<SampleBufferInternal>(ref, channels)) {}
    SampleBufferT(Type* samples, uint32_t sampleCount, uint8_t channels) : samples(std::make_shared<SampleBufferInternal>(samples, sampleCount, channels)) {}
    SampleBufferT(uint32_t sampleCount, uint8_t channels) : samples(std::make_shared<SampleBufferInternal>(sampleCount, channels)) {}


    class StereoIterator {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = std::pair<Type, Type>;
        using difference_type = size_t;
        using pointer = std::pair<std::reference_wrapper<Type>, std::reference_wrapper<Type>>*;
        using reference = std::pair<Type&, Type&>;
        Type* curSample{ nullptr };
        uint8_t channels{ 2 };
    public:
        explicit StereoIterator(Type* sample, uint8_t channels) : curSample(sample), channels(channels) {}
        StereoIterator& operator++() {
            curSample += channels;
            return *this;
        }
        StereoIterator operator++(int) { StereoIterator retval = *this; ++(*this); return retval; }
        StereoIterator& operator+=(int offs) { curSample += offs * channels; return *this; }
        bool operator==(StereoIterator other) const { return curSample == other.curSample; }
        bool operator!=(StereoIterator other) const { return !(*this == other); }
        reference operator*() const { return std::pair<std::reference_wrapper<Type>, std::reference_wrapper<Type>>(*curSample, *(curSample + 1)); }
        Type& left() { return *curSample; }
        Type& right() { return *(curSample+1); }
    };

    SampleBufferT copy(SampleBufferT& outbuf) const {
        if (!outbuf.samples) {
            outbuf.samples = std::make_shared<SampleBufferInternal>(getSampleCount(), getChannels());
        }
        samples->copyTo(outbuf.samples);
        return outbuf;
    }

    SampleBufferT copy() const {
        SampleBufferT outbuf(samples->getSampleCount(), samples->getChannels());
        samples->copyTo(outbuf.samples);
        return outbuf;
    }

    void mixIntoAdditive(SampleBufferT& target) const {
        //#TODO implementation for float is different
        std::transform(target.samples->begin(), target.samples->end(), samples->begin(), target.samples->begin(), [](int a, int b) {
            return std::clamp(a + b, SHRT_MIN, SHRT_MAX);
        });
    }

    void mixIntoAverage(SampleBufferT& target) const {
        //#TODO implementation for float is different
        std::transform(target.samples->begin(), target.samples->end(), samples->begin(), target.samples->begin(), [](int a, int b) {
            return std::clamp((a + b) / 2, SHRT_MIN, SHRT_MAX);
        });
    }

    void applyStereoGain(float gainFrontLeft, float gainFrontRight);
    void applyMonoGain(float gain);
    ///Has checks for 0/1 gain to not actually do processing
    void applyGain(float gain) {
        if (gain == 0.f) return setToNull();
        if (gain == 1.f) return;
        return applyMonoGain(gain);
    }

    void setToNull() {
        std::fill(samples->begin(), samples->end(), 0);
    }

    const Type* getSamples() const {
        return samples->getSamples();
    }
    Type* getSamples() {
        return samples->getSamples();
    }
    uint8_t getChannels() const {
        return samples->getChannels();
    }
    uint32_t getSampleCount() const {
        return samples->getSampleCount();
    }
    //void setSampleCount(size_t count) const {
    //    return samples->setSampleCount(count);
    //}
    struct SIterHelper {
        StereoIterator begIt;
        StereoIterator endIt;
        StereoIterator begin() { return begIt; }
        StereoIterator end() { return endIt; }
    };
    SIterHelper iterateStereo() {
        return SIterHelper{
            StereoIterator(getSamples(), getChannels()),
            StereoIterator(getSamples() + getSampleCount() * getChannels(), getChannels())
        };
    }
    Type* begin() { return samples->begin(); }
    Type* end() { return samples->end(); }
    Type& operator[](size_t offset) { return *(samples->begin() + offset); }

};

using SampleBuffer = SampleBufferT<short>;
using SampleBufferFloat = SampleBufferT<float>;
