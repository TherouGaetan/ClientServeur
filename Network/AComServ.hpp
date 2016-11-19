#pragma once

#include <string>

#include "Server.hpp"
#include "Socket\ISocket.hpp"

namespace Server
{
	class AComServ
	{
	public:
		/*
		*	@param "port"		-> port to connect socket
		*	@param "protocol"	-> protocol transfert set ("TCP" or "UDP")
		*/
		AComServ(const int port = 2500, const std::string &protocol = "TCP");
		virtual ~AComServ() {};

	public:
		/*
		*	"serverWrite" call to the server for write, in socket "sock".
		*
		*	@param "sock"		-> socket to write.
		*	@param "write"		-> buff at write.
		*/
		void			serverWrite(Socket::ISocket *sock, const std::string &write) const;

		/*
		*	 call "runServer" in the main while.
		*/
		void			runServer() const;

	public:
		/*
		*	"newUser" is call by the server, after recv new connection.
		*
		*	@param "sock"		-> new socket create.
		*/
		virtual void	newUser(Socket::ISocket *sock) = 0;

		/*
		*	"decoUser" is call by the server, after recv disconnection.
		*
		*	@param "idSock"		-> id socket destroy.
		*/
		virtual void	decoUser(const int idSock) = 0;

		/*
		*	"serverRead" is call by the server, after recv message.
		*
		*	@param "sock"		-> socket to write.
		*	@param "buff"		-> contain recv message to sock.
		*/
		virtual void	serverRead(Socket::ISocket *sock, const std::string &buff) = 0;

	private:
		Server			*_server;
	};
}
