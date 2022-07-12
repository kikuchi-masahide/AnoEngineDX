//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Log.h"

#include "quill/Quill.h"

void Log::Init()
{
	auto file_handler = quill::file_handler("logs/log.txt", "w");
	file_logger_ = quill::create_logger("file_logger_", file_handler);
#ifdef _DEBUG
	auto std_handler = quill::stdout_handler();
	std_logger_ = quill::create_logger("std_logger_", std_handler);
#endif
	quill::start();
	LOG_INFO(file_logger_, "initialized quill");
#ifdef _DEBUG
	LOG_INFO(std_logger_, "initialized quill");
#endif
}

void Log::OutputTrivial(const std::string& str)
{
	LOG_INFO(file_logger_, "{}", str.c_str());
#ifdef _DEBUG
	LOG_INFO(std_logger_, "{}", str.c_str());
#endif
}

void Log::OutputCritical(const std::string& str)
{
	static auto sprit = "\n================================================================================\n";
	LOG_CRITICAL(file_logger_, "{}<critical!>{}{}", sprit, str.c_str(), sprit);
#ifdef _DEBUG
	LOG_CRITICAL(std_logger_, "{}<critical!>{}{}", sprit, str.c_str(), sprit);
#endif
}

void Log::OutputTrivial(const char str[])
{
	LOG_INFO(file_logger_, "{}", str);
#ifdef _DEBUG
	LOG_INFO(std_logger_, "{}", str);
#endif
}

void Log::OutputCritical(const char str[])
{
	static auto sprit = "\n================================================================================\n";
	LOG_CRITICAL(file_logger_, "{}<critical!>{}{}", sprit, str, sprit);
#ifdef _DEBUG
	LOG_CRITICAL(std_logger_, "{}<critical!>{}{}", sprit, str, sprit);
#endif
}

quill::Logger* Log::file_logger_ = nullptr;
quill::Logger* Log::std_logger_ = nullptr;
