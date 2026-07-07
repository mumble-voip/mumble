#include "PlaybackHandler.hpp"
#include <vector>

#include "helpers.hpp"
#include <dspfilters/Filter.h>
#include <dspfilters/Butterworth.h>
#include "clientData.hpp"
#include "task_force_radio.hpp"
#include "Teamspeak.hpp"
#include "Logger.hpp"
#include <filesystem>

std::string SoundFile::getFullPath() const {
    return TFAR::getInstance().getPluginPath() + fileName + ".wav";
}


PlaybackHandler::PlaybackHandler() {
    TFAR::getInstance().doDiagReport.connect([this](std::stringstream& diag) {
        diag << "PH:\n";
        std::array<std::string_view, 4> typeToString{ "base"sv, "stereo"sv, "raw"sv, "processing"sv };

        for (auto& it : playbacks) {
            diag << TS_INDENT << "PB: " << it.first << ":\n";
            diag << TS_INDENT << TS_INDENT << "Type: " << typeToString[static_cast<uint8_t>(it.second->type())] << "\n";
            diag << TS_INDENT << TS_INDENT << "samplesReady: " << it.second->samplesReady() << "\n";
            diag << TS_INDENT << TS_INDENT << "isDone: " << it.second->isDone() << "\n";
        }

        for (auto& it : wavCache) {
            diag << TS_INDENT << "WC: " << it.first << ":\n";
        }
    });
}

void PlaybackHandler::onEditMixedPlaybackVoiceDataEvent(short * samples, int sampleCount, int channels, const unsigned int *, unsigned int * channelFillMask) {
    ProfileFunction;
    LockGuard_exclusive lock(playbackCriticalSection);
    bool fill = false;
    std::vector<std::string> to_remove;
    if (!(*channelFillMask & 3)) {
        memset(samples, 0, sampleCount * channels * sizeof(short));
    }
    for (auto& [name,playback] : playbacks) {
        const short* playbackSamples = nullptr;
        const size_t playbackSampleCount = playback->getSamples(playbackSamples);
        if (playbackSampleCount == 0) continue;

        int outputPosition = 0;
        int inputPosition = 0;
        //mix stereo sound into multichannel sound
        while (outputPosition < sampleCount * channels && (static_cast<int>(playbackSampleCount) - inputPosition) > 0) {
            for (int q = 0; q < 2; q++) {
#ifdef _DEBUG
                if (outputPosition > sampleCount * channels) __debugbreak();
                if (inputPosition > playbackSampleCount) __debugbreak();

#endif   
                const auto s = playbackSamples[inputPosition];

                samples[outputPosition] = std::clamp(samples[outputPosition] + s, SHRT_MIN, SHRT_MAX);

                outputPosition++;
                inputPosition++;
                fill = true;
            }
            outputPosition += std::max(channels - 2, 0);
        }
        playback->cleanSamples(inputPosition);
        if (playback->isDone()) {
            to_remove.push_back(name);
        }
    }
    for (const auto& it : to_remove) {
        playbacks.erase(it);
    }

    if (fill) *channelFillMask |= 3;
}

void PlaybackHandler::appendPlayback(std::string name, SoundFile file) {
    if (file.type == SoundFile::SoundFileType::PluginFolderFile) return appendPlayback(name, file, stereoMode::stereo, 1.f);

    LockGuard_exclusive lock(playbackCriticalSection);
    if (playbacks.count(name) == 0) {
        std::shared_ptr<playbackWavRaw> d = std::make_shared<playbackWavRaw>();
        playbacks[name] = d;
        d->appendSamples(file.samples.data(), file.samples.size(), file.channels);
    } else {
        if (playbacks[name]->type() == playbackType::raw) {
            std::static_pointer_cast<playbackWavRaw>(playbacks[name])->appendSamples(file.samples.data(), file.samples.size(), file.channels);
        } else {
            MessageBoxA(0, "void PlaybackHandler::appendPlayback(std::string name, SoundFile file) 93", "tfar", 0);
            __debugbreak();
            //should not have different playback types with same name.
        }
    }
}

