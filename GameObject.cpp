#include "GameObject.h"
#include "Component.h"
#include "Layer.h"
#include "Scene.h"
#include "Game.h"

GameObject::GameObject(boost::weak_ptr<Layer>& _layer, Vector2 _pos, double _scale, double _angle, int _prio = 0)
	:mLayer(_layer), mPosition(_pos), mScale(_scale), mRotation(_angle),
	mUpdPriority(_prio), mDeleteFlag(false), mThis(boost::weak_ptr<GameObject>()) {}

void GameObject::Update() {
	UniqueUpdate();
	CommonUpdate();
}

void GameObject::CommonUpdate() {
	UpdateUpdComponents();
	DeleteUpdComponents();
}

void GameObject::UniqueUpdate() {}

void GameObject::Output()
{
	CommonOutput();
	UniqueOutput();
}

void GameObject::CommonOutput() {
	UpdateOutputComponents();
	DeleteOutputComponents();
}

void GameObject::UniqueOutput() {}

void GameObject::AddComponent(boost::shared_ptr<Component> _comp)
{
	auto order = _comp->GetUpdPriority();
	//—Dæ“x~‡‚É‘}“ü‚·‚é
	auto itr = mUpdateComponents.begin();
	for (; itr != mUpdateComponents.end(); itr++)
	{
		if ((*itr)->GetUpdPriority() < order)
		{
			break;
		}
	}
	mUpdateComponents.insert(itr, _comp);
}

void GameObject::AddOutputComponent(boost::shared_ptr<Component> _comp)
{
	auto order = _comp->GetUpdPriority();
	//—Dæ“x~‡‚É‘}“ü‚·‚é
	auto itr = mOutputComponents.begin();
	for (; itr != mOutputComponents.end(); itr++)
	{
		if ((*itr)->GetUpdPriority() < order)
		{
			break;
		}
	}
	mOutputComponents.insert(itr, _comp);
}

Vector2 GameObject::GetPosition() const
{
	return mPosition;
}

Vector2 GameObject::SetPosition(Vector2 _pos)
{
	mPosition = _pos;
	return mPosition;
}

double GameObject::GetScale() const
{
	return mScale;
}

double GameObject::SetScale(double _sc)
{
	if (_sc < 0)mScale = 0.0f;
	else mScale = _sc;
	return mScale;
}

double GameObject::GetRotation() const
{
	return mRotation;
}

double GameObject::SetRotation(double _ro)
{
	mRotation = _ro;
	return mRotation;
}

GameObject::~GameObject() {}

boost::weak_ptr<Scene> GameObject::GetScene() const
{
	boost::shared_ptr<Layer> layer(mLayer);
	return layer->mScene;
}

boost::weak_ptr<Game> GameObject::GetGame() const
{
	boost::shared_ptr<Scene> scene(GetScene());
	return scene->mGame;
}

boost::weak_ptr<GameObject> GameObject::GetWeakThis() const
{
	return mThis;
}

void GameObject::SetWeakThis(boost::shared_ptr<GameObject>& _this)
{
	if (mThis.expired() != 0)
	{
		Log::OutputCritical("Resetting of weak this pointer");
		assert(0);
	}
	mThis = boost::weak_ptr<GameObject>(_this);
}

void GameObject::UpdateUpdComponents()
{
	for (int n = 0; n < mUpdateComponents.size(); n++) {
		mUpdateComponents[n]->Update();
	}
}

void GameObject::UpdateOutputComponents()
{
	for (int n = 0; n < mOutputComponents.size(); n++) {
		mOutputComponents[n]->Update();
	}
}

void GameObject::DeleteUpdComponents()
{
	for (int n = 0; n < mUpdateComponents.size(); n++) {
		if (mUpdateComponents[n]->GetDeleteFlag()) {
			mUpdateComponents.erase(mUpdateComponents.begin() + n);
			n--;
		}
	}
}

void GameObject::DeleteOutputComponents()
{
	for (int n = 0; n < mOutputComponents.size(); n++) {
		if (mOutputComponents[n]->GetDeleteFlag()) {
			mOutputComponents.erase(mOutputComponents.begin() + n);
			n--;
		}
	}
}

int GameObject::GetUpdPriority() const
{
	return mUpdPriority;
}
