//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace quill {
	class Logger;
}

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
