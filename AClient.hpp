#pragma once

#include "Socket/TcpSocket.hpp"
#include "Socket/UdpSocket.hpp"
#include "Socket/Packet.hpp"
#include "Exception/NetworkException.hpp"
#include "Logger/Logger.hpp"

namespace Network {
    class AClient {
    public:
        /*
        *	@param "ip"			->  ip for connect client to the server
        *	@param "port"		->  port to connect at the server
        *	@param "protocol"	->	by default it's set at "TCP"
        */
        AClient(const std::string &ip, unsigned int port, const std::string &protocol = "TCP");

        ~AClient();

    public:
        /*
        *	"recvMessage" it's call by the client after recv message.
        *
        *	@param "msg"  -> it's a recv message.
        */
        virtual void recvMessage(const std::string &msg) = 0;

    public:
        /*
        *	"run" is a main server function,
        *	call this function in main while.
        */
        virtual void run();

        /*
        *	"write" add message to buff write.
        *
        *	@param "buff"    -> message at send.
        */
        virtual void write(const TSocket::Packet &buff);

    private:
        void checkReadWrite();

    private:
        std::string mProtocol;
        TSocket::ASocket *mSocket;
        fd_set mFdRead;
        fd_set mFdWrite;
    };
}