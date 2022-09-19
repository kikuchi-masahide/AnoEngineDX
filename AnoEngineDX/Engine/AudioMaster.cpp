//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "AudioMaster.h"

#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include "CryptoMaster.h"
#include "SoundEvent.h"
#include "Log.h"

//FMODのデバッグ出力で用いる関数
FMOD_RESULT FMODDebugCallback(FMOD_DEBUG_FLAGS, const char*, int, const char*, const char* message)
{
	std::string str("error from FMOD:");
	str += message;
	Log::OutputCritical(str);
	return FMOD_OK;
}

AudioMaster::AudioMaster()
{
}

AudioMaster::~AudioMaster()
{
}

bool AudioMaster::Initialize()
{
	FMOD_DEBUG_CALLBACK;
	//HACK:デバッグ出力をLogに切り替える
	FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR, FMOD_DEBUG_MODE_CALLBACK, FMODDebugCallback);
	FMOD::Studio::System* studio_system_temp;
	auto result = FMOD::Studio::System::create(&studio_system_temp);
	studio_system_ = std::shared_ptr<FMOD::Studio::System>(studio_system_temp, MockDeleter<FMOD::Studio::System>());
	if (result != FMOD_OK) {
		std::string errormessage("Failed to create FMOD system:");
		errormessage += FMOD_ErrorString(result);
		Log::OutputCritical(errormessage);
		return false;
	}
	//512:最大同時再生数
	result = studio_system_->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK) {
		std::string errormessage("Failed to initialize FMOD system:");
		errormessage += FMOD_ErrorString(result);
		Log::OutputCritical(errormessage);
		return false;
	}
	FMOD::System* low_level_system_temp;
	studio_system_->getCoreSystem(&low_level_system_temp);
	low_level_system_ = std::shared_ptr<FMOD::System>(low_level_system_temp, MockDeleter<FMOD::System>());
	std::default_delete<int> d;
	return true;
}

void AudioMaster::Shutdown()
{
	UnloadAllBanks();
	if (studio_system_) {
		studio_system_->release();
	}
	if (low_level_system_) {
		low_level_system_->release();
	}
}

void AudioMaster::LoadBank(const std::string& name, bool encrypt)
{
	{
		boost::unique_lock<boost::mutex> lock(banks_mutex_);
		if (banks_.find(name) != banks_.end()) {
			return;
		}
	}
	FMOD::Studio::Bank* bank_temp = nullptr;
	FMOD_RESULT result;
	if (encrypt) {
		auto filebuf = CryptoMaster::Decrypt(name);
		//メモリから読み込み
		result = studio_system_->loadBankMemory((const char*)filebuf.first.get(), filebuf.second,
			FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank_temp);
	}
	else {
		//ファイルから直に読み込み
		result = studio_system_->loadBankFile(name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank_temp);
	}
	if (result != FMOD_OK) {
		std::string errormessage("Failed to load bank:");
		errormessage += name;
		Log::OutputCritical(errormessage);
		return;
	}
	//non-streamingのサンプルデータを読み込む
	bank_temp->loadSampleData();
	//このbankに含まれるイベントの数
	int events_num = 0;
	bank_temp->getEventCount(&events_num);
	std::unordered_map<std::string, std::shared_ptr<FMOD::Studio::EventDescription>> events_map;
	if (events_num > 0) {
		//このbankのevent descriptionsのリストを得る
		std::vector<FMOD::Studio::EventDescription*> events(events_num);
		bank_temp->getEventList(events.data(), events_num, &events_num);
		char event_name[512];
		for (int n = 0; n < events_num; n++) {
			FMOD::Studio::EventDescription* e = events[n];
			//このイベントのパスを得る(event:/Explosion2Dなど)
			e->getPath(event_name, 512, nullptr);
			events_map.emplace(event_name + 7,
				std::shared_ptr<FMOD::Studio::EventDescription>(e,MockDeleter<FMOD::Studio::EventDescription>()));
		}
	}
	//このbankに含まれるbusの数
	int buses_num = 0;
	bank_temp->getBusCount(&buses_num);
	std::unordered_map<std::string, std::shared_ptr<FMOD::Studio::Bus>> buses_map;
	if (buses_num > 0) {
		//このbankに含まれるbusのリストを得る
		std::vector<FMOD::Studio::Bus*> buses(buses_num);
		bank_temp->getBusList(buses.data(), buses_num, &buses_num);
		char bus_name[512];
		for (int n = 0; n < buses_num; n++) {
			FMOD::Studio::Bus* bus = buses[n];
			//このbusのパスを得る(bus:/SFXなど)
			bus->getPath(bus_name, 512, nullptr);
			buses_map.emplace(bus_name + 5,
				std::shared_ptr<FMOD::Studio::Bus>(bus,MockDeleter<FMOD::Studio::Bus>()));
		}
	}
	if(events_map.size() > 0) {
		boost::unique_lock<boost::mutex> lock(events_map_mutex_);
		events_map_.merge(events_map);
	}
	if(buses_map.size() > 0) {
		boost::unique_lock<boost::mutex> lock(buses_map_mutex_);
		buses_map_.merge(buses_map);
	}
	{
		boost::unique_lock<boost::mutex> lock(banks_mutex_);
		banks_.emplace(name,
			std::shared_ptr<FMOD::Studio::Bank>(bank_temp,MockDeleter<FMOD::Studio::Bank>()));
	}
}

