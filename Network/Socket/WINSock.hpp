#pragma once

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <io.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "../../Exception/SocketException.hpp"
#include "ISocket.hpp"

namespace Socket
{
	template <class T>
	class WINSock : public ISocket<T>
	{
	public:
		WINSock();
		WINSock(const int fd);
		~WINSock();

	public:
		bool	operator==(ISocket *socket) override;

	public:
		void	SKInit(const std::string &_ip, int port, const std::string & = "TCP");
		void	SKBind() const;
		void	SKListen(int nb) const;
		int		SKAccept() const;
		void	SKConnect() const;
		int		SKSelect(fd_set *read, fd_set *write);

		/*
		*	Send
		*/
		int		SKSend(const T &send, size_t const size = 4096) const;
		int		SKSend(const int fd, const T &send, size_t const size = 4096) const;
		int		SKSendTo(const int fd, const T &send, size_t const size = 4096) const;

		/*
		*	Recv
		*/
		T		SKRecv(size_t const size = 4096);
		T		SKRecvFrom(size_t const size = 4096);

		/*
		*	Close socket
		*/
		void	SKClose();

		/*
		*	Prepare socket for read, write.
		*/
		void	SKFD_ZERO(fd_set *arg);
		void	SKFD_SET(fd_set *arg);
		int		SKFD_ISSET(fd_set *arg);

		/*
		*	Return socket
		*/
		int		getSock() const;

	private:
		int					_sock;
		struct sockaddr_in	_server;
		std::string const	_name;
		struct timeval		_tv;
	};
}