void PlaybackHandler::appendPlayback(std::string name, SoundFile file, stereoMode stereo, float gain) {
    LockGuard_exclusive lock(playbackCriticalSection);
    if (playbacks.count(name) == 0) {
        if (file.type == SoundFile::SoundFileType::PluginFolderFile) {
            
            if (auto wave = getWavFileFromPath(file.getFullPath()))
                playbacks[name] = std::make_shared<playbackWavStereo>(wave.get(), stereo, gain);//we can use wave.get() because playbackWavStereo doesn't hold a ref to it
        } else {
            playbacks[name] = std::make_shared<playbackWavStereo>(file.samples.data(), file.samples.size(), file.channels, stereo, gain);
        }
    } else {
        MessageBoxA(0, "void PlaybackHandler::appendPlayback(std::string name, SoundFile file, stereoMode stereo, float gain) 110", "tfar", 0);
        __debugbreak();
        //should not have different playback types with same name.
        //wavStereo types cannot append
    }
}

void PlaybackHandler::appendPlayback(std::string name, SoundFile file, std::vector<std::function<void(SampleBuffer&)>> functors) {
    LockGuard_exclusive lock(playbackCriticalSection);
    if (playbacks.count(name) == 0) {
        if (file.type == SoundFile::SoundFileType::PluginFolderFile) {
            if (auto wave = getWavFileFromPath(file.getFullPath()))
                playbacks[name] = std::make_shared<playbackWavProcessing>(static_cast<short*>(wave->_data.get_ptr()), (wave->_data.get_size() / sizeof(short)) / wave->_spec.channels, wave->_spec.channels, functors);
        } else {
            playbacks[name] = std::make_shared<playbackWavProcessing>(file.samples.data(), file.samples.size(), file.channels, functors);
        }
    } else {
        MessageBoxA(0, "void PlaybackHandler::appendPlayback(std::string name, SoundFile file, functors) 128", "tfar", 0);
        __debugbreak();
    }
}

void PlaybackHandler::playWavFile(SoundFile file) {
    if (!Teamspeak::isConnected()) return;
    if (file.type != SoundFile::SoundFileType::PluginFolderFile) return playWavFile(Teamspeak::getCurrentServerConnection(), file, 1.0, stereoMode::stereo);
    Teamspeak::playWavFile(file.getFullPath());
}

void PlaybackHandler::playWavFile(TSServerID serverConnectionHandlerID, SoundFile file, float gain, stereoMode stereo) {
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _mm_setcsr((_mm_getcsr() & ~0x0040) | (0x0040));//_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
    if (!Teamspeak::isConnected(serverConnectionHandlerID)) return;

    appendPlayback(file.fileName + std::to_string(rand()), file, stereo, gain);
}

