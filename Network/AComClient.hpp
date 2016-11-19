#pragma once

#include "Client.hpp"

namespace Client
{
	class AComClient
	{
	public:
		/*
		*	@param "ip"			-> ip server to call
		*	@param "port"		-> port to connect socket
		*	@param "protocol"	-> protocol transfert set ("TCP" or "UDP")
		*/
		AComClient(const std::string &ip, const int port, const std::string &protocol = "TCP");
		~AComClient();

	public:
		/*
		*	"sendMessage" send message at server.
		*
		*	@param "msg"  -> msg to send at server.
		*/
		void				sendMessage(const std::string &msg) const;

		/*
		*	 call "runClient" in the main while.
		*/
		void			runClient() const;

	public:
		/*
		*	"recvMessage" it's call by the client after recv message.
		*
		*	@param "msg"  -> it's a recv message.
		*/
		virtual void		recvMessage(const std::string &msg) = 0;

	private:
		Client				*_client;
	};
}
