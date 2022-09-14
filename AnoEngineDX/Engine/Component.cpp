//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Component.h"
#include "GameObject.h"
#include "Scene.h"
#include "Game.h"
#include "ComponentHandle.h"

Component::Component(GameObjectHandle handle, int order)
	: upd_priority_(order), obj_(handle), delete_flag_(false)
{
}

void Component::AsyncInitialize()
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
	if (!delete_flag_) {
		delete_flag_ = true;
		auto scene = obj_->scene_;
		scene->Erase(this_sh_);
	}
}

void Component::SetSharedPtr(std::shared_ptr<Component> comp)
{
	assert(!this_sh_);
	this_sh_ = comp;
}

void Component::ResetSharedPtr(std::shared_ptr<Component> comp)
{
	assert(this_sh_ == comp);
	this_sh_.reset();
}
