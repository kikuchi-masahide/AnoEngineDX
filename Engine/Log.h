//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace quill {
	class Logger;
}

//パフォーマンスプロファイルの際の、1frameにおける記録データ
struct ProfileUnit;

/// <summary>
/// Log.txtにログを取る
/// </summary>
class Log {
public:
	static void Init();
	static void CleanUp();
	static void OutputTrivial(const std::string& str, std::source_location loc = std::source_location::current());
	static void OutputCritical(const std::string& str, std::source_location loc = std::source_location::current());
	static void OutputTrivial(const char str[], std::source_location loc = std::source_location::current());
	static void OutputCritical(const char str[], std::source_location loc = std::source_location::current());
	/// <summary>
	/// このframeを含めてduration frameのパフォーマンス記録を開始する
	/// 既に開始されていた場合、durationだけ時間が延長される
	/// </summary>
	/// <param name="duration"></param>
	static void StartPerformanceProfile(int duration);
	/// <summary>
	/// この情報を記録し、Log内部のカウンタを進める(プロファイルの有無によらずGameから毎frame呼び出す)
	/// </summary>
	static void UpdateDatas(int update, int output, int obj, int upcomp, int outcomp);
private:
	//JSON書き起こしスレッドから呼ばれる関数
	static void ThreadFunc(int filenum, std::vector<ProfileUnit> datas);
	static int counter_;
	static int profile_limit_;
	static std::vector<ProfileUnit> profile_units_;
	//記録終了したプロファイリングの数、JSONで書き起こしを完了したプロファイルの数
	static int finished_profiling_;
	static int jsoned_profiling_;
	static std::vector<std::string> log_buffer_;
	static boost::mutex log_buffer_mutex_;
};
