#include "AComServ.hpp"

namespace Server
{
	AComServ::AComServ()
	{
		_server = new Server(this);
	}

	void AComServ::serverWrite(Socket::ISocket *sock, const std::string &write)
	{
		_server->writeSocket(sock, write);
	}

	void AComServ::runServer()
	{
		_server->run();
	}
}