#include "AComClient.hpp"

namespace Client
{
	AComClient::AComClient(const std::string &ip, const int port, const std::string &protocol)
	{
		_client = new Client(this, ip, port, protocol);
	}

	AComClient::~AComClient()
	{
	}

	void AComClient::sendMessage(const std::string & msg)
	{
		_client->write(msg);
	}
	void AComClient::runClient()
	{
		_client->run();
	}
}
