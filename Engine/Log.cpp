//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
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

void Log::OutputTrivial(const std::string& str)
{
	BOOST_LOG_TRIVIAL(info) << str.c_str();
#ifdef _DEBUG
	std::cout << str << "\n";
#endif
}

void Log::OutputCritical(const std::string& str)
{
	BOOST_LOG_TRIVIAL(info)
		<< "\n================================================================================\n"
		<< "<critical!>" << str.c_str()
		<< "\n================================================================================";
#ifdef _DEBUG
	std::cout 
		<< "\n================================================================================\n"
		<< "<critical!>" << str.c_str()
		<< "\n================================================================================\n";
#endif
}

void Log::OutputTrivial(const char str[])
{
	BOOST_LOG_TRIVIAL(info) << str;
#ifdef _DEBUG
	std::cout << str << "\n";
#endif
}

void Log::OutputCritical(const char str[])
{
	BOOST_LOG_TRIVIAL(info) 
		<< "\n================================================================================\n" 
		<< "<critical!>" << str
		<< "\n================================================================================";
#ifdef _DEBUG
	std::cout
		<< "\n================================================================================\n"
		<< "<critical!>" << str
		<< "\n================================================================================\n";
#endif
}

namespace boost {
	void assertion_failed(
		const char* expr, const char* function, const char* file, long line
	)
	{
		std::string str("BOOST_ASSERT!\n\tExpression : ");
		str += expr;
		str += "\n\tFunction : ";
		str += function;
		str += "\n\tFile : ";
		str += file;
		str += "\n\tLine : ";
		str += std::to_string(line);
		Log::OutputCritical(str.c_str());
		std::abort();
	}
	void assertion_failed_msg(
		const char* expr, const char* msg, const char* function, const char* file, long line
	)
	{
		std::string str("BOOST_ASSERT_MSG!\n\tExpression : ");
		str += expr;
		str += "\n\tMessage : ";
		str += msg;
		str += "\n\tFunction : ";
		str += function;
		str += "\n\tFile : ";
		str += file;
		str += "\n\tLine : ";
		str += std::to_string(line);
		Log::OutputCritical(str.c_str());
		std::abort();
	}
}