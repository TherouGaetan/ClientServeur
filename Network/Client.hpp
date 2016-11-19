#pragma once

#include <list>

#ifdef __linux__
#include "Socket\UNIXSock.hpp"
#else
#include "Socket\WINSock.hpp"
#endif // __linux__

namespace Client
{
	class AComClient;

	class Client
	{
	public:
		/*
		*	@param "com"		->  class communicate with client
		*	@param "ip"			->  ip for connect client to the server
		*	@param "port"		->  port to connect at the server
		*	@param "protocol"	->	by default it's set at "TCP"
		*/
		Client(AComClient *com, const std::string &ip, const int port, const std::string &protocol = "TCP");
		~Client();

	public:
		/*
		*	"run" is a main server function,
		*	call this function in main while.
		*/
		void					run();

		/*
		*	"write" add message to buff write.
		*
		*	@param "buff"    -> message at send.
		*/
		void					write(const std::string &buff);

	private:
		void					checkReadWrite();

	private:
		AComClient				*_comClient;
		Socket::ISocket			*_socket;
		std::list<std::string>	_buff;
		fd_set					_fdread;
		fd_set					_fdwrite;
	};
}