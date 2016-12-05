#pragma once

#include <list>

#ifdef __linux__
#include "Socket\UNIXSock.hpp"
#else
#include "Socket\WINSock.hpp"
#endif // __linux__

namespace Client
{
	template <class T>
	class AClient
	{
	public:
		/*
		*	@param "ip"			->  ip for connect client to the server
		*	@param "port"		->  port to connect at the server
		*	@param "protocol"	->	by default it's set at "TCP"
		*/
		AClient(const std::string &ip, const int port, const std::string &protocol = "TCP");
		~AClient();

	public:
		/*
		*	"recvMessage" it's call by the client after recv message.
		*
		*	@param "msg"  -> it's a recv message.
		*/
		virtual void			recvMessage(const std::string &msg) = 0;

	public:
		/*
		*	"run" is a main server function,
		*	call this function in main while.
		*/
		virtual void			run();

		/*
		*	"write" add message to buff write.
		*
		*	@param "buff"    -> message at send.
		*/
		virtual void			write(const std::string &buff);

	private:
		void					checkReadWrite();

	private:
		Socket::ISocket<T>		*_socket;
		std::list<T>			_buff;
		fd_set					_fdread;
		fd_set					_fdwrite;
	};
}