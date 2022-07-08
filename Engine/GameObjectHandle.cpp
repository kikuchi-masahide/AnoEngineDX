//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "GameObjectHandle.h"
#include "GameObject.h"

GameObjectHandle::GameObjectHandle(const GameObjectHandle& handle)
	:object_(handle.object_), handles_set_(handle.handles_set_)
{
	if (object_ != nullptr) {
		handles_set_->insert(this);
	}
}

GameObjectHandle::GameObjectHandle(GameObject* obj, std::unordered_set<GameObjectHandle*>* set)
	:object_(obj), handles_set_(set)
{
	handles_set_->insert(this);
}

GameObjectHandle::GameObjectHandle()
	:object_(nullptr), handles_set_(nullptr)
{}

GameObjectHandle::~GameObjectHandle() {
	Reset();
}

GameObject* GameObjectHandle::operator->() const noexcept {
	//ダングリングポインタならばassertを出す
	BOOST_ASSERT_MSG(object_ != nullptr, "GameObjectHandle dungling pointer");
	return object_;
}

bool GameObjectHandle::IsValid() const
{
	return (object_ != nullptr);
}

void GameObjectHandle::Reset()
{
	if (object_ != nullptr) {
		handles_set_->erase(static_cast<GameObjectHandle*>(this));
		handles_set_ = nullptr;
		object_ = nullptr;
	}
}

GameObjectHandle& GameObjectHandle::operator=(const GameObjectHandle& handle)
{
	Reset();
	if (handle.object_ != nullptr) {
		object_ = handle.object_;
		handles_set_ = handle.handles_set_;
		handles_set_->insert(this);
	}
	return *this;
}