void PlaybackHandler::playWavFile(TSServerID serverConnectionHandlerID, SoundFile file, float gain, Position3D position, bool onGround, int radioVolume, bool underwater, float vehicleVolumeLoss, bool vehicleCheck, stereoMode stereoMode) {
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _mm_setcsr((_mm_getcsr() & ~0x0040) | (0x0040));//_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
    if (!Teamspeak::isConnected(serverConnectionHandlerID)) return;

    const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverConnectionHandlerID);
    if (!clientDataDir) return;

    std::vector<std::function<void(SampleBuffer&)>> processors;
    const auto id = file.fileName + std::to_string(rand());
    //apply gain
    if (gain != 1.0f)
        processors.emplace_back([gain](SampleBuffer& samples) {
        samples.applyGain(gain);
    });


    //processors.push_back([gain](short* samples, size_t sampleCount, uint8_t channels) {
    //    std::ofstream f("P:/out.raw", std::ios::binary);
    //    for (size_t i = sampleCount; i < sampleCount * channels; i++)
    //        samples[i] = std::numeric_limits<short>::lowest();
    //
    //
    //    for (size_t i = 0; i < sampleCount * channels; i++)
    //        f.write((char*)&samples[i],2);
    //    f.close();
    //});

    auto myClientData = clientDataDir->myClientData;

    execAtReturn ret([this, &id, &file, &processors]() {
        appendPlayback(id, file, processors);
    });

    if (!myClientData) return;

    const auto speakerDistance = (radioVolume / 10.f) * TFAR::getInstance().m_gameData.speakerDistance;
    const auto distanceFromRadio = position.distanceTo(myClientData->getClientPosition());

    if (vehicleVolumeLoss > 0.01f && !vehicleCheck)
        //In vehicle filter
        processors.emplace_back([id, distanceFromRadio, speakerDistance, vehicleVolumeLoss, myClientDataWeak = std::weak_ptr<clientData>(myClientData)](SampleBuffer& samples) {
        if (auto myClientData = myClientDataWeak.lock()) {
            helpers::processFilterStereo<Dsp::SimpleFilter<Dsp::Butterworth::LowPass<2>, MAX_CHANNELS>>(samples, helpers::volumeAttenuation(distanceFromRadio, true, round(speakerDistance), 1.0f - vehicleVolumeLoss) * pow(1.0f - vehicleVolumeLoss, 1.2f), myClientData->effects.getFilterVehicle(id + "vehicle", vehicleVolumeLoss));
            myClientData->effects.removeFilterVehicle(id + "vehicle");

        }
    });

    if (onGround) {
        //Speaker effect
        processors.emplace_back([id, distanceFromRadio, speakerDistance, myClientDataWeak = std::weak_ptr<clientData>(myClientData)](SampleBuffer& samples) {
            samples.applyGain(helpers::volumeAttenuation(distanceFromRadio, true, round(speakerDistance)));
            if (auto myClientData = myClientDataWeak.lock()) {
                helpers::processFilterStereo<Dsp::SimpleFilter<Dsp::Butterworth::BandPass<1>, MAX_CHANNELS>>(samples, SPEAKER_GAIN, myClientData->effects.getSpeakerFilter(id));
                myClientData->effects.removeSpeakerFilter(id);
            }
        });

        if (underwater) {
            //Underwater Speaker effect
            processors.emplace_back([id, myClientDataWeak = std::weak_ptr<clientData>(myClientData)](SampleBuffer& samples) {
                if (auto myClientData = myClientDataWeak.lock()) {
                    helpers::processFilterStereo<Dsp::SimpleFilter<Dsp::Butterworth::LowPass<4>, MAX_CHANNELS>>(samples, CANT_SPEAK_GAIN * 50, myClientData->effects.getFilterCantSpeak(id));
                    myClientData->effects.removeFilterCantSpeak(id);
                }
            });
        }

        //3D Positioning
        if (!position.isNull())
            if (file.type == SoundFile::SoundFileType::PluginFolderFile) //Clunk can't handle these somehow
                processors.emplace_back([position, myClientDataWeak = std::weak_ptr<clientData>(myClientData), id](SampleBuffer& samples) {
                    auto myClientData = myClientDataWeak.lock();
                    if (!myClientData) return;
                    auto pClunk = myClientData->effects.getClunk(id);
                    auto relativePos = myClientData->getClientPosition().directionTo(position);
                    auto viewDirection = myClientData->getViewDirection().toAngle();
                    helpers::applyILD(samples, relativePos, viewDirection); //interaural level difference
                    myClientData->effects.removeClunk(id);
                });
            else
                processors.emplace_back([position, myClientDataWeak = std::weak_ptr<clientData>(myClientData), id](SampleBuffer& samples) {
                    auto myClientData = myClientDataWeak.lock();
                    if (!myClientData) return;
                    auto pClunk = myClientData->effects.getClunk(id);
                    auto relativePos = myClientData->getClientPosition().directionTo(position);
                    auto viewDirection = myClientData->getViewDirection().toAngle();
                    pClunk->process(samples, relativePos, viewDirection); //interaural time difference
                    helpers::applyILD(samples, relativePos, viewDirection); //interaural level difference
                    myClientData->effects.removeClunk(id);
                });

    } else {
        //muting for stereo mode
        if (stereoMode != stereoMode::stereo)
            processors.emplace_back([stereoMode](SampleBuffer& samples) {
            auto sampleCount = samples.getSampleCount();
            auto channels = samples.getChannels();
            //if (false && channels == 2) { //#TODO this optimization was a fail.
            //    //Performance opt using 32bit operations instead of 16 bit ones
            //    uint32_t* samples32bit = reinterpret_cast<uint32_t*>(samples);
            //    if (stereoMode == stereoMode::leftOnly) {//Only left
            //        for (size_t q = 0; q < sampleCount / 2; q += 1)
            //            samples32bit[q] &= 0xFFFF0000;//mute right channel
            //    } else if (stereoMode == stereoMode::rightOnly)//Only right
            //        for (size_t q = 0; q < sampleCount / 2; q += 1)
            //            samples32bit[q] &= 0x0000FFFF;//mute left channel
            //} else {
                if (stereoMode == stereoMode::leftOnly) {//Only left
                    for (size_t q = 0; q < sampleCount * channels; q += channels)
                        samples[q + 1] = 0;//mute right channel
                } else if (stereoMode == stereoMode::rightOnly)//Only right
                    for (size_t q = 0; q < sampleCount * channels; q += channels)
                        samples[q] = 0;//mute left channel
            //}
        });

    }
}

