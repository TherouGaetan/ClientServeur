#include "UNIXSock.hpp"

UNIXSock::UNIXSock()
{

}

UNIXSock::UNIXSock(const int fd)
{
	sock = fd;
}

UNIXSock::~UNIXSock()
{

}

void UNIXSock::SKInit(std::string const &_ip, int port, std::string const &_protocol)
{
    struct protoent *proto;
    tv.tv_sec = 1;
    tv.tv_usec = 1000000;
    if ((proto = getprotobyname(_protocol.c_str())) == NULL)
        throw SocketException("getprotobyname failed\n");
    sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
    if (sock < 0)
        throw SocketException("Socket creation failed\n");
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
	char opt;
	if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
		|| (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int) == -1)))
		throw SocketException("Socket re use socket fail\n");
}

void UNIXSock::SKBind() const
{
    if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_in)))
        throw SocketException("Socket binding failed\n");
}

void UNIXSock::SKListen(int queueMax) const
{
    if (listen(sock, queueMax) == -1)
        throw SocketException("Socket listen failed\n");
}

int UNIXSock::SKAccept() const
{
    int fd;
    struct sockaddr_in client;
    unsigned int len;

    len = sizeof(client);
    if ((fd = accept(sock, (struct sockaddr *) &client, &len)) == -1)
        throw SocketException("Socket accept failed");
    return fd;
}

int UNIXSock::SKRecv(int fd, std::string &buff, size_t const size)
{
    char * test = new char[size];
    memset(test, 0, size);
    int rd = recv(fd, test, size - 1, 0);
    if (rd <= 0)
        return -1;
    test[rd] = 0;
    buff = test;
    delete test;
    return rd;
}

int UNIXSock::SKRecv(std::string &buff, size_t const size)
{
    char * test = new char[size];
    memset(test, 0, size);
    int rd = recv(sock, test, size - 1, 0);
    if (rd <= 0)
        return -1;
    test[rd] = 0;
    buff = test;
    delete test;
    return rd;
}

void UNIXSock::SKClose(int fd) const
{
    close(fd);
}

void UNIXSock::SKClose()
{
    close(sock);
}

void UNIXSock::SKConnect() const
{
    if ((connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_in))) < 0)
        throw SocketException("Socket connect failed\n");
}

int UNIXSock::SKSend(int fd, std::string const &str) const
{
    return send(fd, str.c_str(), str.length(), 0);
}

int UNIXSock::SKSend(std::string const &str) const
{
    return send(sock, str.c_str(), str.length(), 0);
}

int UNIXSock::SKSelect(int sockTmp, fd_set * read, fd_set * write)
{
	int ret;
    if ((ret = select(sockTmp + 1, read, write, NULL, &tv)) == -1)
        throw SocketException("Socket select failed\n");
	return ret;
}

int UNIXSock::SKSelect(fd_set * read, fd_set * write)
{
	int ret;
    if ((ret = select(sock + 1, read, write, NULL, &tv)) == -1)
        throw SocketException("Socket select failed\n");
	return ret;
}

void UNIXSock::SKFD_ZERO(fd_set *arg)
{
    FD_ZERO(arg);
}

void UNIXSock::SKFD_SET(int sockTmp, fd_set * arg)
{
    FD_SET(sockTmp, arg);
}

void UNIXSock::SKFD_SET(fd_set * arg)
{
    FD_SET(sock, arg);
}

int UNIXSock::SKFD_ISSET(fd_set * arg)
{
	return FD_ISSET(sock, arg);
}

int UNIXSock::getSock() const
{
    return sock;
}
