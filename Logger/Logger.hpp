#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <time.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

namespace Network {
    class Logger {
    public:
        ~Logger() {};

    public:
        /*
        *	Get the unique instance of "Logger"
        */
        static Logger &GetInstance();

        /*
        *	close log file.
        */
        void Quit();

        /*
        *	write line, in log file and in std out.
        *
        *	@param "msg" -> is a message at write
        */
        void LogLine(const std::string &msg);

        /*
        *	write msg, in log file and in std out.
        *
        *	@param "msg" -> print msg on the end of file
        */
        void LogMsg(const std::string &msg);

        /*
        *	Initialize, create, or append log file.
        */
        void Init();

    private:
        Logger(const Logger &log);

        Logger();

    public:
        static Logger mInstance;
        std::fstream mFstream;
    };
}

#endif
