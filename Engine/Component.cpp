//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Component.h"
#include "GameObject.h"
#include "Scene.h"
#include "Game.h"
#include "ComponentHandle.h"

Component::Component(Scene* scene, GameObjectHandle handle, int order)
	: upd_priority_(order), obj_(handle), delete_flag_(false), scene_(scene)
{
}

void Component::Initialize()
{
}

Component::~Component() {
}

void Component::Update() {}

bool Component::GetDeleteFlag() const
{
	return delete_flag_;
}

void Component::SetDeleteFlag()
{
	delete_flag_ = true;
}

void Component::SetSelfSharedptr(std::shared_ptr<Component> comp)
{
	self_ = comp;
}
