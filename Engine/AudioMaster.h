//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "SoundEvent.h"
class Game;

namespace FMOD {
	class System;
	namespace Studio {
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	}
}

class AudioMaster final:public boost::noncopyable {
public:
	AudioMaster();
	~AudioMaster();
	bool Initialize();
	void Shutdown();
	/// <summary>
	/// 暗号化されたバンク(イベント、サンプルデータ、ストリーミングデータが入るコンテナ)を読み込む
	/// ～～..strings.bank -> ～～.bankの順で読み込む
	/// (同期処理、thread-safe)
	/// </summary>
	/// <param name="name">ファイル名</param>
	/// <param name="encrypt">暗号化されているか否か</param>
	void LoadBank(const std::string& name, bool encrypt);
	//同期処理、thread-safe
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();
	/// <summary>
	/// EventDescriptionを実体化し、eventを作成する
	/// </summary>
	/// <param name="name">event:/Explosion2DならばExplosion2Dなど</param>
	SoundEvent PlayEvent(const std::string& name);
	void Update();
	/// <param name="name">先頭の「bus:/」は省略する</param>
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	/// <param name="name">先頭の「bus:/」は省略する</param>
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
private:
	//FMODで作成したオブジェクトをstd::shared_ptrで包むときは、
	//メモリをdeleteしないよう以下をdeleterに指定する
	template<class T>
	struct MockDeleter {
	public:
		void operator()(T* ptr) {};
	};
	std::unordered_map<std::string, std::shared_ptr<FMOD::Studio::Bank>> banks_;
	boost::mutex banks_mutex_;
	std::shared_ptr<FMOD::Studio::System> studio_system_;
	std::shared_ptr<FMOD::System> low_level_system_;
	std::unordered_map<std::string, std::shared_ptr<FMOD::Studio::EventDescription>> events_map_;
	boost::mutex events_map_mutex_;
	std::unordered_map<std::string, std::shared_ptr<FMOD::Studio::Bus>> buses_map_;
	boost::mutex buses_map_mutex_;
	std::unordered_set<std::shared_ptr<FMOD::Studio::EventInstance>> insts_set_;
	boost::mutex insts_set_mutex_;
};