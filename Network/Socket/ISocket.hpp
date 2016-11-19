#ifndef ISOCKET_HPP_
#define ISOCKET_HPP_

#include <sys/types.h>
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
	class ISocket
	{
	public:
		virtual bool	operator==(ISocket *) = 0;
		virtual void	SKInit(std::string const &_ip, int port, std::string const & = "TCP") = 0;
		virtual void	SKBind() const = 0;
		virtual void	SKListen(int nb) const = 0;
		virtual int		SKAccept() const = 0;
		virtual int		SKRecv(int, std::string &, size_t const size = 4096) = 0;
		virtual int		SKRecv(std::string &, size_t const size = 4096) = 0;
		virtual void	SKClose(int) const = 0;
		virtual void	SKClose() = 0;
		virtual void	SKConnect() const = 0;
		virtual int		SKSend(int fd, std::string const& str) const = 0;
		virtual int		SKSend(std::string const&) const = 0;
		virtual int		SKSelect(int sockTmp, fd_set *read, fd_set *write) = 0;
		virtual int		SKSelect(fd_set *read, fd_set *write) = 0;
		virtual void	SKFD_ZERO(fd_set *arg) = 0;
		virtual void	SKFD_SET(int sockTmp, fd_set *arg) = 0;
		virtual void	SKFD_SET(fd_set *arg) = 0;
		virtual int		SKFD_ISSET(fd_set *arg) = 0;
		virtual int		getSock() const = 0;

	protected:
		int					_sock;
		struct sockaddr_in	_server;
		std::string const	_name;
		struct timeval		_tv;
	};
}

#endif