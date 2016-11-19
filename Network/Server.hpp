#pragma once

#include <list>

#include "Socket\ISocket.hpp"

#ifdef __linux__
#include "Socket\UNIXSock.hpp"
#else
#include "Socket\WINSock.hpp"
#endif // __linux__

#include "../Logger/Logger.hpp"

namespace Server
{
	class AComServ;

	class Server
	{
	public:
		Server(AComServ *com);
		~Server();

	public:
		/*
		*	"run" is a main server function,
		*	call this function in main while.
		*/
		void					run();

		/*
		*	"writeSocket" is call by AComServ for write in socket.
		*
		*	@param "sock"  -> sock for write
		*	@param "write" -> buff at write
		*/
		void					writeSocket(Socket::ISocket *sock, const std::string &write);

	private:
		/*
		*	"initializeFd" prepare all socket connected for recv message.
		*/
		void					initializeFd();

		/*
		*	"checkReadWrite" check all socket connected for read and write.
		*
		*	@param "sock" -> Socket test
		*
		*	He call function "serverRead" or "decoUser" to AComServ.
		*/
		void					checkReadWrite(Socket::ISocket *sock);

		/*
		*	"recvNewConnection" check socket server, he create new socket.
		*
		*	He call function "newUser" to AComServ.
		*/
		void					recvNewConnection();

	private:
		AComServ					*_comServ;

		/* "_socket" contained all socket connect */
		std::list<Socket::ISocket*>	_socket;

		/* "_sockServ" is a server socket */
		Socket::ISocket				*_sockServ;
		fd_set						_fdread;
		fd_set						_fdwrite;

		/* "_buffwrite" is a circular buffer for write */
		std::list<
			std::pair<Socket::ISocket*,
			std::string>
		>							_buffWrite;
	};
}