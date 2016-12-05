#include "WINSock.hpp"

namespace Socket
{
	template <class T>
	WINSock<T>::WINSock()
	{
	}

	template <class T>
	WINSock<T>::WINSock(const int fd)
	{
		_sock = fd;
	}

	template <class T>
	WINSock<T>::~WINSock()
	{
		WSACleanup();
	}

	template <class T>
	bool WINSock<T>::operator==(ISocket *socket)
	{
		return (this->getSock() == socket->getSock());
	}

	template <class T>
	void WINSock<T>::SKInit(std::string const &_ip, int port, std::string const &_protocol)
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

	template <class T>
	void WINSock<T>::SKBind() const
	{
		if (bind(_sock, (struct sockaddr *) &_server, sizeof(struct sockaddr_in)) == -1)
			throw SocketException("Sockect binding failed\n");
	}

	template <class T>
	void WINSock<T>::SKListen(int queueMax) const
	{
		if (listen(_sock, queueMax) == -1)
			throw SocketException("Socket listen failed\n");
	}

	template <class T>
	int WINSock<T>::SKAccept() const
	{
		int fd;
		struct sockaddr_in client;
		int len;

		len = sizeof(client);
		if ((fd = accept(_sock, (struct sockaddr *) &client, &len)) == -1)
			throw SocketException("Socket accept failed");
		return fd;
	}

	template <class T>
	void WINSock<T>::SKConnect() const
	{
		if ((connect(_sock, (struct sockaddr *) &_server, sizeof(struct sockaddr_in))) < 0)
			throw SocketException("Socket connect failed\n");
	}

	template <class T>
	int WINSock<T>::SKSelect(fd_set * read, fd_set * write)
	{
		int ret;
		if ((ret = select(_sock + 1, read, write, NULL, &_tv)) == -1)
			throw SocketException("Socket select failed\n");
		return ret;
	}



	/*
	*	Send
	*/
	template<class T>
	int WINSock<T>::SKSend(const T & send, size_t const size) const
	{
		return send(_sock, T, size, 0);
	}

	template <class T>
	int WINSock<T>::SKSend(const int fd, const T &send, size_t const size) const
	{
		return send(fd, T, size, 0;);
	}

	template <class T>
	int WINSock<T>::SKSendTo(const int fd, const T &send, size_t const size) const
	{
		return sendto(fd, T, size, 0, (struct sockaddr *)&_server, sizeof((struct sockaddr *)&_server));;
	}

	/*
	*	Recv
	*/
	template <class T>
	T WINSock<T>::SKRecv(size_t const size = 4096)
	{
		char buf[size];
		recv(_sock, &buf, size, 0);
		T message;
		message << buf;
		return message;
	}

	template <class T>
	T WINSock<T>::SKRecvFrom(size_t const size = 4096)
	{
		char buf[size];
		recvfrom(_sock, &buf, size, 0, (struct sockaddr *)&_server, sizeof((struct sockaddr *)&_server));
		T message;
		message << buf;
		return message;
	}

	/*
	*	Close socket
	*/
	template <class T>
	void WINSock<T>::SKClose()
	{
		closesocket(_sock);
	}

	/*
	*	Prepare socket for read, write.
	*/
	template <class T>
	void WINSock<T>::SKFD_ZERO(fd_set *arg)
	{
		FD_ZERO(arg);
	}

	template <class T>
	void WINSock<T>::SKFD_SET(fd_set * arg)
	{
		FD_SET(_sock, arg);
	}

	template <class T>
	int WINSock<T>::SKFD_ISSET(fd_set * arg)
	{
		return FD_ISSET(_sock, arg);
	}

	template <class T>
	int WINSock<T>::getSock() const
	{
		return _sock;
	}

}