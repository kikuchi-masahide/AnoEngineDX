//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "GameObjectHandle.h"

#include "Log.h"

GameObjectHandle::GameObjectHandle()
	:obj_()
{
}

GameObjectHandle::GameObjectHandle(std::shared_ptr<GameObject> obj)
	:obj_(obj)
{
}

std::shared_ptr<GameObject> GameObjectHandle::operator->() const noexcept
{
#ifdef _DEBUG
	if (obj_.expired()) {
		Log::OutputCritical("expired GameObjectHandle referred");
	}
#endif
	return obj_.lock();
}

GameObjectHandle::operator bool() const noexcept
{
	return !(obj_.expired());
}

bool GameObjectHandle::operator!() const noexcept
{
	return !static_cast<bool>(*this);
}

void GameObjectHandle::Reset() noexcept
{
	obj_.reset();
}

bool operator<(const GameObjectHandle& a, const GameObjectHandle& b)
{
	return a.obj_.owner_before(b.obj_);
}

bool operator>(const GameObjectHandle& a, const GameObjectHandle& b)
{
	return b < a;
}

bool operator!=(const GameObjectHandle& a, const GameObjectHandle& b)
{
	return (a < b || b < a);
}

bool operator==(const GameObjectHandle& a, const GameObjectHandle& b)
{
	return a != b;
}