std::shared_ptr<clunk::WavFile> PlaybackHandler::getWavFileFromPath(const std::string& filePath) {
    if (auto found = wavCache.find(filePath); found != wavCache.end())
        return found->second;

    auto p = std::filesystem::u8path(filePath.c_str()); //Handle utf8 input path

    if (FILE *f =
#ifdef _MSC_VER
        _wfopen(p.c_str(), L"rb")
#else
        std::fopen(p.c_str(), "rb")
#endif
        ) {
        std::shared_ptr<clunk::WavFile> wav = std::make_shared<clunk::WavFile>(f);
        wav->read();
        if (wav->ok() && wav->_spec.channels == 2 && wav->_spec.sample_rate == 48000) {
            wavCache[filePath] = wav;
        } else {
            Logger::log(LoggerTypes::teamspeakClientlog, "Cannot read Soundfile: " + filePath, LogLevel::LogLevel_ERROR);
            return {};
        }
        fclose(f);
        return wav;
    }

    Logger::log(LoggerTypes::teamspeakClientlog, "Cannot open Soundfile: " + filePath + " " + strerror(errno), LogLevel::LogLevel_ERROR);
    return {};
}

void playbackWavStereo::construct(clunk::WavFile* wavFile, stereoMode stereo, float gain) {
    if (wavFile->ok() && wavFile->_spec.channels == 2 && wavFile->_spec.sample_rate == 48000 && wavFile->_spec.format == clunk::AudioSpec::S16) {
        construct(static_cast<short*>(wavFile->_data.get_ptr()), (wavFile->_data.get_size() / sizeof(short)) / wavFile->_spec.channels, wavFile->_spec.channels, stereo, gain);
    } else if (wavFile->ok()) {
        MessageBoxA(0, "Unknown audio file has invalid format.", "Task Force Arrowhead Radio", MB_OK);
    }
}

void playbackWavStereo::construct(std::string wavFilePath, stereoMode stereo, float gain) {
    if (FILE *f = fopen(wavFilePath.c_str(), "rb")) {
        auto wav = new clunk::WavFile(f);
        wav->read();
        if (!wav->ok() || wav->_spec.channels != 2 || wav->_spec.sample_rate != 48000 || wav->_spec.format != clunk::AudioSpec::S16) {
            char buffer[MAX_PATH + const_strlen("File %s has invalid format.")];
            _snprintf_s(buffer, MAX_PATH + const_strlen("File %s has invalid format."), _TRUNCATE, "File %s has invalid format.", wavFilePath.c_str());
            MessageBoxA(0, buffer, "Task Force Arrowhead Radio", MB_OK);
        } else {
            construct(wav, stereo, gain);
        }
        fclose(f);
        delete wav;
    } else {
        log_string("Can't Open file " + wavFilePath, LogLevel_ERROR);
    }
}

