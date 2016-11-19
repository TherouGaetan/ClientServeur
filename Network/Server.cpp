#include "Server.hpp"
#include "AComServ.hpp"

namespace Server
{
	Server::Server(AComServ *com, const int port, const std::string &protocol)
	{
		Logger::GetInstance().LogLine("=================================================================");
		Logger::GetInstance().LogLine("||                         Creat server                        ||");

	#ifdef __linux__
		Logger::GetInstance().LogLine("||                         Unix socket                         ||");
		_sockServ = new Socket::UNIXSock();
	#else
		Logger::GetInstance().LogLine("||                          Win socket                         ||");
		_sockServ = new Socket::WINSock();
	#endif // _WIN

		Logger::GetInstance().LogLine("||                         Init server                         ||");

		_sockServ->SKInit(".", port, protocol);

		Logger::GetInstance().LogLine("||                         Bind server                         ||");

		_sockServ->SKBind();

		Logger::GetInstance().LogLine("||                        Listen server                        ||");

		_sockServ->SKListen(5);

		Logger::GetInstance().LogLine("||                     Server initialize OK                    ||");
		Logger::GetInstance().LogLine("=================================================================");
	}

	Server::~Server()
	{
		delete _sockServ;
	}

	void Server::run()
	{
		initializeFd();

		if (_sockServ->SKSelect(&_fdread, &_fdwrite) > 0) {
			/*
			*	Check socket server, for new user
			*/
			recvNewConnection();

			/*
			*	Check all socket connect, if he need read or write
			*/
			for (std::list<Socket::ISocket*>::iterator it = _socket.begin(); it != _socket.end(); ++it) {
				checkReadWrite((*it));
			}
		}
	}

	void Server::writeSocket(Socket::ISocket *sock, const std::string &write)
	{
		/*
		*	Initialize socket for write
		*/
		sock->SKFD_SET(&_fdwrite);

		/*
		*	Stock with std::pair, the socket and buff to write.
		*/
		_buffWrite.push_back(std::pair<Socket::ISocket*, std::string>(sock, write));
	}

	void Server::initializeFd()
	{
		/*
		*	Prepare fd_read and fd_write.
		*/
		_sockServ->SKFD_ZERO(&_fdread);
		_sockServ->SKFD_ZERO(&_fdwrite);

		/*
		*	Initialize all socket for read and write
		*/
		_sockServ->SKFD_SET(&_fdread);
		for (std::list<Socket::ISocket *>::iterator it = _socket.begin(); it != _socket.end(); ++it)
			(*it)->SKFD_SET(&_fdread);
	}

	void Server::checkReadWrite(Socket::ISocket *sock)
	{
		/*
		*	Check with FD_ISSET, if sock need read.
		*/
		if (sock->SKFD_ISSET(&_fdread)) {
			/*
			*	Recv return -1, when sock is "end of file"
			*	
			*	So, if Recv return -1, user disconneted.
			*/
			std::string buff;
			if (_sockServ->SKRecv(buff) != -1) {
				_comServ->serverRead(sock, buff);
				Logger::GetInstance().LogLine("\t-\tRecv message to (" + std::to_string(sock->getSock()) + ").");
			}
			else {
				int idSock = sock->getSock();
				delete sock;
				_comServ->decoUser(idSock);
				Logger::GetInstance().LogLine("\t-\tUser with socket (" + std::to_string(idSock) + ") disconnect.");
			}
		}
		/*
		*	Check if sock need write
		*/
		if (sock->SKFD_ISSET(&_fdwrite)) {
			for (std::list<std::pair<Socket::ISocket*, std::string>>::iterator it = _buffWrite.begin(); it != _buffWrite.end(); ++it) {
				if ((*it).first == sock) {
					_sockServ->SKSend((*it).first->getSock(), (*it).second);
					Logger::GetInstance().LogLine("\t-\tSend message to (" + std::to_string((*it).first->getSock()) + ").");
					_buffWrite.erase(it);
					break;
				}
			}
		}
	}

	void Server::recvNewConnection()
	{
		if (FD_ISSET(_sockServ, &_fdread)) {
			/*
			*	Create socket for new user/connection
			*/
		#ifdef __linux__
			_comServ->newUser(new Socket::UNIXSock(_sockServ->SKAccept()));
		#else
			_comServ->newUser(new Socket::WINSock(_sockServ->SKAccept()));
		#endif // __linux__
			Logger::GetInstance().LogLine("\t-\tNew connection.");
		}
	}
}