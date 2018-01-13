#pragma once

#ifdef TARGET_OS_WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif // __linux__

#include <string>
#include "../Exception/NetworkException.hpp"
#include "Packet.hpp"

namespace Network {
	namespace TSocket {
		enum Type {
			Client,
			Server
		};

		class ASocket {
		public:
			ASocket(fd_set *pRead, fd_set *pWrite, const Type &pType = Type::Client);
			ASocket(fd_set *pRead, fd_set *pWrite, unsigned int pSocket);
			virtual ~ASocket() {};

		public:
			int getSockId() const;
            void sClose();
			int sSelect();
			void sBind() const;

		public:
			/*	fd set*/
			void setFdZero();
			void setFdZero(fd_set *pZero);
			void setFdSet(fd_set *pSet);
			int setFdIsset(fd_set *pIsset);

		public:
			virtual void sPrepare(unsigned int Millisecond, const std::string &ip, unsigned int port) = 0;
			virtual void sInit(unsigned int Millisecond, const std::string &ip, unsigned int port) throw() = 0;
			virtual void sListen(unsigned int nb) = 0;
			virtual unsigned int sAccept() const throw() = 0;
			virtual void sConnect() const throw() = 0;
            virtual int sSend(const Packet &buff, struct sockaddr_in *addr) const = 0;
            virtual int sRecv(Packet & buff, struct sockaddr_in *addr) = 0;

		protected:
			Type mType;
			struct timeval mTime;
			struct sockaddr_in mAddr;
			int mSocketId;
			fd_set *mFdRead;
			fd_set *mFdWrite;
		};
	}
}