void playbackWavStereo::construct(const short* samples, size_t sampleCount, uint8_t channels, stereoMode stereo, float gain) {
    sampleStore.assign(samples, samples + sampleCount * 2);

    //Behaviour of this code changed as you can see, we already copy samples into sampleStore, so we don't need to copy anything. We just need to set stuff to 0


    if (stereo == stereoMode::stereo) {
        //if (channels != 2) {
        //    const auto target = sampleStore.data();
        //    uint32_t posInTarget = 0;
        //    for (uint32_t q = 0; q < sampleCount*channels; q += channels) {
        //        target[posInTarget++] = samples[q];//copy left channel
        //        target[posInTarget++] = samples[q + 1];//copy right channel
        //    }
        //}
    } else if (stereo == stereoMode::leftOnly) {
        const auto target = sampleStore.data();
        uint32_t posInTarget = 0;
        for (uint32_t q = 0; q < sampleCount*channels; q += channels) {
            //target[posInTarget++] = samples[q];//only copy left channel
            posInTarget++; //leave left channel
            target[posInTarget++] = 0;//set right channel 0
            //posInTarget++;//leave right channel 0
        }
    } else if (stereo == stereoMode::rightOnly) {
        const auto target = sampleStore.data();
        uint32_t posInTarget = 0;
        for (uint32_t q = 0; q < sampleCount*channels; q += channels) {
            //posInTarget++;//leave left channel 0
            //target[posInTarget++] = samples[q + 1];//only copy right channel

            target[posInTarget++] = 0;//set left channel 0
            posInTarget++;//leave right channel 0

        }
    }
    SampleBuffer(sampleStore.data(), sampleCount, 2).applyGain(gain);
}

playbackWavStereo::playbackWavStereo(const short* samples, size_t sampleCount, uint8_t channels, stereoMode stereo, float gain /*= 1.0f*/) : currentPosition(0) {
    construct(samples, sampleCount, channels, stereo, gain);
}

playbackWavStereo::playbackWavStereo(clunk::WavFile* wavFile, stereoMode stereo, float gain /*= 1.0f*/) : currentPosition(0) {
    construct(wavFile, stereo, gain);
}

playbackWavStereo::playbackWavStereo(std::string wavFilePath, stereoMode stereo, float gain) : currentPosition(0) {
    construct(wavFilePath, stereo, gain);
}

playbackWavStereo::~playbackWavStereo() {
    sampleStore.clear();
}

size_t playbackWavStereo::getSamples(const short* &data) {
    data = std::min(sampleStore.data() + currentPosition, sampleStore.end()._Ptr);
    return sampleStore.end()._Ptr - (sampleStore.data() + currentPosition);
}

size_t playbackWavStereo::cleanSamples(size_t sampleCount) {
    const auto increase = std::min(sampleCount, sampleStore.size() - currentPosition);
    currentPosition += increase;
    if (isDone()) {
        sampleStore.clear();
        currentPosition = 0;
    }
    return increase;
}


playbackWavRaw::playbackWavRaw() :currentPosition(0) {
#ifdef DEBUG_PLAYBACK_TIMES
    creation = std::chrono::high_resolution_clock::now();
#endif
}

size_t playbackWavRaw::getSamples(const short*& data) {
#ifdef DEBUG_PLAYBACK_TIMES
    std::call_once(flag1, [this]() {
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - creation).count();
        log_string("raw use " + std::to_string(duration), LogLevel_WARNING);
    });
#endif
#ifndef isCI 
    if (sampleStore.data() + currentPosition >= sampleStore.end()._Ptr) {
        std::stringstream str;
        str << "playbackWavRaw::getSamples tried read beyond end!! " << sampleStore.size() << currentPosition;
        str << "offs " << sampleStore.data() + currentPosition << sampleStore.end()._Ptr;
        Logger::log(LoggerTypes::teamspeakClientlog, str.str(), LogLevel_WARNING);
    }
