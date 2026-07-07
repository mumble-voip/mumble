#pragma once
#include <cstdint>
#include <map>
#include <atomic>
#include <vector>
#include "clunk/wav_file.h"
#include "common.hpp"
#include <memory>
#include <functional>
#include <thread>
#include "Locks.hpp"
#include "SampleBuffer.hpp"

//#define DEBUG_PLAYBACK_TIMES //Prints Processing/Use times to teamspeak log
/*
 Dedmen:
 This style of separate functions for getting and removing may seem weird.
 My first thought for this was that i was going to use queue's which i scrapped for performance reasons.
 But I left that style to give more freedom to underlying containers. I don't really know why... But I'll still leave it.
*/
//#TODO looping playback may need some way to tell when to stop looping

enum class playbackType {
    base,
    stereo,
    raw,
    processing
};

class playbackBase {
protected:
    playbackBase() = default;
    ~playbackBase() = default;
public:
    //************************************
    // Method:    getSamples
    // FullName:  playbackBase::getSamples
    // Returns:   uint32_t availableSamples - the actual amount of samples available in data
    // Parameter: short * & data - will contain pointer to data after call
    // Description: Used to retrieve a Pointer to the beginning of the internal buffer.	 
    //              After processing cleanSamples should be called with the amount of availableSamples returned by this function 
    //************************************
    virtual size_t getSamples(const short*& data) = 0;
    //************************************
    // Method:    cleanSamples
    // FullName:  playbackBase::cleanSamples
    // Returns:   uint32_t removedSamples - the amount of samples actually removed
    // Parameter: uint32_t sampleCount - the amount of samples to remove from the internal buffer
    // Description: This will remove sampleCount samples from the beginning of the internal buffer.
    //************************************
    virtual size_t cleanSamples(size_t sampleCount) = 0;
    //************************************
    // Method:    samplesReady
    // FullName:  playbackBase::samplesReady
    // Returns:   bool ready - If all samples are processed and ready for playback
    // Description: This can be used to determine if a playback that has effects applied to it 
    //				has all samples processed and is ready to output them.
    //				If this returns false then getSamples would block till the thread doing the processing is done. 
    //************************************
    virtual bool samplesReady() { return true; }
    //************************************
    // Method:    isDone
    // FullName:  playbackBase::isDone
    // Returns:   bool - Ready to be deleted
    // Description: Returns whether this playback is done playing and can be deleted
    //************************************
    virtual bool isDone() { return false; }
    //************************************
    // Method:    type
    // FullName:  playbackBase::type
    // Returns:   playbackType - The type of the current Playback. Can be used to check beforehand if a dynamic_cast will succeed
    //************************************
    virtual playbackType type() { return playbackType::base; }

};

//************************************
// Class:    playbackWavStereo
// Description: A basic wav playback that only has processing for setting the stereo channel.
//              Construction is slow because processing stereo of setting is done in constructor.
//************************************
class playbackWavStereo : public playbackBase {
public:
    static std::shared_ptr<playbackWavStereo> create(short* samples, uint32_t sampleCount, uint8_t channels, stereoMode stereo) {
        return std::make_shared<playbackWavStereo>(samples, sampleCount, channels, stereo);
    }
    static std::shared_ptr<playbackWavStereo> create(clunk::WavFile* wavFile, stereoMode stereo) {
        return std::make_shared<playbackWavStereo>(wavFile, stereo);
    }
    static std::shared_ptr<playbackWavStereo> create(std::string wavFilePath, stereoMode stereo) {
        return std::make_shared<playbackWavStereo>(wavFilePath, stereo);
    }
    playbackWavStereo(const short* samples, size_t sampleCount, uint8_t channels, stereoMode stereo, float gain = 1.0f); //#DOCS
    playbackWavStereo(clunk::WavFile* wavFile, stereoMode stereo, float gain = 1.0f);  //#DOCS
    playbackWavStereo(std::string wavFilePath, stereoMode stereo, float gain = 1.0f);	//#DOCS
    virtual ~playbackWavStereo();
    //************************************
    // Method:    getSamples
    // FullName:  playbackWavStereo::getSamples
    // Returns:   uint32_t availableSamples - the actual amount of samples available in data
    // Parameter: short * & data - will contain pointer to data after call
    // Description: Used to retrieve a Pointer to the beginning of the internal buffer.	 
    //              After processing cleanSamples should be called with the amount of availableSamples returned by this function 
    //************************************
    size_t getSamples(const short*& data) override;
    //************************************
    // Method:    cleanSamples
    // FullName:  playbackWavStereo::cleanSamples
    // Returns:   uint32_t removedSamples - the amount of samples actually removed
    // Parameter: uint32_t sampleCount - the amount of samples to remove from the internal buffer
    // Description: This will remove sampleCount samples from the beginning of the internal buffer.
    //************************************
    size_t cleanSamples(size_t sampleCount) override;
    //************************************
    // Method:    samplesReady
    // FullName:  playbackWavStereo::samplesReady
    // Returns:   bool ready - If all samples are processed and ready for playback
    // Description: This can be used to determine if a playback that has effects applied to it 
    //              has all samples processed and is ready to output them.
    //              If this returns false then getSamples would block till the thread doing the processing is done. 
    //************************************
    bool samplesReady() override { return true; }
    //************************************
    // Method:    isDone
    // FullName:  playbackWavStereo::isDone
    // Returns:   bool - Ready to be deleted
    // Description: Returns whether this playback is done playing and can be deleted
    //************************************
    bool isDone() override { return currentPosition == sampleStore.size(); }
    //************************************
    // Method:    type
    // FullName:  playbackWavStereo::type
    // Returns:   playbackType - The type of the current Playback. Can be used to check beforehand if a dynamic_cast will succeed
    //************************************
    playbackType type() override { return playbackType::stereo; }
private:
    void construct(const short* samples, size_t sampleCount, uint8_t channels, stereoMode stereo, float gain); //#DOCS
    void construct(clunk::WavFile* wavFile, stereoMode stereo, float gain);  //#DOCS
    void construct(std::string wavFilePath, stereoMode stereo, float gain);	//#DOCS
    std::vector<short> sampleStore;
    size_t currentPosition;
};

