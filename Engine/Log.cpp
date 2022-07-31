//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Log.h"

#include "quill/Quill.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <fstream>

namespace {
	quill::Logger* file_logger_;
#ifdef _DEBUG
	quill::Logger* std_logger_;
#endif
}

struct ProfileUnit {
	int counter_;
	int update_time_;
	int output_time_;
	int obj_num_;
	int update_comp_num_;
	int output_comp_num_;
	std::vector<std::string> logs_;
	//îzóÒå^Ç≈Ç†ÇÈDOMóvëfarrÇ…é©ï™ÇÃÉfÅ[É^Çí«â¡
	void AddDataTo(rapidjson::Document&, rapidjson::Value& arr) const;
};

void Log::Init()
{
	auto file_handler = quill::file_handler("logs/log.txt", "w");
	file_handler->set_pattern(QUILL_STRINGIFY(%(ascii_time) [%(thread)] %(message)), "%D %H:%M:%S",
		quill::Timezone::GmtTime);
	file_logger_ = quill::create_logger("file_logger_", file_handler);
#ifdef _DEBUG
	auto std_handler = quill::stdout_handler();
	std_handler->set_pattern(QUILL_STRINGIFY(%(ascii_time) [%(thread)] %(message)), "%D %H:%M:%S",
		quill::Timezone::GmtTime);
	std_logger_ = quill::create_logger("std_logger_", std_handler);
#endif
	quill::start();
	LOG_INFO(file_logger_, "initialized quill");
#ifdef _DEBUG
	LOG_INFO(std_logger_, "initialized quill");
#endif
	profile_units_.reserve(240);
	log_buffer_.reserve(16);
}

void Log::CleanUp()
{
	//ãLò^ìríÜÇ…èIóπÇµÇΩÇÁÅAãLò^ìríÜÇÃï™ÇèëÇ´ÇæÇ∑
	if (counter_ <= profile_limit_) {
		boost::thread th(ThreadFunc, finished_profiling_, profile_units_);
		finished_profiling_++;
	}
	//èëÇ´çûÇÒÇ≈Ç¢ÇÈìríÜÇÃÉfÅ[É^Ç™Ç†ÇÍÇŒÇªÇÍÇë“Ç¬
	while (finished_profiling_ != jsoned_profiling_) {
		//HACK:Ç±Ç±Ç‡Ç§ÇøÇÂÇ¡Ç∆Ç‹ÇµÇ…Ç»ÇÁÇ»Ç¢Ç©?
		boost::this_thread::sleep(boost::posix_time::millisec(50));
	}
}

void Log::OutputTrivial(const std::string& str, std::source_location loc)
{
	log_buffer_.push_back(str);
	LOG_INFO(file_logger_, "[{} line{} in {}] {}", loc.file_name(), loc.line(), loc.function_name(), str.c_str());
#ifdef _DEBUG
	LOG_INFO(std_logger_, "[{} line{} in {}] {}", loc.file_name(), loc.line(), loc.function_name(), str.c_str());
#endif
}

void Log::OutputCritical(const std::string& str, std::source_location loc)
{
	log_buffer_.push_back(std::string("critical:") + str);
	static auto sprit = "\n================================================================================\n";
	LOG_CRITICAL(file_logger_, "[{} line{} in {}] <critical!>{}", loc.file_name(), loc.line(), loc.function_name(), str.c_str());
#ifdef _DEBUG
	LOG_CRITICAL(std_logger_, "[{} line{} in {}] <critical!>{}", loc.file_name(), loc.line(), loc.function_name(), str.c_str());
#endif
}

void Log::OutputTrivial(const char str[], std::source_location loc)
{
	log_buffer_.push_back(str);
	LOG_INFO(file_logger_, "[{} line{} in {}] {}", loc.file_name(), loc.line(), loc.function_name(), str);
#ifdef _DEBUG
	LOG_INFO(std_logger_, "[{} line{} in {}] {}", loc.file_name(), loc.line(), loc.function_name(), str);
#endif
}

