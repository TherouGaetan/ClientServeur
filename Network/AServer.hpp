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
	template <class T>
	class AServer
	{
	public:
		AServer(const int port = 2500, const std::string &protocol = "TCP");
		~AServer(); 
	
	public:
		/*
		*	"newUser" is call by the server, after recv new connection.
		*
		*	@param "sock"		-> new socket create.
		*/
		virtual void			newUser(Socket::ISocket<T> *sock) = 0;

		/*
		*	"decoUser" is call by the server, after recv disconnection.
		*
		*	@param "idSock"		-> id socket destroy.
		*/
		virtual void			decoUser(const int idSock) = 0;

		/*
		*	"serverRead" is call by the server, after recv message.
		*
		*	@param "sock"		-> socket to write.
		*	@param "buff"		-> contain recv message to sock.
		*/
		virtual void			serverRead(Socket::ISocket<T> *sock, const std::string &buff) = 0;

	public:
		/*
		*	"run" is a main server function,
		*	call this function in main while.
		*/
		virtual void			run();

		/*
		*	"writeSocket" is call by AComServ for write in socket.
		*
		*	@param "sock"  -> sock for write
		*	@param "write" -> buff at write
		*/
		virtual void			writeSocket(Socket::ISocket<T> *sock, const std::string &write);

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
		void					checkReadWrite(Socket::ISocket<T> *sock);

		/*
		*	"recvNewConnection" check socket server, he create new socket.
		*
		*	He call function "newUser" to AComServ.
		*/
		void					recvNewConnection();

	private:
		const std::string			&_protocol;
		/* "_socket" contained all socket connect */
		std::list<Socket::ISocket<T>*>	_socket;

		/* "_sockServ" is a server socket */
		Socket::ISocket<T>			*_sockServ;
		fd_set						_fdread;
		fd_set						_fdwrite;

		/* "_buffwrite" is a circular buffer for write */
		std::list<
			std::pair<Socket::ISocket<T>*,
			std::string>
		>							_buffWrite;
	};
}