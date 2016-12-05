#ifndef ISOCKET_HPP_
#define ISOCKET_HPP_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <netinet/in.h>
#endif // _WIN32

namespace Socket
{
	template <class T>
	class ISocket
	{
	public:
		virtual bool	operator==(ISocket *socket) = 0;
		virtual void	SKInit(std::string const &_ip, int port, std::string const & = "TCP") = 0;
		virtual void	SKBind() const = 0;
		virtual void	SKListen(int nb) const = 0;
		virtual int		SKAccept() const = 0;
		virtual void	SKConnect() const = 0;
		virtual int		SKSelect(fd_set *read, fd_set *write) = 0;

		/*
		*	Send
		*/
		virtual int		SKSend(const T &send, size_t const size = 4096) const = 0;
		virtual int		SKSend(const int fd, const T &send, size_t const size = 4096) const = 0;
		virtual int		SKSendTo(const int fd, const T &send, size_t const size = 4096) const = 0;

		/*
		*	Recv
		*/
		virtual T		SKRecv(size_t const size = 4096) = 0;
		virtual T		SKRecvFrom(size_t const size = 4096) = 0;

		/*
		*	Close socket
		*/
		virtual void	SKClose() = 0;

		/*
		*	Prepare socket for read, write.
		*/
		virtual void	SKFD_ZERO(fd_set *arg) = 0;
		virtual void	SKFD_SET(fd_set *arg) = 0;
		virtual int		SKFD_ISSET(fd_set *arg) = 0;

		/*
		*	Return socket
		*/
		virtual int		getSock() const = 0;
	};
}

#endif