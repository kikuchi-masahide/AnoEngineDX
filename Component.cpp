#include "Component.h"
#include "GameObject.h"
#include "Scene.h"
#include "Game.h"
#include "ComponentHandle.h"

Component::~Component() {
	std::for_each(mHandles->begin(), mHandles->end(), [this](void* _obj) {
		//–³—‚â‚è
		((ComponentHandle<Component>*)_obj)->Reset(this);
	});
}

Component::Component(GameObject* _owner, boost::shared_ptr<std::set<void*>> _hset, int _order)
	: mOwner(*_owner), mHandles(_hset), mUpdPriority(_order), mDeleteFlag(false)
{
	//_owner‚ªnullptr‚É‚È‚Á‚Ä‚éê‡‚Æ‚©‚ğ‚Í‚¶‚­
	BOOST_ASSERT_MSG(_owner != nullptr,"Component::Component() should be called in GameObject::AddOutput/UpdateComponent()");
	BOOST_ASSERT_MSG(_hset != nullptr, "Component::Component() should be called in GameObject::AddOutput/UpdateComponent()");
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