void AudioMaster::UnloadBank(const std::string& name)
{
	std::shared_ptr<FMOD::Studio::Bank> bank;
	{
		boost::unique_lock<boost::mutex> lock(banks_mutex_);
		auto itr = banks_.find(name);
		if (itr == banks_.end()) {
			return;
		}
		bank = itr->second;
		banks_.erase(itr);
	}
	int events_num = 0;
	bank->getEventCount(&events_num);
	if (events_num > 0) {
		std::vector<FMOD::Studio::EventDescription*> events(events_num);
		bank->getEventList(events.data(), events_num, &events_num);
		char event_name[512];
		for (auto e : events) {
			e->getPath(event_name, 512, nullptr);
			{
				boost::unique_lock<boost::mutex> lock(events_map_mutex_);
				auto eventi = events_map_.find(event_name + 7);
				if (eventi != events_map_.end()) {
					events_map_.erase(eventi);
				}
			}
		}
	}
	int buses_num = 0;
	bank->getBusCount(&buses_num);
	if (buses_num > 0) {
		std::vector<FMOD::Studio::Bus*> buses(buses_num);
		bank->getBusList(buses.data(), buses_num, &buses_num);
		char bus_name[512];
		for (auto bus : buses) {
			bus->getPath(bus_name, 512, nullptr);
			{
				auto busi = buses_map_.find(bus_name + 5);
				if (busi != buses_map_.end()) {
					buses_map_.erase(busi);
				}
			}
		}
	}
	bank->unloadSampleData();
	bank->unload();
}

void AudioMaster::UnloadAllBanks()
{
	{
		boost::unique_lock<boost::mutex> lock(banks_mutex_);
		for (auto bank : banks_) {
			bank.second->unloadSampleData();
			bank.second->unload();
		}
	}
	{
		boost::unique_lock<boost::mutex> lock(banks_mutex_);
		banks_.clear();
	}
	{
		boost::unique_lock<boost::mutex> lock(events_map_mutex_);
		events_map_.clear();
	}
}

SoundEvent AudioMaster::PlayEvent(const std::string& name)
{
	std::shared_ptr<FMOD::Studio::EventDescription> ed_p;
	{
		boost::unique_lock<boost::mutex> lock(events_map_mutex_);
		auto itr = events_map_.find(name);
		if (itr != events_map_.end()) {
			ed_p = itr->second;
		}
	}
	if (ed_p) {
		FMOD::Studio::EventInstance* inst = nullptr;
		ed_p->createInstance(&inst);
		if (inst) {
			inst->start();
			std::shared_ptr<FMOD::Studio::EventInstance> sh(inst,MockDeleter<FMOD::Studio::EventInstance>());
			{
				boost::unique_lock<boost::mutex> lock(insts_set_mutex_);
				insts_set_.insert(sh);
			}
			return SoundEvent(sh);
		}
	}
	return SoundEvent();
}

void AudioMaster::Update()
{
	boost::unique_lock<boost::mutex> lock(insts_set_mutex_);
	auto itr = insts_set_.begin();
	while (itr != insts_set_.end()) {
		FMOD_STUDIO_PLAYBACK_STATE state;
		(*itr)->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED) {
			(*itr)->release();
			itr = insts_set_.erase(itr);
		}
		else {
			itr++;
		}
	}
	studio_system_->update();
}

float AudioMaster::GetBusVolume(const std::string& name) const
{
	std::shared_ptr<FMOD::Studio::Bus> sh;
	{
		//HACK:ここでコンパイルが死ぬので外してるが、何かの方法でmutexは付けたい
		//boost::unique_lock<boost::mutex> lock(buses_map_mutex_);
		const auto itr = buses_map_.find(name);
		if (itr != buses_map_.end()) {
			sh = itr->second;
		}
	}
	float ret = 0.0f;
	if (sh) {
		sh->getVolume(&ret);
	}
	return ret;
}

bool AudioMaster::GetBusPaused(const std::string& name) const
{
	std::shared_ptr<FMOD::Studio::Bus> sh;
	{
		//boost::unique_lock<const boost::mutex> lock(buses_map_mutex_);
		const auto itr = buses_map_.find(name);
		if (itr != buses_map_.end()) {
			sh = itr->second;
		}
	}
	bool ret = false;
	if (sh) {
		sh->getPaused(&ret);
	}
	return ret;
}

void AudioMaster::SetBusVolume(const std::string& name, float volume)
{
	std::shared_ptr<FMOD::Studio::Bus> sh;
	{
		boost::unique_lock<boost::mutex> lock(buses_map_mutex_);
		const auto itr = buses_map_.find(name);
		if (itr != buses_map_.end()) {
			sh = itr->second;
		}
	}
	if (sh) {
		sh->setVolume(volume);
	}
}

void AudioMaster::SetBusPaused(const std::string& name, bool pause)
{
	std::shared_ptr<FMOD::Studio::Bus> sh;
	{
		boost::unique_lock<boost::mutex> lock(buses_map_mutex_);
		const auto itr = buses_map_.find(name);
		if (itr != buses_map_.end()) {
			sh = itr->second;
		}
	}
	if (sh) {
		sh->setPaused(pause);
	}
}
