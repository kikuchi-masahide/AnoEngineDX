//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace FMOD {
	namespace Studio {
		class EventInstance;
	}
};

/// <summary>
/// FMOD::Studio::EventDescriptionを実体化したのEventInstanceの、ハンドルとして機能する
/// このオブジェクトが残っていても、FMOD_STUDIO_PLAYBACK_STOPPEDとなりSystemでEventInstanceがreleaseされると失効することに注意
/// </summary>
class SoundEvent {
public:
	SoundEvent();
	//AudioMaster::InstantiateEventから呼び出される
	SoundEvent(std::shared_ptr<FMOD::Studio::EventInstance> inst);
	/// <summary>
	/// 関連するFMOD eventがまだ存在しているか否か
	/// </summary>
	/// <returns></returns>
	bool IsValid() const;
	/// <summary>
	/// eventをはじめから再生する
	/// </summary>
	void Restart();
	/// <summary>
	/// このeventを停止する
	/// </summary>
	/// <param name="allow_fade_out"></param>
	void Stop(bool allow_fade_out = true);
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);
	//有効でない場合はfalseを返すことに注意
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);
private:
	std::weak_ptr<FMOD::Studio::EventInstance> event_inst_;
};