/*
 * GpioController.cpp
 *
 *  Created on: May 17, 2018
 *      Author: bensherman
 */

// System Includes
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <cstdint>
#include <fstream>
#include <cstring>

// Project Includes
#include "GpioController.hpp"

// Static initialization
const char * const GpioController::EXPORT_PIN_PATH = "/sys/class/gpio/export";
const char * const GpioController::UNEXPORT_PIN_PATH = "/sys/class/gpio/unexport";
const char * const GpioController::PIN_DIRECTION_PATH_FORMAT = "/sys/class/gpio/gpio%u/direction";
const char * const GpioController::PIN_VALUE_PATH_FORMAT = "/sys/class/gpio/gpio%u/value";
const std::string GpioController::OUTPUT_DIRECTION = "out";

GpioController::GpioController(const NamedPinMap& pins, bool initializeAllPinsToOutputLow): pins(pins)
{
    if (initializeAllPinsToOutputLow)
    {
        exportAllPinsAndSetToOutputLow();
    }
}

GpioController GpioController::Builder::build()
{
    GpioController controller(pins, initializeAllPinsToOutputLow);
    return controller;
}

/**
 * Adds a mapping from name->pinNum to this builder. If a pin already has
 * been added with the same name as the name parameter, a std::invalid_argument
 * is thrown.
 */
GpioController::Builder& GpioController::Builder::withNamedPin(const std::string& name, const uint16_t pinNum)
{
    if (pins.count(name) == 0)
    {
        this->pins.insert(std::make_pair(name, pinNum));
    }
    else
    {
        throw std::invalid_argument("Pin with name: " + name +
                " already provided; pin names must be unique");
    }
    return *this;
}

/**
 * Appends to updatableMessage a list "Name: PinNum" strings.
 */
void GpioController::getPinMapping(const std::string& UNUSED, std::string* updatableMessage)
{
    (void) UNUSED;

    updatableMessage->append("\n");

    for (const NamedPin& pin: pins)
    {
        updatableMessage->append(pin.first);
        updatableMessage->append(":");
        updatableMessage->append(std::to_string(pin.second));
        updatableMessage->append("\n");
    }
}

/**
 * Converts pinInfo into a PinState instance. pinInfo is expected to take the
 * form <name>:<output state (0 or 1)>. Any deviations from this format will
 * result in a std::invalid_argument being thrown.
 */
GpioController::PinState GpioController::convertPinStateCommandToPair(const std::string& pinInfo)
{
    auto equalsIdx = pinInfo.find(':');

    if (equalsIdx == std::string::npos ||
        equalsIdx == pinInfo.length()-1 ||
        pinInfo.find(':', equalsIdx+1) != std::string::npos)
    {
        throw std::invalid_argument("Syntax err: " + pinInfo + "; It must have"
                " the form \"pinName:0(off/low) or 1(on/high)\"");
    }

    std::string name = pinInfo.substr(0, equalsIdx);
    unsigned long val;
    try {
        val = std::stoul(pinInfo.substr(equalsIdx+1));
    }
    catch(std::invalid_argument& except)
    {
        throw std::invalid_argument("Syntax err: " + pinInfo + "; It must have"
                " the form \"pinName:0(off/low) or 1(on/high)\"");
    }

    OutputPinState pinState;

    if (val == 0)
    {
        pinState = OutputPinState::LOW;
    }
    else if (val == 1)
    {
        pinState = OutputPinState::HIGH;
    }
    else
    {
        throw std::invalid_argument("Syntax err: " + pinInfo + "; It must have"
                " the form \"pinName:0(off/low) or 1(on/high)\"");
    }

    return PinState(name, pinState);
}

/**
 * Uses pinInfo, which isexpected to take the form
 * <name>:<output state (0 or 1)>, to set the output state for the pin.
 * Any deviations from this format will result in a std::invalid_argument being thrown.
 */
void GpioController::setPinOutputState(const std::string& pinInfo, std::string* updatableMessage)
{
    (void) updatableMessage;

    PinState pinState = convertPinStateCommandToPair(pinInfo);

    auto pinIter = pins.find(pinState.first);

    if (pinIter == pins.end())
    {
        throw std::invalid_argument("Pin: " + pinState.first + " not registered");
    }

    std::string path = insertUInt8IntoString(PIN_VALUE_PATH_FORMAT,
            pinIter->second);

    writePinState(path, pinState.second);
}

/**
 * Writes the value, converted to a string, to the file located at filePath.
 */
void GpioController::writeUInt8ToFile(const std::string& filePath, uint8_t value)
{
    writeToFile(filePath, std::to_string(value));
}

/**
 * Writes value to the file located at filePath.
 */
void GpioController::writeToFile(const std::string& filePath, const std::string& value)
{
    std::cout << "Writing: " << value << " to: " << filePath << std::endl;

    std::ofstream outputFileStream(filePath.c_str());

    outputFileStream << value;
}

/**
 * Writes outputState (internally a 0 or 1) to the file located at filePath.
 */
void GpioController::writePinState(const std::string& filePath, OutputPinState outputState)
{
    writeToFile(filePath, std::to_string(static_cast<uint8_t>(outputState)));
}

/**
 * Expects formatString to have a single "%u%" in it, and replaces that format
 * character with the the num parameter, returning the new string.
 */
std::string GpioController::insertUInt8IntoString(const char * formatString, uint8_t num)
{
    // uint8_t can be at most 3 chars, extra char for null term
    size_t formattedLength = std::strlen(formatString) + 4;
    char formatted[formattedLength] = {};

    std::sprintf(formatted, formatString, num);

    return std::string(formatted);

}

/**
 * Sets all pins stored in pins mapping to output mode with value low.
 */
void GpioController::setAllToOutputLow(const std::string& UNUSED_IN,
        std::string* updatableMessage)
{
    (void) UNUSED_IN;

    *updatableMessage += "\n";

    for (auto namedPin : pins)
    {
        std::string path = insertUInt8IntoString(PIN_VALUE_PATH_FORMAT,
                namedPin.second);
        writePinState(path, OutputPinState::LOW);
        *updatableMessage += "Pin: ";
        *updatableMessage += namedPin.first;
        *updatableMessage += " low\n";
    }
}

/**
 * Exports all pins in the pins mapping and sets them to output mode
 * with value low.
 */
void GpioController::exportAllPinsAndSetToOutputLow()
{
    for (auto namedPin : pins)
    {
        // Export pin
        writeUInt8ToFile(EXPORT_PIN_PATH, namedPin.second);
        std::cout << "Exported pin: Name=" << namedPin.first << "; Num=" << namedPin.second << std::endl;

        // Set to output direction
        std::string directionPath = insertUInt8IntoString(PIN_DIRECTION_PATH_FORMAT,
                   namedPin.second);
        writeToFile(directionPath, OUTPUT_DIRECTION);
        std::cout << "Set pin: Name=" << namedPin.first << "; Num=" << namedPin.second <<
                " to direction:" << OUTPUT_DIRECTION << std::endl;

        // Set low
        std::string valuePath = insertUInt8IntoString(PIN_VALUE_PATH_FORMAT,
                   namedPin.second);
        writePinState(valuePath, OutputPinState::LOW);
        std::cout << "Set pin: Name=" << namedPin.first << "; Num=" << namedPin.second <<
                " to value LOW" << std::endl;

    }
}

GpioController::Builder& GpioController::Builder::withGpioInitializedToOutputLow()
{
    initializeAllPinsToOutputLow = true;
    return *this;
}