void Log::OutputCritical(const char str[], std::source_location loc)
{
	log_buffer_.emplace_back(std::string("critical:") + str);
	static auto sprit = "\n================================================================================\n";
	LOG_CRITICAL(file_logger_, "[{} line{} in {}] <critical!>{}", loc.file_name(), loc.line(), loc.function_name(), str);
#ifdef _DEBUG
	LOG_CRITICAL(std_logger_, "[{} line{} in {}] <critical!>{}", loc.file_name(), loc.line(), loc.function_name(), str);
#endif
}

void Log::StartPerformanceProfile(int duration)
{
	profile_limit_ = max(profile_limit_, counter_ + duration - 1);
}

void Log::UpdateDatas(int update, int output, int obj, int upcomp, int outcomp)
{
	if (counter_ <= profile_limit_) {
		profile_units_.push_back({ counter_, update,output,obj,upcomp,outcomp,log_buffer_ });
		if (counter_ == profile_limit_) {
			boost::thread th(ThreadFunc, finished_profiling_, profile_units_);
			profile_units_.clear();
			finished_profiling_++;
		}
	}
	log_buffer_.clear();
	counter_++;
}

void Log::ThreadFunc(int filenum, std::vector<ProfileUnit> datas)
{
#ifdef _DEBUG
	LOG_INFO(std_logger_, "start writing down performance profile.filenum:{}", filenum);
#endif
	rapidjson::Document document(rapidjson::Type::kObjectType);
	rapidjson::Value value_filenum;
	value_filenum.SetInt(filenum);
	document.AddMember("filenum", value_filenum, document.GetAllocator());
	rapidjson::Value value_dataarr;
	value_dataarr.SetArray();
	for (auto& data : datas) {
		data.AddDataTo(document, value_dataarr);
	}
	document.AddMember("datas", value_dataarr, document.GetAllocator());
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
	document.Accept(writer);
	std::string filename("./profiller/");
	filename += std::to_string(filenum);
	filename += ".json";
	std::ofstream ofs(filename);
	ofs << sb.GetString() << std::endl;
	jsoned_profiling_++;
#ifdef _DEBUG
	LOG_INFO(std_logger_, "end writing down performance profile.filenum:{}", filenum);
#endif
}

int Log::counter_ = 0;
int Log::profile_limit_ = -1;
std::vector<ProfileUnit> Log::profile_units_;
int Log::finished_profiling_ = 0;
int Log::jsoned_profiling_ = 0;
std::vector<std::string> Log::log_buffer_;

void ProfileUnit::AddDataTo(rapidjson::Document& document, rapidjson::Value& arr) const
{
	rapidjson::Value value_data(rapidjson::Type::kObjectType);
	rapidjson::Value value_counter;
	value_counter.SetInt(counter_);
	value_data.AddMember("counter", value_counter, document.GetAllocator());
	rapidjson::Value value_update_time;
	value_update_time.SetInt(update_time_);
	value_data.AddMember("update_time", value_update_time, document.GetAllocator());
	rapidjson::Value value_output_time;
	value_output_time.SetInt(output_time_);
	value_data.AddMember("output_time", value_output_time, document.GetAllocator());
	rapidjson::Value value_obj_num;
	value_obj_num.SetInt(obj_num_);
	value_data.AddMember("obj_num", value_obj_num, document.GetAllocator());
	rapidjson::Value value_update_comp_num;
	value_update_comp_num.SetInt(update_comp_num_);
	value_data.AddMember("update_comp", value_update_comp_num, document.GetAllocator());
	rapidjson::Value value_output_comp_num;
	value_output_comp_num.SetInt(output_comp_num_);
	value_data.AddMember("output_comp", value_output_comp_num, document.GetAllocator());
	rapidjson::Value value_log_arr;
	value_log_arr.SetArray();
	for (auto& log : logs_) {
		rapidjson::Value value_log;
		value_log.SetString(log.c_str(), log.size(), document.GetAllocator());
		value_log_arr.PushBack(value_log, document.GetAllocator());
	}
	value_data.AddMember("logs", value_log_arr, document.GetAllocator());
	arr.PushBack(value_data, document.GetAllocator());
}
