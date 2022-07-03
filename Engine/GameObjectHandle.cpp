#include "GameObjectHandle.h"
#include "GameObject.h"

GameObjectHandle::GameObjectHandle(const GameObjectHandle& _handle)
	:mObject(_handle.mObject), mHandleSet(_handle.mHandleSet)
{
	if (mObject != nullptr) {
		mHandleSet->insert(this);
	}
}

GameObjectHandle::GameObjectHandle(GameObject* _obj, std::unordered_set<GameObjectHandle*>* _set)
	:mObject(_obj), mHandleSet(_set)
{
	mHandleSet->insert(this);
}

GameObjectHandle::GameObjectHandle()
	:mObject(nullptr), mHandleSet(nullptr)
{}

GameObjectHandle::~GameObjectHandle() {
	Reset();
}

GameObject* GameObjectHandle::operator->() const noexcept {
	//ダングリングポインタならばassertを出す
	BOOST_ASSERT_MSG(mObject != nullptr, "GameObjectHandle dungling pointer");
	return mObject;
}

bool GameObjectHandle::IsValid() const
{
	return (mObject != nullptr);
}

void GameObjectHandle::Reset()
{
	if (mObject != nullptr)
	{
		mHandleSet->erase(static_cast<GameObjectHandle*>(this));
		mHandleSet = nullptr;
		mObject = nullptr;
	}
}

GameObjectHandle& GameObjectHandle::operator=(const GameObjectHandle& handle)
{
	Reset();
	if (handle.mObject != nullptr)
	{
		mObject = handle.mObject;
		mHandleSet = handle.mHandleSet;
		mHandleSet->insert(this);
	}
	return *this;
}