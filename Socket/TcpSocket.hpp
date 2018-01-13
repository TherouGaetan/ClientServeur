#pragma once

#ifdef TARGET_OS_UNIX
#include <netdb.h>
#endif

#include "ASocket.hpp"
#include "../Exception/NetworkException.hpp"
#include "../Logger/Logger.hpp"

namespace Network {
    namespace TSocket {
        class TcpSocket : public ASocket {
        public:
            TcpSocket(fd_set *read, fd_set *write, const Type &type = Type::Client);
            TcpSocket(fd_set *read, fd_set *write, unsigned int socket);
            virtual ~TcpSocket();

        public:
            void sPrepare(unsigned int Millisecond, const std::string &ip, unsigned int port) override;
            void sInit(unsigned int Millisecond, const std::string &ip, unsigned int port) throw() override;
            void sListen(unsigned int nb) throw() override;
            unsigned int sAccept() const throw() override;
            void sConnect() const throw() override;
            int sSend(const Packet &buff, struct sockaddr_in *addr) const override;
            int sRecv(Packet & buff, struct sockaddr_in *addr) override;
        };
    }
}
