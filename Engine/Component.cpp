//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Component.h"
#include "GameObject.h"
#include "Scene.h"
#include "Game.h"

Component::Component(std::weak_ptr<GameObject> obj, int order)
	: upd_priority_(order), obj_(obj), delete_flag_(false)
{
}

void Component::AsyncInitialize()
{
}

void Component::CleanUp()
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
		auto scene = obj_.lock()->scene_;
		scene->Erase(this_wk_);
	}
}

void Component::SetSharedPtr(std::shared_ptr<Component> comp)
{
	assert(this_wk_.expired());
	this_wk_ = comp;
}
