#include "Log.h"

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/attributes.hpp>
#include <iostream>

void Log::Init()
{
	namespace attrs = boost::log::attributes;
	namespace keywords = boost::log::keywords;
	namespace sinks = boost::log::sinks;

	boost::log::add_file_log(
		keywords::file_name = "logs/%Y_%m_%d.log",
		keywords::format = "[%TimeStamp%]: %Message%",
		keywords::auto_flush = true
	);
	boost::log::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
}

void Log::OutputTrivial(std::string& _str)
{
	BOOST_LOG_TRIVIAL(info) << _str.c_str();
#ifdef _DEBUG
	std::cout << _str << "\n";
#endif
}

void Log::OutputCritical(std::string& _str)
{
	BOOST_LOG_TRIVIAL(info)
		<< "\n================================================================================\n"
		<< "<critical!>" << _str.c_str()
		<< "\n================================================================================";
#ifdef _DEBUG
	std::cout 
		<< "\n================================================================================\n"
		<< "<critical!>" << _str.c_str()
		<< "\n================================================================================\n";
#endif
}

void Log::OutputTrivial(const char _str[])
{
	BOOST_LOG_TRIVIAL(info) << _str;
#ifdef _DEBUG
	std::cout << _str << "\n";
#endif
}

void Log::OutputCritical(const char _str[])
{
	BOOST_LOG_TRIVIAL(info) 
		<< "\n================================================================================\n" 
		<< "<critical!>" << _str
		<< "\n================================================================================";
#ifdef _DEBUG
	std::cout
		<< "\n================================================================================\n"
		<< "<critical!>" << _str
		<< "\n================================================================================\n";
#endif
}

namespace boost {
	void assertion_failed(
		const char* _expr, const char* _function, const char* _file, long _line
	)
	{
		std::string str("BOOST_ASSERT!\n\tExpression : ");
		str += _expr;
		str += "\n\tFunction : ";
		str += _function;
		str += "\n\tFile : ";
		str += _file;
		str += "\n\tLine : ";
		str += std::to_string(_line);
		Log::OutputCritical(str.c_str());
		std::abort();
	}
	void assertion_failed_msg(
		const char* _expr, const char* _msg, const char* _function, const char* _file, long _line
	)
	{
		std::string str("BOOST_ASSERT_MSG!\n\tExpression : ");
		str += _expr;
		str += "\n\tMessage : ";
		str += _msg;
		str += "\n\tFunction : ";
		str += _function;
		str += "\n\tFile : ";
		str += _file;
		str += "\n\tLine : ";
		str += std::to_string(_line);
		Log::OutputCritical(str.c_str());
		std::abort();
	}
}