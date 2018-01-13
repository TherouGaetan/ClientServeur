#pragma once

#ifdef TARGET_OS_UNIX
#include <netdb.h>
#endif

#include "ASocket.hpp"
#include "../Exception/NetworkException.hpp"
#include "../Logger/Logger.hpp"

namespace Network {
	namespace TSocket {
		class UdpSocket : public ASocket {
		public:
			UdpSocket(fd_set *pRead, fd_set *pWrite, const Type &pType = Type::Client);
			UdpSocket(fd_set *pRead, fd_set *pWrite, unsigned int pSocket);
			virtual ~UdpSocket() {};

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
