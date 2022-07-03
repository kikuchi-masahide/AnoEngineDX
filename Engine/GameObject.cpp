#include "GameObject.h"
#include "GameObjectHandle.h"
#include "Component.h"
#include "Scene.h"
#include "Game.h"

GameObject::GameObject(Scene* _scene, MatVec::Vector2 _pos, double _scale, double _angle)
	:mScene(_scene), mPosition(_pos), mScale(_scale), mRotation(_angle),mDeleteFlag(false)
{
}

MatVec::Vector2 GameObject::GetPosition() const
{
	return mPosition;
}

MatVec::Vector2 GameObject::SetPosition(const MatVec::Vector2& _pos)
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

Game& GameObject::GetGame()
{
	return mScene->mGame;
}

GameObject::~GameObject() {
	//自分を消す前にコンポーネントたちを消しておく
	for (auto itr = mUpdateComponents.begin(); itr != mUpdateComponents.end(); itr++)
	{
		DeleteComponent(*itr);
	}
	for (auto itr = mOutputComponents.begin(); itr != mOutputComponents.end(); itr++)
	{
		DeleteComponent(*itr);
	}
	//自分を指すGameObjectHandleをReset
	while (!mHandles.empty())
	{
		auto itr = mHandles.begin();
		(*itr)->Reset();
	}
}

void GameObject::DeleteFlagedComponents()
{
	//コンポーネントを巡回しフラグが立っているものを削除
	auto itr = mUpdateComponents.begin();
	while (itr != mUpdateComponents.end()) {
		if ((*itr)->GetDeleteFlag()) {
			DeleteComponent(*itr);
			itr = mUpdateComponents.erase(itr);
		}
		else itr++;
	}
	itr = mOutputComponents.begin();
	while (itr != mOutputComponents.end()) {
		if ((*itr)->GetDeleteFlag()) {
			DeleteComponent(*itr);
			itr = mOutputComponents.erase(itr);
		}
		else itr++;
	}
}

void GameObject::AddUpdateComponentToScene(ComponentHandle<Component> _handle)
{
	mScene->AddUpdateComponent(this, _handle);
}

void GameObject::AddOutputComponentToScene(ComponentHandle<Component> _handle)
{
	mScene->AddOutputComponent(this, _handle);
}

GameObjectHandle GameObject::This()
{
	return GameObjectHandle(this, &mHandles);
}

void GameObject::DeleteComponent(Component* _component)
{
	_component->mDeleteCheck = true;
	delete _component;
}
