#ifndef UNIXSOCK_HPP_
#define UNIXSOCK_HPP_

#ifdef __linux__

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/select.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#endif // __linux__

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "SocketException.hpp"
#include "ISocket.hpp"

class UNIXSock : public ISocket
{
public:
    UNIXSock();
	UNIXSock(const int fd);
    ~UNIXSock();

public:
    void SKInit(std::string const &, int, std::string const & = "TCP") override;
    void SKBind() const override;
    void SKListen(int nb) const override;
    int SKAccept() const override;
    int SKRecv(int, std::string &, size_t const size = 4096) override;
    int SKRecv(std::string &, size_t const size = 4096) override;
    void SKClose(int) const override;
    void SKClose() override;
    void SKConnect() const override;
    int SKSend(int fd, std::string const& str) const override;
    int SKSend(std::string const&) const override;
    int SKSelect(int sockTmp, fd_set *read, fd_set *write) override;
    int SKSelect(fd_set *read, fd_set *write) override;
    void SKFD_ZERO(fd_set *arg) override;
    void SKFD_SET(int sockTmp, fd_set *arg) override;
    void SKFD_SET(fd_set *arg) override;
	int SKFD_ISSET(fd_set *arg) override;
    int getSock() const override;
};

#endif
