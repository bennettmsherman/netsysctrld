/*
 * SystemUtils.cpp
 *
 *  Created on: Sep 30, 2017
 *      Author: bensherman
 */

// System Includes
#include <cstring>
#include <exception>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <system_error>

// Project Includes
#include "SystemUtils.hpp"

// Static Initialization

// %s refers to the audio control (audioControlName), and %u is the percentage
// to set
const char SystemUtils::VOLUME_SETTER_FORMAT[] = "amixer set %s %u%% -M";

// Use "Master" as the default control
const std::string SystemUtils::DEFAULT_AUDIO_CONTROL_NAME = "Master";

// The user specifies "VOLUME=newvol" to set the volume
const std::string SystemUtils::VOLUME_SETTER_COMMAND = "VOLUME";

/**
 * Sets the system volume to the percentage specified by the volume parameter.
 * std::system_error is thrown if an error occurred.
 * systemVolume member is updated if the volume is set properly.
 * Concurrent access to this function is protected using the volumeSetterMutex.
 */
void SystemUtils::setVolume(const uint8_t vol)
{
    std::lock_guard<std::mutex> volumeSetterGuard{volumeSetterMutex};

    if (vol > 100)
    {
        throw std::out_of_range("System volume cannot be greater than 100%");
    }

    // The 3 accounts for the int value to be substituted
    static char * volumeSetCommandBuff = new char[sizeof(VOLUME_SETTER_FORMAT) + audioControlName.length() + 3];

    // Substitute the new volume into the volume set command
    (void) std::sprintf(volumeSetCommandBuff, VOLUME_SETTER_FORMAT, audioControlName.c_str(), vol);

    std::cout << "Executing volume set command: " << volumeSetCommandBuff << std::endl;

    int systemRetVal = system(volumeSetCommandBuff);

    if (systemRetVal != 0)
    {
        throw std::system_error(systemRetVal, std::generic_category(),
                "Error when setting system volume using system()");
    }

    // Update the sysVol variable to reflect the new system volume settings
    systemVolume = vol;
}

/**
 * Wrapper for setVolume(). Intended to be called by the command interpreter.
 * The vol param should be a percentage representing the new system volume.
 * After the volume is set, if there were no errors, all clients are informed
 * of the state change. If there was an error, it's propagated to the caller.
 */
void SystemUtils::setVolumeCommandHandler(const std::string& vol, std::string* updatableMessage)
{
    (void) updatableMessage;

    try
    {
        setVolume(static_cast<uint8_t>(std::stoul(vol)));
    }
    // Allow the exception to be propagated up while ensuring that the clients
    // aren't updated in this error case
    catch (const std::system_error& err)
    {
        throw err;
    }
}

/*
 * Constructs a SystemUtils
 * Sets the audio control name to be used when changing the output volume
 * through the daemon.
 * The systemVolume is initialized to 0, but isn't accurate until the user
 * first sets it.
 */
SystemUtils::SystemUtils(const std::string& audioControlName):
        audioControlName(audioControlName), systemVolume(0)
{

}

/**
 * Gets the current system volume
 */
uint8_t SystemUtils::getVolume()
{
    return systemVolume;
}

SystemUtils::Builder& SystemUtils::Builder::withAudioControlName(const char* audioControlName)
{
    if (audioControlName == nullptr)
    {
        throw std::invalid_argument("ERROR! A null pointer has been provided"
                "as the audio control name!");
    }

    this->audioControlName = std::string(audioControlName);
    return *this;
}

SystemUtilsSharedPtr SystemUtils::Builder::build()
{
    return SystemUtilsSharedPtr(new SystemUtils(audioControlName));
}
