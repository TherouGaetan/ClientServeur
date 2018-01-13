#include "UdpSocket.hpp"

namespace Network {
	namespace TSocket {
		UdpSocket::UdpSocket(fd_set *pRead, fd_set *pWrite, const Type &pType)
				: ASocket(pRead, pWrite, pType) {
		}

		UdpSocket::UdpSocket(fd_set *pRead, fd_set *pWrite, unsigned int pSocket)
				: ASocket(pRead, pWrite, pSocket) {

		}

		void UdpSocket::sPrepare(unsigned int Millisecond, const std::string &ip, unsigned int port) {
			if (mType == Type::Client)
				sInit(Millisecond, ip, port);
			else {
				sInit(Millisecond, ip, port);
				sBind();
			}
		}

		void UdpSocket::sInit(unsigned int Millisecond, const std::string &ip, unsigned int port) throw() {
			struct protoent *proto;
#ifdef _WIN32
            WSADATA  data;
            int result;

            result = WSAStartup(MAKEWORD(2, 2), &data);
            if (result != 0)
                throw NetworkException("WSAStartu failed\n");
#endif // _WIN32

			mTime.tv_sec = 0;
			mTime.tv_usec = Millisecond * 50;

			Logger::GetInstance().LogLine("||               *-* Protobyname                               ||");
			if ((proto = getprotobyname("UDP")) == NULL)
				throw NetworkException("getprotobyname failed\n");

			Logger::GetInstance().LogLine("||               *-* Create socket                             ||");
			if ((mSocketId = socket(AF_INET, SOCK_DGRAM, proto->p_proto)) < 0)
				throw NetworkException("Creat socket failed\n");

			if (mType == Type::Client) {
				char opt;
				struct hostent *host;

				Logger::GetInstance().LogLine("||               *-* set sockopt                               ||");
				if (setsockopt(mSocketId, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
					throw NetworkException("Setsockopt failed\n");

				Logger::GetInstance().LogLine("||               *-* Gethostbyname                             ||");
				struct in_addr in;
				inet_pton(AF_INET, ip.c_str(), &in);
				if ((host = gethostbyaddr((char *) &in, sizeof in, AF_INET)) == NULL)
					throw NetworkException("Unknown host\n");

				mAddr.sin_family = AF_INET;
				mAddr.sin_port = htons(port);
				mAddr.sin_addr = *(struct in_addr *) host->h_addr;
			} else {
				mAddr.sin_family = AF_INET;
				mAddr.sin_port = htons(port);
				mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			}
		}

		void UdpSocket::sListen(unsigned int nb) throw() {
			// ok
		}

		unsigned int UdpSocket::sAccept() const throw() {
			// ok
			return 0;
		}

		void UdpSocket::sConnect() const throw() {
			// ok
		}

		int UdpSocket::sSend(const Packet &buff, struct sockaddr_in *addr) const
        {
            int sizeAddr = sizeof(struct sockaddr_in);
            return static_cast<int>(sendto(mSocketId, buff.getBuff().c_str(), buff.getBuff().length(), 0, (struct sockaddr *)((mType == Type::Client) ? &mAddr : addr), sizeAddr));
        }

        int UdpSocket::sRecv(Packet & buff, struct sockaddr_in *addr)
        {
			socklen_t sizeAddr = sizeof(struct sockaddr_in);
            char *buffrd = new char[1024];

            int ret = static_cast<int>(recvfrom(mSocketId, buffrd, 1024, 0, (struct sockaddr *)((mType == Type::Client) ? &mAddr : addr), &sizeAddr));
            buffrd[ret] = 0;
            buff.append(std::string(buffrd));
            return ret;
        }
	}
}