#endif

    data = std::min(sampleStore.data() + currentPosition, sampleStore.end()._Ptr);
    return sampleStore.end()._Ptr - (sampleStore.data() + currentPosition);
}

size_t playbackWavRaw::cleanSamples(size_t sampleCount) {
    const auto increase = std::min(sampleCount, sampleStore.size() - currentPosition);
    sampleStore.erase(sampleStore.begin(), sampleStore.begin() + increase);
    // currentPosition += increase;
    if (isDone()) {
        sampleStore.clear();
        currentPosition = 0;
    }
    return increase;
}

void playbackWavRaw::appendSamples(const short* samples, size_t sampleCount, uint8_t channels) {
    sampleStore.reserve(sampleStore.size() + (sampleCount * 2));
    const auto previousEnd = &(*sampleStore.insert(sampleStore.end(), samples, samples + (sampleCount * 2)));
    //Even if channelcount is not 2 we still want to set the new vector size
    //If its not stereo we overwrite the data again with the proper 2 channel stuff
    if (channels != 2) {
        uint32_t posInTarget = 0;
        for (uint32_t q = 0; q < sampleCount; q += channels) {
            previousEnd[posInTarget++] = samples[q];//copy left channel
            previousEnd[posInTarget++] = samples[q + 1];//copy right channel
        }
    }
}

playbackWavProcessing::playbackWavProcessing(const short* samples, size_t sampleCount, int channels, std::vector<std::function<void(SampleBuffer&)>> processors)
    : currentPosition(0), processingDone(false), myThread(nullptr) {
    functors = processors;
    const auto previousEnd = &(*sampleStore.insert(sampleStore.end(), samples, samples + (sampleCount * 2)));
    //Even if channelcount is not 2 we still want to set the new vector size
    //If its not stereo we overwrite the data again with the proper 2 channel stuff
    if (channels != 2) {
        size_t posInTarget = 0;
        for (size_t q = 0; q < sampleCount; q += channels) {
            previousEnd[posInTarget++] = samples[q];//copy left channel
            previousEnd[posInTarget++] = samples[q + 1];//copy right channel
        }
    }
#ifdef DEBUG_PLAYBACK_TIMES
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#endif
    myThread = new std::thread([this, sampleCount]() {
        ProfileFunctionN("process audio functors");
        SampleBuffer buf(sampleStore.data(), sampleCount, 2);
        for (auto& it : functors) {
            it(buf);
        }
        processingDone = true;
    });
#ifdef DEBUG_PLAYBACK_TIMES
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    creation = t2;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    log_string("processing init " + std::to_string(duration), LogLevel_WARNING);
#endif
}

size_t playbackWavProcessing::getSamples(const short*& data) {
    if (!processingDone) {
#ifdef DEBUG_PLAYBACK_TIMES
        //This was used to diagnose the time myThread->join() would block if it wasn't done yet
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#endif
        //when someone implements waiting again consider next comment
        //Add possibility to wait instead of skipping (add mutex lock so we actually wait for the thread to end) 
        //myThread->join(); //if thread doesnt exist here something is seriously wrong.. Should check for nullptr
        //delete myThread;
#ifdef DEBUG_PLAYBACK_TIMES
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        log_string("processing skip " + std::to_string(duration), LogLevel_WARNING);
#endif
        return 0;
    }
#ifdef DEBUG_PLAYBACK_TIMES
    std::call_once(flag1, [this]() {
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - creation).count();
        log_string("processing use " + std::to_string(duration), LogLevel_WARNING);
    });
#endif
    data = std::min(sampleStore.data() + currentPosition, sampleStore.end()._Ptr);
    return sampleStore.end()._Ptr - (sampleStore.data() + currentPosition);
}

size_t playbackWavProcessing::cleanSamples(size_t sampleCount) {
    const auto increase = std::min(sampleCount, sampleStore.size() - currentPosition);
    currentPosition += increase;
    if (isDone()) {
        sampleStore.clear();
        currentPosition = 0;
    }
    return increase;
}

bool playbackWavProcessing::samplesReady() {
    return true;//implement threading
}
