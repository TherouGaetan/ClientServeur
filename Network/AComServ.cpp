#include "AComServ.hpp"

namespace Server
{
	AComServ::AComServ(const int port, const std::string &protocol)
	{
		_server = new Server(this, port, protocol);
	}

	void AComServ::serverWrite(Socket::ISocket *sock, const std::string &write) const
	{
		_server->writeSocket(sock, write);
	}

	void AComServ::runServer() const
	{
		_server->run();
	}
}