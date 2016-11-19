#include "Logger.hpp"

Logger Logger::_instance = Logger();

Logger::Logger()
{
	Init();
}

Logger::Logger(const Logger & log)
{
	Init();
}


Logger & Logger::GetInstance()
{
    return _instance;
}

void Logger::Init()
{
    std::string filename("log-");
    time_t now = time(0);
	struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    filename.append(std::to_string(timeinfo.tm_mday)).append("-")
        .append(std::to_string(1 + timeinfo.tm_mon)).append("-")
        .append(std::to_string(1900 + timeinfo.tm_year)).append(".log");
    std::cout << "Initializing logger in " << filename << "... ";
    _fstream.open(filename, std::fstream::out | std::fstream::app);
    if (!_fstream.is_open())
    {
        std::cout << "error!" << std::endl;
        std::cerr << "Error while opening file" << std::endl;
    }
    else
        std::cout << "done!" << std::endl;
}

void Logger::Quit()
{
    if (_fstream.is_open())
    {
        std::cout << "Closing file... ";
        _fstream.close();
        if (_fstream.is_open())
            std::cout << "error!" << std::endl;
        else
            std::cout << "done!" << std::endl;
    }
}

void Logger::LogLine(std::string const & msg)
{
    if (_fstream.is_open())
    {
        _fstream << msg << std::endl;
        _fstream.flush();
        std::cout << msg << std::endl;
    }
    else
    {
        std::cerr << "Error while loging mesage: \"" << msg << "\"" << std::endl;
    }
}

void Logger::LogMsg(std::string const & msg)
{
    if (_fstream.is_open())
    {
        _fstream << msg;
        _fstream.flush();
        std::cout << msg;
    }
    else
    {
        std::cerr << "Error while loging mesage: " << msg << std::endl;
    }
}
