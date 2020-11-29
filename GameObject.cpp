#include "GameObject.h"
#include "Component.h"
#include "ComponentHandle.h"
#include "Layer.h"
#include "Scene.h"
#include "Game.h"

GameObject::GameObject(Scene* _scene, boost::shared_ptr<std::set<GameObjectHandle*>> _hset, Vector2 _pos, double _scale, double _angle)
	:mScene(*_scene), mHandles(_hset), mPosition(_pos), mScale(_scale), mRotation(_angle),mDeleteFlag(false)
{}

template<class T,class... Args>
ComponentHandle GameObject::AddUpdateComponent(Args... _args) {
	boost::shared_ptr<std::set<ComponentHandle*>> comphsetp(new std::set<ComponentHandle*>());
	boost::shared_ptr<Component> compp(new T(this, comphsetp, _args...));
	mUpdateComponents.push_back(compp);
}

template<class T, class... Args>
ComponentHandle GameObject::AddOutputComponent(Args... _args) {
	boost::shared_ptr<std::set<ComponentHandle*>> comphsetp(new std::set<ComponentHandle*>());
	boost::shared_ptr<Component> compp(new T(this, comphsetp, _args...));
	mOutputComponents.push_back(compp);
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