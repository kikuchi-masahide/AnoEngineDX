#include "GameObjectHandle.h"
#include "GameObject.h"

GameObjectHandle::GameObjectHandle(const GameObjectHandle& _handle)
	:mObject(_handle.mObject), mHandleSet(_handle.mHandleSet)
{
	mHandleSet->insert(this);
}

GameObjectHandle::GameObjectHandle(GameObject* _obj, boost::shared_ptr<std::set<GameObjectHandle*>> _set)
	:mObject(_obj), mHandleSet(_set)
{
	mHandleSet->insert(this);
}

GameObjectHandle::GameObjectHandle()
	:mObject(nullptr), mHandleSet(nullptr)
{}

GameObjectHandle::~GameObjectHandle() {
	if (mHandleSet != nullptr)mHandleSet->erase(this);
}

GameObject* GameObjectHandle::operator->() const noexcept {
	//ダングリングポインタならばassertを出す
	if (mObject == nullptr) {
		Log::OutputCritical("Dungling Pointer");
	}
	return mObject;
}

bool GameObjectHandle::IsValid()
{
	return (mObject != nullptr);
}

void GameObjectHandle::Reset(GameObject* _obj)
{
	assert(mObject == _obj);
	mObject = nullptr;
	mHandleSet.reset();
}