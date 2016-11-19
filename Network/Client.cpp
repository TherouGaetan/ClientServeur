#include "Client.hpp"
#include "AComClient.hpp"

namespace Client
{
	Client::Client(AComClient *com, const std::string &ip, const int port, const std::string &protocol)
	{
		Logger::GetInstance().LogLine("=================================================================");
		Logger::GetInstance().LogLine("||                         Creat client                        ||");

	#ifdef __linux__
		Logger::GetInstance().LogLine("||                         Unix socket                         ||");
		_socket = new Socket::UNIXSock();
	#else
		Logger::GetInstance().LogLine("||                          Win socket                         ||");
		_socket = new Socket::WINSock();
	#endif // _WIN

		Logger::GetInstance().LogLine("||                         Init client                         ||");

		_socket->SKInit(ip, port, protocol);

		Logger::GetInstance().LogLine("||                        Connect client                       ||");

		_socket->SKConnect();

		Logger::GetInstance().LogLine("||                     Client initialize OK                    ||");
		Logger::GetInstance().LogLine("=================================================================");

	}

	Client::~Client()
	{
		delete _socket;
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
		/*
		*	buffer write stock, and write if the socket is disponible.
		*/
		_buff.push_back(buff);
	}

	void Client::checkReadWrite()
	{
		/*
		*	Check if socket recv message
		*/
		if (_socket->SKFD_ISSET(&_fdread)) {
			std::string buff;
			_socket->SKRecv(buff);
			_comClient->recvMessage(buff);
		}
		/*
		*	Take a first string in buffer for write in socket
		*/
		if (_socket->SKFD_ISSET(&_fdwrite)) {
			std::string write;
			write = _buff.front();
			_buff.pop_front();
			_socket->SKSend(write);
		}
	}
}