#include "Component.h"
#include "GameObject.h"
#include "Layer.h"
#include "Scene.h"
#include "Game.h"

Component::Component(boost::weak_ptr<GameObject> _owner, int _order)
	: mOwner(_owner), mUpdPriority(_order), mDeleteFlag(false),
	mThis(boost::weak_ptr<Component>()) {}

Component::~Component() {}

void Component::Update() {}

boost::weak_ptr<Layer> Component::GetLayer() const
{
	boost::shared_ptr<GameObject> obj(mOwner);
	return obj->mLayer;
}

boost::weak_ptr<Scene> Component::GetScene() const
{
	boost::shared_ptr<Layer> layer(GetLayer());
	return layer->mScene;
}

boost::weak_ptr<Game> Component::GetGame() const
{
	boost::shared_ptr<Scene> scene(GetScene());
	return scene->mGame;
}

void Component::SetWeakThis(boost::shared_ptr<Component> _this)
{
	if (!mThis.empty())
	{
		Log::OutputCritical("Resetting of weak this pointer");
		assert(0);
	}
	mThis = boost::weak_ptr<Component>(_this);
}

boost::weak_ptr<Component> Component::GetWeakThis() const
{
	return mThis;
}
