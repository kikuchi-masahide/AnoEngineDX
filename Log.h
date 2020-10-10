#pragma once

/// <summary>
/// Log.txt‚ÉƒƒO‚ğæ‚é(use boost)
/// </summary>
class Log {
public:
	static void Init();
	static void OutputTrivial(std::string& _str);
	static void OutputCritical(std::string& _str);
	static void OutputTrivial(const char _str[]);
	static void OutputCritical(const char _str[]);
};