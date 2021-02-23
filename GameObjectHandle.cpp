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
	BOOST_ASSERT_MSG(mObject != nullptr, "GameObjectHandle dungling pointer");
	return mObject;
}

bool GameObjectHandle::IsValid() const
{
	return (mObject != nullptr);
}

void GameObjectHandle::Reset(GameObject* _obj)
{
	BOOST_ASSERT_MSG(mObject == _obj, "GameObjectHandle::Reset() should be called in GameObject::~GameObject()");
	mObject = nullptr;
	mHandleSet.reset();
}

GameObjectHandle& GameObjectHandle::operator=(const GameObjectHandle& handle)
{
	mHandleSet->erase(this);
	mObject = handle.mObject;
	mHandleSet = handle.mHandleSet;
	mHandleSet->insert(this);
	return *this;
}