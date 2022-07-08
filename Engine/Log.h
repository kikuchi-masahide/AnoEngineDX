//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>

/// <summary>
/// Log.txtÇ…ÉçÉOÇéÊÇÈ(use boost)
/// </summary>
class Log {
public:
	static void Init();
	static void OutputTrivial(const std::string& str);
	static void OutputCritical(const std::string& str);
	static void OutputTrivial(const char str[]);
	static void OutputCritical(const char str[]);
};

namespace boost {
	void assertion_failed(const char* expr, const char* function, const char* file, long line);
	void assertion_failed_msg(const char* expr, const char* msg, const char* function, const char* file, long _line);
}