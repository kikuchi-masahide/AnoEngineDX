#include "Component.h"
#include "GameObject.h"
#include "Scene.h"
#include "Game.h"
#include "ComponentHandle.h"

Component::~Component() {
	std::for_each(mHandles->begin(), mHandles->end(), [this](void* _obj) {
		//無理やり
		((ComponentHandle<Component>*)_obj)->Reset(this);
	});
}

Component::Component(GameObject* _owner, boost::shared_ptr<std::set<void*>> _hset, int _order)
	: mOwner(*_owner), mHandles(_hset), mUpdPriority(_order), mDeleteFlag(false)
{
	//_ownerがnullptrになってる場合とかをはじく
	assert(_owner != nullptr);
	assert(_hset != nullptr);
}

void Component::Update() {}

Scene& Component::GetScene() const
{
	return mOwner.GetScene();
}

Game& Component::GetGame() const
{
	return mOwner.GetScene().mGame;
}

