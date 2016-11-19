#include "Client.hpp"
#include "AComClient.hpp"

namespace Client
{
	Client::Client(AComClient *com, const std::string &ip, const int port, const std::string &protocol)
	{
		_socket->SKInit(ip, port, protocol);
		_socket->SKConnect();
	}

	Client::~Client()
	{
	}

	void Client::run()
	{
		/*
		*	initialize socket for read
		*/
		_socket->SKFD_ZERO(&_fdread);
		_socket->SKFD_ZERO(&_fdwrite);
		_socket->SKFD_SET(&_fdread);
		if (_buff.size() != 0)
			_socket->SKFD_SET(&_fdwrite);
		if (_socket->SKSelect(&_fdread, &_fdwrite) > 0) {
			checkReadWrite();
		}
	}

	void Client::write(const std::string &buff)
	{
		_buff.push_back(buff);
	}

	void Client::checkReadWrite()
	{
		if (_socket->SKFD_ISSET(&_fdread)) {
			std::string buff;
			_socket->SKRecv(buff);
			_comClient->recvMessage(buff);
		}
		if (_socket->SKFD_ISSET(&_fdwrite)) {
			std::string write;
			write = _buff.front();
			_buff.pop_front();
			_socket->SKSend(write);
		}
	}
}