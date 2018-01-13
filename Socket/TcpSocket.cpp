#include "TcpSocket.hpp"

namespace Network {
	namespace TSocket {
		TcpSocket::TcpSocket(fd_set *read, fd_set *write, const Type &type)
				: ASocket(read, write, type) {
		}

		TcpSocket::TcpSocket(fd_set *read, fd_set *write, const unsigned int socket)
				: ASocket(read, write, socket) {
#ifdef _WIN32
            WSADATA  data;
            int result;

            result = WSAStartup(MAKEWORD(2, 2), &data);
            if (result != 0)
                throw NetworkException("WSAStartu failed\n");
#endif // _WIN32s
		}

		TcpSocket::~TcpSocket() {
#ifdef _WIN32
			WSACleanup();
#endif // _WIN32
		}

		void TcpSocket::sPrepare(const unsigned int Millisecond, const std::string &ip, const unsigned int port) {
			if (mType == Type::Client) {
				sInit(Millisecond, ip, port);

				Logger::GetInstance().LogLine("||               *-* Connect client                            ||");
				sConnect();
			} else {
				sInit(Millisecond, ip, port);
				sBind();
				sListen(5);
			}
		}

		void TcpSocket::sInit(const unsigned int Millisecond, const std::string &ip, const unsigned int port) throw() {
			struct protoent *proto;
#ifdef _WIN32
            WSADATA  data;
            int result;

            result = WSAStartup(MAKEWORD(2, 2), &data);
            if (result != 0)
                throw NetworkException("WSAStartu failed\n");
#endif // _WIN32

			mTime.tv_sec = 0;
			mTime.tv_usec = Millisecond * 1000;

			Logger::GetInstance().LogLine("||               *-* Protobyname                               ||");
			if ((proto = getprotobyname("TCP")) == NULL)
				throw NetworkException("getprotobyname failed\n");

			Logger::GetInstance().LogLine("||               *-* Create socket                             ||");
			if ((mSocketId = socket(AF_INET, SOCK_STREAM, proto->p_proto)) < 0)
				throw NetworkException("Creat socket failed\n");

			if (mType == Type::Client) {
				char opt;
				struct hostent *host;

				Logger::GetInstance().LogLine("||               *-* Set sockopt                               ||");
				if (setsockopt(mSocketId, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
					throw NetworkException("Setsockopt failed\n");

				Logger::GetInstance().LogLine("||               *-* Gethostbyname                             ||");
				struct in_addr in;
				inet_pton(AF_INET, ip.c_str(), &in);
				if ((host = gethostbyaddr((char *) &in, sizeof in, AF_INET)) == NULL)
					throw NetworkException("Unknown host\n");

				mAddr.sin_family = AF_INET;
				mAddr.sin_port = htons(port);
				mAddr.sin_addr = in;
			} else {
				mAddr.sin_family = AF_INET;
				mAddr.sin_port = htons(port);
				mAddr.sin_addr.s_addr = INADDR_ANY;
			}
		}

		void TcpSocket::sListen(const unsigned int nb) throw() {
			if (listen(mSocketId, nb) == -1)
				throw NetworkException("Socket listen failed\n");
		}

		unsigned int TcpSocket::sAccept() const throw() {
			unsigned int fd;
			struct sockaddr_in addr;
			socklen_t len;

			len = sizeof(addr);
			if ((fd = accept(mSocketId, (struct sockaddr *) &addr, &len)) == -1)
				throw NetworkException("Socket accept failed");
			return fd;
		}

		void TcpSocket::sConnect() const throw() {
			int value;
			if ((value = connect(mSocketId, (struct sockaddr *) &mAddr, sizeof(struct sockaddr_in))) < 0)
				throw NetworkException("Socket connect failed " + std::to_string(value) + " \n");
		}

		int TcpSocket::sSend(const Packet &buff, struct sockaddr_in *addr) const
        {
            int header = htonl(buff.getBuff().length());
            send(mSocketId, (char *)&header, 4, 0);
            return static_cast<int>(send(mSocketId, buff.getBuff().c_str(), buff.getBuff().length(), 0));
        }

        int TcpSocket::sRecv(Packet &buff, struct sockaddr_in *addr)
        {
            int size;
            recv(mSocketId, (char*)&size, 4, 0);
            int sizeMsg = ntohl(size);

            if (sizeMsg >= 0)
            {
                char *buffrd = new char[sizeMsg + 1];
                buffrd[sizeMsg] = 0;
                int ret = static_cast<int>(recv(mSocketId, buffrd, sizeMsg, 0));
                buff.append(std::string(buffrd));
                return ret;
            }
            return 0;
        }
	}
}