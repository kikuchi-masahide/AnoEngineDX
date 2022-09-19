//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "SoundEvent.h"

#include "fmod_studio.hpp"
#include "AudioMaster.h"

SoundEvent::SoundEvent()
{
}

SoundEvent::SoundEvent(std::shared_ptr<FMOD::Studio::EventInstance> inst)
	:event_inst_(inst)
{
}

bool SoundEvent::IsValid() const
{
	return !event_inst_.expired();
}

void SoundEvent::Restart()
{
	if (IsValid()) {
		auto e = event_inst_.lock();
		e->start();
	}
}

void SoundEvent::Stop(bool allow_fade_out)
{
	if (IsValid()) {
		auto e = event_inst_.lock();
		FMOD_STUDIO_STOP_MODE mode = allow_fade_out ?
			FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
		e->stop(mode);
	}
}

void SoundEvent::SetPaused(bool pause)
{
	if (IsValid()) {
		auto e = event_inst_.lock();
		e->setPaused(pause);
	}
}

void SoundEvent::SetVolume(float value)
{
	if (IsValid()) {
		auto e = event_inst_.lock();
		e->setVolume(value);
	}
}

void SoundEvent::SetPitch(float value)
{
	if (IsValid()) {
		auto e = event_inst_.lock();
		e->setPitch(value);
	}
}

void SoundEvent::SetParameter(const std::string& name, float value)
{
	if (IsValid()) {
		auto e = event_inst_.lock();
		e->setParameterByName(name.c_str(), value);
	}
}

bool SoundEvent::GetPaused() const
{
	bool ret = false;
	if (IsValid()) {
		auto e = event_inst_.lock();
		e->getPaused(&ret);
	}
	return ret;
}

float SoundEvent::GetVolume() const
{
	float ret = 0.0f;
	if (IsValid()) {
		auto e = event_inst_.lock();
		e->getVolume(&ret);
	}
	return ret;
}

float SoundEvent::GetPitch() const
{
	float ret = 0.f;
	if (IsValid()) {
		auto e = event_inst_.lock();
		e->getPitch(&ret);
	}
	return ret;
}

float SoundEvent::GetParameter(const std::string& name)
{
	float ret = 0.0f;
	if (IsValid()) {
		auto e = event_inst_.lock();
		e->getParameterByName(name.c_str(), &ret);
	}
	return ret;
}
