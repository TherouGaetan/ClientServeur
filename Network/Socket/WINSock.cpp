#include "WINSock.hpp"

namespace Socket
{
	WINSock::WINSock()
	{

	}

	WINSock::WINSock(const int fd)
	{
		_sock = fd;
	}

	WINSock::~WINSock()
	{
		WSACleanup();
	}

	bool WINSock::operator==(ISocket *socket)
	{
		return (this->getSock() == socket->getSock());
	}

	void WINSock::SKInit(std::string const &_ip, int port, std::string const &_protocol)
	{
		struct protoent *proto;
		WSADATA wsaData;
		int iResult;

		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			throw SocketException("WSAStartup failed\n");
		}
		_tv.tv_sec = 5;
		_tv.tv_usec = 5000000;
		if ((proto = getprotobyname(_protocol.c_str())) == NULL)
			throw SocketException("getprotobyname failed\n");
		_sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
		if (_sock < 0)
			throw SocketException("Socket creation failed\n");
		_server.sin_family = AF_INET;
		_server.sin_port = htons(port);
		_server.sin_addr.s_addr = INADDR_ANY;
	}

	void WINSock::SKBind() const
	{
		if (bind(_sock, (struct sockaddr *) &_server, sizeof(struct sockaddr_in)) == -1)
			throw SocketException("Sockect binding failed\n");
	}

	void WINSock::SKListen(int queueMax) const
	{
		if (listen(_sock, queueMax) == -1)
			throw SocketException("Socket listen failed\n");
	}

	int WINSock::SKAccept() const
	{
		int fd;
		struct sockaddr_in client;
		int len;

		len = sizeof(client);
		if ((fd = accept(_sock, (struct sockaddr *) &client, &len)) == -1)
			throw SocketException("Socket accept failed");
		return fd;
	}

	int WINSock::SKRecv(int fd, std::string &buff, size_t const size)
	{
		char * test = new char[size];
		memset(test, 0, size);
		int rd = recv(fd, test, size - 1, 0);
		if (rd <= 0)
			return -1;
		test[rd] = 0;
		buff = test;
		delete test;
		return rd;
	}

	int WINSock::SKRecv(std::string &buff, size_t const size)
	{
		char * test = new char[size];
		memset(test, 0, size);
		int rd = recv(_sock, test, size - 1, 0);
		if (rd <= 0)
			return -1;
		test[rd] = 0;
		buff = test;
		delete test;
		return rd;
	}

	void WINSock::SKClose(int fd) const
	{
		closesocket(fd);
	}

	void WINSock::SKClose()
	{
		closesocket(_sock);
	}

	void WINSock::SKConnect() const
	{
		if ((connect(_sock, (struct sockaddr *) &_server, sizeof(struct sockaddr_in))) < 0)
			throw SocketException("Socket connect failed\n");
	}

	int WINSock::SKSend(int fd, std::string const & str) const
	{
		return send(fd, str.c_str(), str.length(), 0);
	}

	int WINSock::SKSend(std::string const &str) const
	{
		return send(_sock, str.c_str(), str.length(), 0);
	}

	int WINSock::SKSelect(int sockTmp, fd_set * read, fd_set * write)
	{
		int ret;
		if ((ret = select(sockTmp + 1, read, write, NULL, &_tv)) == -1)
			throw SocketException("Socket select failed with\n");
		return ret;
	}

	int WINSock::SKSelect(fd_set * read, fd_set * write)
	{
		int ret;
		if ((ret = select(_sock + 1, read, write, NULL, &_tv)) == -1)
			throw SocketException("Socket select failed\n");
		return ret;
	}

	void WINSock::SKFD_ZERO(fd_set *arg)
	{
		FD_ZERO(arg);
	}

	void WINSock::SKFD_SET(int sockTmp, fd_set * arg)
	{
		FD_SET(sockTmp, arg);
	}

	void WINSock::SKFD_SET(fd_set * arg)
	{
		FD_SET(_sock, arg);
	}

	int WINSock::SKFD_ISSET(fd_set * arg)
	{
		return FD_ISSET(_sock, arg);
	}

	int WINSock::getSock() const
	{
		return _sock;
	}

}