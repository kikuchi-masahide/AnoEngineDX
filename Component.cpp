#include "Component.h"
#include "GameObject.h"
#include "Layer.h"
#include "Scene.h"
#include "Game.h"
#include "ComponentHandle.h"

Component::~Component() {
	std::for_each(mHandles->begin(), mHandles->end(), [this](ComponentHandle* _obj) {
		_obj->Reset(this);
	});
}

Component::Component(GameObject* _owner, boost::shared_ptr<std::set<ComponentHandle*>> _hset, int _order)
	: mOwner(*_owner), mHandles(_hset), mUpdPriority(_order), mDeleteFlag(false)
{}

void Component::Update() {}

Scene& Component::GetScene() const
{
	return mOwner.mScene;
}

Game& Component::GetGame() const
{
	return mOwner.mScene.mGame;
}

