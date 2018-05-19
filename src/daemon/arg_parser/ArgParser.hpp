/*
 * ArgParser.hpp
 *
 *  Created on: Dec 23, 2017
 *      Author: bensherman
 */

#ifndef DAEMON_ARG_PARSER_ARGPARSER_HPP_
#define DAEMON_ARG_PARSER_ARGPARSER_HPP_

// System Includes
#include <cstdint>
#include <string>

// Project Includes
#include "ArgParserFunction.hpp"
#include "TcpServer.hpp"
#include "GpioController.hpp"

class ArgParser
{
public:
    void parse(int argc, const char *argv[]);

    // Get generated instances
    GpioController buildGpioController();
    TcpServerSharedPtr buildTcpServer();
    SystemUtilsSharedPtr buildSystemUtils();

private:

    /**
     * NONSTATIC
     */
    TcpServer::Builder tcpServerBuilder;
    GpioController::Builder gpioControllerBuilder;
    SystemUtils::Builder systemUtilsBuilder;


    // Functions which handle their associated arguments
    void setAudioControlName(const std::string& audioControlName);
    void setAudioOutputDevice(const std::string& audioOutputDevice);
    void setServerPassword(const std::string& password);
    void setServerPort(const std::string& portAsStr);
    void addGpioPin(const std::string& namedPin);
    void printHelp(const std::string& UNUSED);
    void initGpioToOutputLow(const std::string& UNUSED);

    void executeArgParserFunction(const ArgParserFunction* argParserFunc,
            const std::string& argAsStr, int& argvIdx, const int argc,
            const char *argv[]);

    /**
     * STATIC
     */
    /* Describes the type of a detected argument.
     * SHORT refers to arguments preceded by "-"
     * EXTENDED refers to arguments preceded by "--"
     * PARAM refers to parameters to an argument
     * INVALID indicates that the character/string is incorrect
     * In "--a 10 -p pass", "--a" = EXTENDED, "10" = PARAM, "-p" = SHORT, and
     * "pass" = PARAM
     */
    enum class ArgType {SHORT, EXTENDED, PARAM, INVALID};

    static ArgType determineArgType(const std::string& strToCheck);
    static void incrementArgIdx(const ArgParserFunction * const func, int& argIdx);

    static const ArgParserFunction* findByShortSpecifier(const std::string& shortSpecifierWithIdentifier);
    static const ArgParserFunction* findByExtendedSpecifier(const std::string& extendedSpecifierWithIdentifier);
    static const ArgParserFunction* getArgParserFunctionFromArg(const std::string& singlarArg);

    // Class variables
    static const ArgParserFunction ARG_PARSER_FUNCTIONS[];
    static const size_t ARG_PARSER_FUNCTIONS_LIST_LENGTH;

};

#endif /* DAEMON_ARG_PARSER_ARGPARSER_HPP_ */
