#pragma once
#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>

/// <summary>
/// Log.txtÇ…ÉçÉOÇéÊÇÈ(use boost)
/// </summary>
class Log {
public:
	static void Init();
	static void OutputTrivial(std::string& _str);
	static void OutputCritical(std::string& _str);
	static void OutputTrivial(const char _str[]);
	static void OutputCritical(const char _str[]);
};

namespace boost {
	void assertion_failed(const char* _expr, const char* _function, const char* _file, long _line);
	void assertion_failed_msg(const char* _expr, const char* _msg, const char* _function, const char* _file, long _line);
}