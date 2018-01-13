//
// Created by Gaëtan Therou on 18-01-13.
//

#include "AClient.hpp"

namespace Network {
    AClient::AClient(const std::string &ip, const unsigned int port, const std::string &protocol)
            : mProtocol(protocol) {
        Logger::GetInstance().LogLine("=================================================================");
        Logger::GetInstance().LogLine(
                "||               *-* Creat client " + mProtocol + "                          ||");

        Logger::GetInstance().LogLine("||               *-* socket                                    ||");
        if (protocol == "TCP")
            mSocket = new TSocket::TcpSocket(&mFdRead, &mFdWrite, TSocket::Type::Client);
        else if (protocol == "UDP")
            mSocket = new TSocket::UdpSocket(&mFdRead, &mFdWrite, TSocket::Type::Client);

        Logger::GetInstance().LogLine("||               *-* Init client                               ||");

        if (ip.find(' ') != std::string::npos)
            mSocket->sPrepare(10, ip.substr(ip.find(' ') + 1), port);
        else
            mSocket->sPrepare(10, ip, port);

        Logger::GetInstance().LogLine("||               *-* Client initialize OK                      ||");
        Logger::GetInstance().LogLine("=================================================================");
    }

    AClient::~AClient() {
        delete mSocket;
    }

    void AClient::run() {
        mSocket->setFdZero();
        mSocket->setFdSet(&mFdRead);
        if (mSocket->sSelect() > 0) {
            checkReadWrite();
        }
    }

    void AClient::write(const TSocket::Packet &buff) {
        /*
        *	buffer write stock, and write if the socket is disponible.
        */
        Logger::GetInstance().LogLine("		-(" + mProtocol + ") write " + buff.getBuff() + ".");
        mSocket->setFdSet(&mFdWrite);
        mSocket->sSend(buff, nullptr);
    }

    void AClient::checkReadWrite()  {
        /*
        *	Check if socket recv message
        */
        if (mSocket->setFdIsset(&mFdRead)) {
            TSocket::Packet buf;
            if (mSocket->sRecv(buf, nullptr) == 0)
                throw NetworkException("Server KO");

            /*
            *	Call surcharge methode
            */
            recvMessage(buf.str());
        }
    }
}
