/*
 * TcpServer.hpp
 *
 *  Created on: Sep 16, 2017
 *      Author: bensherman
 */

#ifndef NETWORK_TCPSERVER_HPP_
#define NETWORK_TCPSERVER_HPP_

// System Includes
#include <boost/asio.hpp>
#include <cstdint>
#include <list>
#include <vector>
#include <string>
#include <memory>

// Project Includes
#include "SocketWrapper.hpp"
#include "CommandParser.hpp"

// Forward declarations
class TcpServer;
class CommandParser;

// Using statements
using BoostTcp = boost::asio::ip::tcp;
using BoostIoService = boost::asio::io_service;
using TcpServerSharedPtr = std::shared_ptr<TcpServer>;

class TcpServer
{
public:

    class Builder
    {
    public:
        Builder() : port(DEFAULT_PORT), usePassword(false),
                             password(DEFAULT_PASSWORD) {};

        Builder& withPort(uint16_t port);
        Builder& withPassword(const std::string& password);
        Builder& withoutPassword();
        TcpServerSharedPtr build() const;

    private:
        uint16_t port;
        bool usePassword;
        std::string password;
    };

    void informAllClientsOfStateChange();
    void run(CommandParser& parser);
    void terminate();

    // Interpreter-executable commands
    void getAddressInfoHandler(const std::string& UNUSED, std::string* clientReturnableInfo);
    void getClientsInfoHandler(const std::string& UNUSED, std::string* clientReturnableInfo);

private:
    TcpServer(const uint16_t port=DEFAULT_PORT, const char * const password=DEFAULT_PASSWORD);

    // General info retrieval and print functions
    std::string getServerInfo();
    std::vector<std::string> getLocalIpAddresses();

    // Thread to be executed for each client
    void connectionHandler(SocketWrapper& sockWrap, CommandParser& parser);

    // Locked mutators for socketWrappersInUse
    SocketWrapper& appendToSocketWrappersInUse(TcpSocketSharedPtr& newSocket);
    void removeFromSocketWrappersInUse(const SocketWrapper& sockWrapper);

    // Authentication of clients which are trying to connect
    bool authenticate(SocketWrapper& socketWrap, BoostStreamBuff& socketReadStreamBuff);

    // The port which this server is associated with
    uint16_t port;

    // Whether or not a password is to be used for this server
    const bool requirePassword;

    // The password for this server
    const std::string password;

    BoostIoService ioService;

    BoostTcp::acceptor acceptor;

    // List of sockets and associated mutexes
    std::list<SocketWrapper> socketWrappersInUse;

    // Used to synchronize addition and deletion operations
    // on the socketWrappersInUse vector
    std::mutex socketWrappersInUseMutex;

    static const std::string UPDATED_PARAMETERS_AVAILABLE_STRING_PREFIX;
    static const std::string REQUESTING_PASSWORD_STRING_PREFIX;
    static const std::string END_OF_RESPONSE_STRING;
    static const std::string SOCKET_READ_UNTIL_END_SPECIFIER;
    static const uint16_t DEFAULT_PORT = 2018;
    static const char * const DEFAULT_PASSWORD;
    static const std::string AUTH_SUCCESSFUL_STRING;
    static const std::string AUTH_FAILED_STRING;

};

#endif /* NETWORK_TCPSERVER_HPP_ */
