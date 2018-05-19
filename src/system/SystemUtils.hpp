/*
 * SystemUtils.hpp
 *
 *  Created on: Sep 30, 2017
 *      Author: bensherman
 */

#ifndef SYSTEM_SYSTEMUTILS_HPP_
#define SYSTEM_SYSTEMUTILS_HPP_

// System Includes
#include <atomic>
#include <mutex>
#include <string>
#include <stdint.h>
#include <memory>

// Project Includes
// <none>


class SystemUtils;
using SystemUtilsSharedPtr = std::shared_ptr<SystemUtils>;

class SystemUtils
{
public:
    void setVolume(const uint8_t vol);

    uint8_t getVolume();

    // Meant only to be called by the command parser
    void setVolumeCommandHandler(const std::string& vol, std::string* updatableMessage);

    // The user specifies "VOLUME=newvol" to set the volume
    static const std::string VOLUME_SETTER_COMMAND;

    class Builder
    {
    public:
        Builder() {};
        Builder& withAudioControlName(const char* audioControlName);
        SystemUtilsSharedPtr build();
    private:
        std::string audioControlName = DEFAULT_AUDIO_CONTROL_NAME;
    };

private:

    SystemUtils(const std::string& audioControlName = DEFAULT_AUDIO_CONTROL_NAME);

    static const char VOLUME_SETTER_FORMAT[];

    static const std::string DEFAULT_AUDIO_CONTROL_NAME;

    // The name of the audio control to be used when changing the volume
    const std::string audioControlName;

    // The system volume. Uninitialized to a valid value until the first call
    // to setVolume()
    std::atomic<uint8_t> systemVolume;

    // Used to prevent clients from trying to change the volume simultaneously
    std::mutex volumeSetterMutex;

};

#endif /* SYSTEM_SYSTEMUTILS_HPP_ */
