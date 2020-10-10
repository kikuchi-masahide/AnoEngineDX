#pragma once

/// <summary>
/// Log.txtにログを取る(use boost)
/// </summary>
class Log {
public:
	static void Init();
	static void OutputTrivial(std::string _str);
	static void OutputCritical(std::string _str);
};