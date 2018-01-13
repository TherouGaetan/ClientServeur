#pragma once

#include <list>
#include <utility>
#include <vector>

#include "Socket/ASocket.hpp"
#include "Socket/TcpSocket.hpp"
#include "Socket/UdpSocket.hpp"

#include "Logger/Logger.hpp"
#include "Exception/NetworkException.hpp"

namespace Network {
    class AServer {
    public:
        AServer(int pPort = 2500, const std::string &pProtocol = "TCP");

        ~AServer();

    public:
        /*
        *	"newUser" is call by the server, after recv new connection.
        *
        *	@param "pSock"		-> new socket create.
        */
        virtual void newUser(TSocket::ASocket *pSock) = 0;

        /*
        *	"decoUser" is call by the server, after recv disconnection.
        *
        *	@param "pIdSock"		-> id socket destroy.
        */
        virtual void decoUser(int pIdSock) = 0;

        /*
        *	"serverRead" is call by the server, after recv message.
        *
        *	@param "pSock"		-> socket to write.
        *	@param "pBuff"		-> contain recv message to sock.
        */
        virtual void serverRead(TSocket::ASocket *pSock, const TSocket::Packet &pBuff) = 0;

    public:
        /*
        *	"run" is a main server function,
        *	call this function in main while.
        */
        virtual void run();

        /*
        *	"writeSocket" is call by AComServ for write in socket.
        *
        *	@param "pSock"  -> sock for write
        *	@param "pWrite" -> buff at write
        */
        virtual void writeSocket(TSocket::ASocket *pSock, const TSocket::Packet &pWrite);

    private:
        /*
        *	"initializeFd" prepare all socket connected for recv message.
        */
        void initializeFd();

        /*
        *	"checkReadWrite" check all socket connected for read and write.
        *
        *	@param "pSock" -> Socket test
        *
        *	He call function "serverRead" or "decoUser" to AComServ.
        */
        void checkReadWrite(TSocket::ASocket *pSock);

        /*
        *	"recvNewConnection" check socket server, he create new socket.
        *
        *	He call function "newUser" to AComServ.
        */
        void recvNewConnection();

    public:
        /*
        *	Accesseur
        */
        TSocket::ASocket *getServerSocket() const;

    private:
        void checkClient(struct sockaddr_in *pSin);

    protected:
        std::string mProtocol;
        /* "mSocket" contained all socket connect */
        std::list<std::pair<bool,
                TSocket::ASocket *>> mSocket;

        /* "mSockServ" is the server socket */
        TSocket::ASocket *mSockServ;
        fd_set mFdRead;
        fd_set mFdWrite;

        std::vector<struct sockaddr_in *> mClientUdp;
    };
}