//************************************
// Class:    playbackWavRaw
// Description:	A basic wav playback that only plays back samples
//************************************
class playbackWavRaw : public playbackBase {
public:
    //playbackWavRaw(short* samples, size_t sampleCount, uint8_t channels); //#DOCS
    playbackWavRaw(); //#DOCS
    virtual ~playbackWavRaw() = default;
    //************************************
    // Method:    getSamples
    // FullName:  playbackWavRaw::getSamples
    // Returns:   uint32_t availableSamples - the actual amount of samples available in data
    // Parameter: short * & data - will contain pointer to data after call
    // Description:	Used to retrieve a Pointer to the beginning of the internal buffer.	 
    //				After processing cleanSamples should be called with the amount of availableSamples returned by this function 
    //************************************
    size_t getSamples(const short*& data) override;
    //************************************
    // Method:    cleanSamples
    // FullName:  playbackWavRaw::cleanSamples
    // Returns:   uint32_t removedSamples - the amount of samples actually removed
    // Parameter: uint32_t sampleCount - the amount of samples to remove from the internal buffer
    // Description: This will remove sampleCount samples from the beginning of the internal buffer.
    //************************************
    size_t cleanSamples(size_t sampleCount) override;
    //************************************
    // Method:    samplesReady
    // FullName:  playbackWavRaw::samplesReady
    // Returns:   bool ready - If all samples are processed and ready for playback
    // Description: This can be used to determine if a playback that has effects applied to it 
    //				has all samples processed and is ready to output them.
    //				If this returns false then getSamples would block till the thread doing the processing is done. 
    //************************************
    bool samplesReady() override { return true; }
    //************************************
    // Method:    isDone
    // FullName:  playbackWavRaw::isDone
    // Returns:   bool - Ready to be deleted
    // Description: Returns whether this playback is done playing and can be deleted
    //************************************
    bool isDone() override { return currentPosition == sampleStore.size(); }
    void appendSamples(const short* samples, size_t sampleCount, uint8_t channels);//#DOCS
    //************************************
    // Method:    type
    // FullName:  playbackWavRaw::type
    // Returns:   playbackType - The type of the current Playback. Can be used to check beforehand if a dynamic_cast will succeed
    //************************************
    playbackType type() override { return playbackType::raw; }
#ifdef DEBUG_PLAYBACK_TIMES
    std::chrono::high_resolution_clock::time_point creation;
    std::once_flag flag1;
#endif
private:
    std::vector<short> sampleStore;
    size_t currentPosition;
};

