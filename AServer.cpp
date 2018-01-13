//
// Created by Gaëtan Therou on 18-01-13.
//

#include "AServer.hpp"


namespace Network {
    AServer::AServer(int pPort, const std::string &pProtocol)
            : mProtocol(pProtocol) {
        Logger::GetInstance().LogLine("=================================================================");
        Logger::GetInstance().LogLine("||                         Creat server                        ||");

        Logger::GetInstance().LogLine("||                            socket                           ||");
        if (pProtocol == "TCP")
            mSockServ = new TSocket::TcpSocket(&mFdRead, &mFdWrite, TSocket::Type::Server);
        else if (pProtocol == "UDP")
            mSockServ = new TSocket::UdpSocket(&mFdRead, &mFdWrite, TSocket::Type::Server);

        Logger::GetInstance().LogLine("||                         Init client                         ||");

        mSockServ->sPrepare(5000, "10.18.208.11", pPort);

        Logger::GetInstance().LogLine("||                     Server initialize OK                    ||");
        Logger::GetInstance().LogLine("=================================================================");
    }

    AServer::~AServer()  {
        delete mSockServ;
    }

    void AServer::run()  {
        initializeFd();

        if (mSockServ->sSelect() > 0) {

            /*
            *	Check all socket connect, if he need read or write
            */
            if (mProtocol == "TCP") {
                /*
                *	Check socket server, for new user
                */
                recvNewConnection();
                for (std::list<std::pair<bool, TSocket::ASocket *> >::iterator it = mSocket.begin();
                     it != mSocket.end(); ++it) {
                    checkReadWrite((*it).second);
                }
            } else if (mProtocol == "UDP") {
                TSocket::Packet buf;
                struct sockaddr_in *sin = new struct sockaddr_in;
                mSockServ->sRecv(buf, sin);
                Logger::GetInstance().LogLine("\t-\tRecv message UDP " + buf.getBuff());
                checkClient(sin);
                /*Adding*/ serverRead(mSockServ, buf);
            }

        }
    }

    void AServer::writeSocket(TSocket::ASocket *pSock, const TSocket::Packet &pWrite)  {
        /*
        *	Initialize socket for write
        */
        pSock->setFdSet(&mFdWrite);
        /*
        *	Stock with std::pair, the socket and buff to write.
        */
        if (mProtocol == "TCP")
            pSock->sSend(pWrite, nullptr);
        else if (mProtocol == "UDP") {
            for (unsigned int i = 0; i < mClientUdp.size(); i++)
                pSock->sSend(pWrite, mClientUdp[i]);
        }
    }

    void AServer::initializeFd()  {
        /*
        *	Prepare fd_read and fd_write.
        */
        mSockServ->setFdZero(&mFdRead);
        mSockServ->setFdZero(&mFdWrite);

        /*
        *	Initialize all socket for read and write
        */
        mSockServ->setFdSet(&mFdRead);
        for (std::list<std::pair<bool, TSocket::ASocket *> >::iterator it = mSocket.begin();
             it != mSocket.end(); ++it)
            if ((*it).first)
                (*it).second->setFdSet(&mFdRead);
        for (std::list<std::pair<bool, TSocket::ASocket *> >::iterator it = mSocket.begin();
             it != mSocket.end(); ++it)
            if (!(*it).first) {
                it = mSocket.erase(it);
                it = mSocket.begin();
            }
    }

    void AServer::checkReadWrite(TSocket::ASocket *pSock)  {
        /*
        *	Check with FD_ISSET, if sock need read.
        */
        if (pSock->setFdIsset(&mFdRead)) {
            /*
            *	Recv return -1, when sock is "end of file"
            *
            *	So, if Recv return -1, user disconneted.
            */
            int ret;
            TSocket::Packet buf;
            if (mProtocol == "TCP")
                ret = pSock->sRecv(buf, nullptr);

            if (buf.str() != "") {
                serverRead(pSock, buf);
            }
            if (ret == 0) {
                int idSock = pSock->getSockId();
                for (std::list<std::pair<bool, TSocket::ASocket *> >::iterator it = mSocket.begin();
                     it != mSocket.end(); ++it) {
                    if ((*it).second == pSock)
                        (*it).first = false;
                }
                decoUser(idSock);
                Logger::GetInstance().LogLine(
                        "\t-\tUser with socket (" + std::to_string(idSock) + ") disconnect.");
            }
        }
    }

    void AServer::recvNewConnection()  {
        if (mSockServ->setFdIsset(&mFdRead)) {
            /*
            *	Create socket for new user/connection
            */
            if (mProtocol == "TCP") {
                TSocket::ASocket *socket = new TSocket::TcpSocket(&mFdRead, &mFdWrite, mSockServ->sAccept());

                socket->setFdZero();

                newUser(socket);
                mSocket.emplace_back(true, socket);
                Logger::GetInstance().LogLine("\t-\tNew connection.");
            }
        }
    }

    TSocket::ASocket* AServer::getServerSocket() const {
        return mSockServ;
    }

    void AServer::checkClient(struct sockaddr_in *pSin)  {
        bool present = false;
        for (unsigned int i = 0; i < mClientUdp.size(); i++) {
            if (pSin->sin_addr.s_addr == mClientUdp[i]->sin_addr.s_addr &&
                pSin->sin_port == mClientUdp[i]->sin_port)
                present = true;
        }
        if (!present)
            mClientUdp.push_back(pSin);
    }
}