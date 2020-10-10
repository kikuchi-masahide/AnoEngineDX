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

void Log::OutputTrivial(std::string _str)
{
#ifdef _DEBUG
	BOOST_LOG_TRIVIAL(info) << _str;
	std::cout << _str << "\n";
#endif
}

void Log::OutputCritical(std::string _str)
{
	BOOST_LOG_TRIVIAL(info) << "\n<critical!>" << _str << "\n";
#ifdef _DEBUG
	std::cout << "================================================================================\n";
	std::cout << "<critical!>\n\t" << _str << "\n";
	std::cout << "================================================================================\n";
#endif
}