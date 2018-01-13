//
// Created by Gaëtan Therou on 18-01-13.
//

#include "NetworkException.hpp"

namespace Network {
    NetworkException::NetworkException(const std::string &pMsg)
    : runtime_error(pMsg), mMessage(pMsg) {
        Logger::GetInstance().LogLine("NetworkException : " + mMessage + ".");
    }

    const char *NetworkException::what() const throw() {
        return mMessage.c_str();
    }
}