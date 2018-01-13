#include "Logger.hpp"

namespace Network {
    Logger Logger::mInstance = Logger();

    Logger::Logger() {
        Init();
    }

    Logger::Logger(const Logger &log) {
        Init();
    }


    Logger &Logger::GetInstance() {
        return mInstance;
    }

    void Logger::Init() {
        std::string filename("log-");
        time_t now = time(0);
        struct tm timeinfo;
#ifdef TARGET_OS_WIN32
        localtime_s(&timeinfo, &now);
#else
        localtime_r(&now, &timeinfo);
#endif /*__linux*/
        filename.append(std::to_string(timeinfo.tm_mday)).append("-")
                .append(std::to_string(1 + timeinfo.tm_mon)).append("-")
                .append(std::to_string(1900 + timeinfo.tm_year)).append(".log");
        std::cout << "Initializing logger in " << filename << "... ";
        mFstream.open(filename, std::fstream::out | std::fstream::app);
        if (!mFstream.is_open()) {
            std::cout << "error!" << std::endl;
            std::cerr << "Error while opening file" << std::endl;
        } else
            std::cout << "done!" << std::endl;
    }

    void Logger::Quit() {
        if (mFstream.is_open()) {
            std::cout << "Closing file... ";
            mFstream.close();
            if (mFstream.is_open())
                std::cout << "error!" << std::endl;
            else
                std::cout << "done!" << std::endl;
        }
    }

    void Logger::LogLine(const std::string &msg) {
        if (mFstream.is_open()) {
            mFstream << msg << std::endl;
            mFstream.flush();
            std::cout << msg << std::endl;
        } else {
            std::cerr << "Error while loging mesage: \"" << msg << "\"" << std::endl;
        }
    }

    void Logger::LogMsg(const std::string &msg) {
        if (mFstream.is_open()) {
            mFstream << msg;
            mFstream.flush();
            std::cout << msg;
        } else {
            std::cerr << "Error while loging mesage: " << msg << std::endl;
        }
    }
}