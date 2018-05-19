/*
 * GpioController.hpp
 *
 *  Created on: May 17, 2018
 *      Author: bensherman
 */

#ifndef GPIO_GPIOCONTROLLER_HPP_
#define GPIO_GPIOCONTROLLER_HPP_

// System Includes
#include <unordered_map>
#include <string>
#include <cstdint>
#include <utility>

// Project Includes


// Foward declaration
class GpioController;
enum class OutputPinState;

// Typedefs
using NamedPinMap = std::unordered_map<std::string, const uint16_t>;
using NamedPin = std::pair<const std::string, const uint16_t>;

class GpioController
{
public:
    enum class OutputPinState
    {
        LOW = 0, HIGH = 1
    };

    // Used for constructing a GpioController instance
    class Builder
    {
    public:
        Builder() {};
        Builder& withNamedPin(const std::string& name, const uint16_t pinNum);
        Builder& withGpioInitializedToOutputLow();
        GpioController build();
    private:
        NamedPinMap pins;
        bool initializeAllPinsToOutputLow = false;
    };

    void getPinMapping(const std::string& UNUSED, std::string* updatableMessage);
    void setPinOutputState(const std::string& pinInfo, std::string* updatableMessage);
    void setAllToOutputLow(const std::string& UNUSED_IN, std::string* updatableMessage);

private:
    using PinState = std::pair<std::string, GpioController::OutputPinState>;
    GpioController(const NamedPinMap& pins, bool initializeAllPinsToOutputLow=false);

    // File IO
    static void writeToFile(const std::string& filePath, const std::string& value);
    static void writeUInt8ToFile(const std::string& filePath, uint8_t value);
    static void writePinState(const std::string& filePath, OutputPinState pinState);

    PinState convertPinStateCommandToPair(const std::string& pinInfo);

    static std::string insertUInt8IntoString(const char * formatString, uint8_t num);

    void exportAllPinsAndSetToOutputLow();

    static const char * const EXPORT_PIN_PATH;
    static const char * const  UNEXPORT_PIN_PATH;
    static const char * const  PIN_DIRECTION_PATH_FORMAT;
    static const char * const PIN_VALUE_PATH_FORMAT;

    static const std::string OUTPUT_DIRECTION;

    const NamedPinMap pins;
};



#endif /* GPIO_GPIOCONTROLLER_HPP_ */