//************************************
// Class:    playbackWavProcessing
// Description:	A playback that does more in-depth processing on audio data
//************************************
class playbackWavProcessing : public playbackBase {
public:
    playbackWavProcessing(const short* samples, size_t sampleCount, int channels, std::vector<std::function<void(SampleBuffer&)>> processors); //#DOCS
    virtual ~playbackWavProcessing() {if (myThread) {
        if (myThread->joinable())
            myThread->join();
        delete myThread;
    }};
    //************************************
    // Method:    getSamples
    // FullName:  playbackWavProcessing::getSamples
    // Returns:   uint32_t availableSamples - the actual amount of samples available in data
    // Parameter: short * & data - will contain pointer to data after call
    // Description: Used to retrieve a Pointer to the beginning of the internal buffer.	 
    //              After processing cleanSamples should be called with the amount of availableSamples returned by this function
    //              If multithreading is enabled this can block until all processing is done!
    //************************************
    size_t getSamples(const short*& data) override;
    //************************************
    // Method:    cleanSamples
    // FullName:  playbackWavProcessing::cleanSamples
    // Returns:   uint32_t removedSamples - the amount of samples actually removed
    // Parameter: uint32_t sampleCount - the amount of samples to remove from the internal buffer
    // Description: This will remove sampleCount samples from the beginning of the internal buffer.
    //************************************
    size_t cleanSamples(size_t sampleCount) override;
    //************************************
    // Method:    samplesReady
    // FullName:  playbackWavProcessing::samplesReady
    // Returns:   bool ready - If all samples are processed and ready for playback
    // Description: This can be used to determine if a playback that has effects applied to it 
    //              has all samples processed and is ready to output them.
    //              If this returns false then getSamples would block till the thread doing the processing is done. 
    //************************************
    bool samplesReady() override;
    //************************************
    // Method:    isDone
    // FullName:  playbackWavProcessing::isDone
    // Returns:   bool - Ready to be deleted
    // Description: Returns whether this playback is done playing and can be deleted
    //************************************
    bool isDone() override { return currentPosition == sampleStore.size(); }
    //************************************
    // Method:    type
    // FullName:  playbackWavProcessing::type
    // Returns:   playbackType - The type of the current Playback. Can be used to check beforehand if a dynamic_cast will succeed
    //************************************
    playbackType type() override { return playbackType::processing; }

#ifdef DEBUG_PLAYBACK_TIMES
    std::chrono::high_resolution_clock::time_point creation;
    std::once_flag flag1;
#endif
private:
    std::vector<short> sampleStore;
    size_t currentPosition;
    std::atomic<bool> processingDone;
    std::vector<std::function<void(SampleBuffer&)>> functors;
    std::thread* myThread;
};


class SoundFile {
public:
    enum class SoundFileType {
        PluginFolderFile,
        RawSamples,
        Empty
    } type;
    std::string fileName;
    std::basic_string_view<short> samples;
    uint8_t channels{0};

    SoundFile() : type(SoundFileType::Empty) {}
    SoundFile(std::string fileName) : type(SoundFileType::PluginFolderFile), fileName(std::move(fileName)) {}
    SoundFile(const short* samples, size_t sampleCount, uint8_t channels) : type(SoundFileType::RawSamples), samples({ samples, sampleCount }), channels(channels) {}

    bool isEmpty() const { return type == SoundFileType::Empty; }
    std::string getFullPath() const;
};

class SoundDirectory {
public:

    static SoundFile getRadioSound(PTTDelayArguments::subtypes subtype, bool start) {
        switch (subtype) {
        case PTTDelayArguments::subtypes::digital_lr:
            return SoundFile(start ? "radio-sounds/lr/local_start" : "radio-sounds/lr/local_end");
            break;
        case PTTDelayArguments::subtypes::digital:
            return SoundFile(start ? "radio-sounds/sw/local_start" : "radio-sounds/sw/local_end");
        case PTTDelayArguments::subtypes::airborne:
            return SoundFile(start ? "radio-sounds/ab/local_start" : "radio-sounds/ab/local_end");
        default:
            return SoundFile();
        }
    }

    static SoundFile getRemoteRadioSound(PTTDelayArguments::subtypes subtype, bool start) {
        switch (subtype) {
        case PTTDelayArguments::subtypes::digital_lr:
            return SoundFile(start ? "radio-sounds/lr/remote_start" : "radio-sounds/lr/remote_end");
            break;
        case PTTDelayArguments::subtypes::digital:
            return SoundFile(start ? "radio-sounds/sw/remote_start" : "radio-sounds/sw/remote_end");
        case PTTDelayArguments::subtypes::airborne:
            return SoundFile(start ? "radio-sounds/ab/remote_start" : "radio-sounds/ab/remote_end");
        default:
            return SoundFile();
        }
    }
};

class PlaybackHandler {
public:
    PlaybackHandler();
    ~PlaybackHandler() = default;

    void onEditMixedPlaybackVoiceDataEvent(short* samples, int sampleCount, int channels, const unsigned int* channelSpeakerArray, unsigned int* channelFillMask);
    void appendPlayback(std::string name, SoundFile file);
    void appendPlayback(std::string name, SoundFile file, stereoMode stereo, float gain = 1.0f);
    //functors is short* samples,int sampleCount,int channels
    void appendPlayback(std::string name, SoundFile file, std::vector<std::function<void(SampleBuffer&)>> functors);
    //tangentReleased(serverconhandler,clientID) iterates through playbacks and calls their onTangentReleased funcs
    std::map<std::string, std::shared_ptr<playbackBase>> playbacks;
    std::map<std::string, std::shared_ptr<clunk::WavFile>> wavCache;


    void playWavFile(SoundFile file);
    void playWavFile(TSServerID serverConnectionHandlerID, SoundFile file, float gain, stereoMode stereo);
    void playWavFile(TSServerID serverConnectionHandlerID, SoundFile file, float gain, Position3D position, bool onGround, int radioVolume, bool underwater, float vehicleVolumeLoss, bool vehicleCheck, stereoMode stereoMode = stereoMode::stereo);
private:
    std::shared_ptr<clunk::WavFile> getWavFileFromPath(const std::string& filePath);
    CriticalSectionLock playbackCriticalSection{ "PlaybackHandler" };
};

