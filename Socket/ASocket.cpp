//
// Created by Gaëtan Therou on 18-01-13.
//

#include "ASocket.hpp"

namespace Network {
    namespace TSocket {
        ASocket::ASocket(fd_set *pRead, fd_set *pWrite, const Type &pType)
                : mFdRead(pRead), mFdWrite(pWrite), mType(pType) {
            mSocketId = 0;
        }

        ASocket::ASocket(fd_set *pRead, fd_set *pWrite, unsigned int pSocket)
                : mFdRead(pRead), mFdWrite(pWrite), mSocketId(pSocket) {
        }

        void ASocket::setFdZero() {
            FD_ZERO(mFdRead);
            FD_ZERO(mFdWrite);
        }

        void ASocket::setFdZero(fd_set *pZero) {
            FD_ZERO(pZero);
        }

        void ASocket::setFdSet(fd_set *pSet) {
            FD_SET(mSocketId, pSet);
        }

        int ASocket::setFdIsset(fd_set *pIsset) {
            return FD_ISSET(mSocketId, pIsset);
        }

        int ASocket::getSockId() const {
            return mSocketId;
        }

        void ASocket::sClose() {
#ifdef TARGET_OS_WIN32
            closesocket(mSocketId);
#else
            close(mSocketId);
#endif
        }

        int ASocket::sSelect() {
            int value = 0;
            if ((value = select(mSocketId + 1, mFdRead, mFdWrite, NULL, &mTime)) == -1) {
                std::string msg("select: ");
#ifdef _WIN32
                throw new NetworkException("Socket select failed\n" + std::to_string(WSAGetLastError()));
#endif
            }
            return value;
        }

        void ASocket::sBind() const {
            if (bind(mSocketId, (struct sockaddr *) &mAddr, sizeof(struct sockaddr_in)) == -1)
                throw new NetworkException("Sockect binding failed\n");
        }
    }
}