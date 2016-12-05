#include "AClient.hpp"

namespace Client
{
	template <typename T>
	AClient<T>::AClient(const std::string &ip, const int port, const std::string &protocol)
	{
		Logger::GetInstance().LogLine("=================================================================");
		Logger::GetInstance().LogLine("||                         Creat client                        ||");

	#ifdef __linux__
		Logger::GetInstance().LogLine("||                         Unix socket                         ||");
		_socket = new Socket::UNIXSock<T>();
	#else
		Logger::GetInstance().LogLine("||                          Win socket                         ||");
		_socket = new Socket::WINSock<T>();
	#endif // _WIN

		Logger::GetInstance().LogLine("||                         Init client                         ||");

		_socket->SKInit(ip, port, protocol);

		Logger::GetInstance().LogLine("||                        Connect client                       ||");

		_socket->SKConnect();

		Logger::GetInstance().LogLine("||                     Client initialize OK                    ||");
		Logger::GetInstance().LogLine("=================================================================");

	}

	template <typename T>
	AClient<T>::~AClient()
	{
		delete _socket;
	}

	template <typename T>
	void AClient<T>::run()
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

	template <typename T>
	void AClient<T>::write(const std::string &buff)
	{
		/*
		*	buffer write stock, and write if the socket is disponible.
		*/
		_buff.push_back(buff);
	}

	template <typename T>
	void AClient<T>::checkReadWrite()
	{
		/*
		*	Check if socket recv message
		*/
		if (_socket->SKFD_ISSET(&_fdread)) {
			std::string buf;
			buf = _socket->SKRecv();

			/*
			*	Call surcharge methode 
			*/
			this->recvMessage(buf);
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