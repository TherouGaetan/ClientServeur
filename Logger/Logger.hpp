#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

class Logger
{
public:
	~Logger() {};

public:
	/*
	*	Get the unique instance of "Logger"
	*/
	static Logger	&GetInstance();

	/*
	*	close log file .
	*/
    void			Quit();

	/*
	*	write line, in log file and in std out.
	*
	*	@param "msg" -> is a message at write
	*/
    void			LogLine(std::string const &msg);

	/*
	*	write msg, in log file and in std out.
	*
	*	@param "msg" -> print msg on the end of file
	*/
    void			LogMsg(std::string const &msg);

	/*
	*	Initialize, create, or append log file.
	*/
	void			Init();

private:
	Logger(const Logger &log);
	Logger();

    static Logger	_instance;
    std::fstream	_fstream;
};

#endif
