#pragma once

#include <exception>
#include <iostream>

#include "../Logger/Logger.hpp"

class SocketException : public std::runtime_error
{
private:
    std::string message;

public:
    SocketException(std::string const &Msg) : std::runtime_error(Msg.c_str())
    {
	#ifdef __linux__
		message = "\033[1;31m" + Msg + "\033[0m";
	#else
		message = Msg;
	#endif // __linux__
    }

    virtual ~SocketException() throw() {}

    virtual const char * what() const throw() 
    {
        return message.c_str();
    }
};