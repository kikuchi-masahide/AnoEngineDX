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
	: upd_priority_(order), obj_(handle), delete_flag_(false), delete_check_(false)
{
	handles_.reserve(10);
}

Component::~Component() {
	BOOST_ASSERT_MSG(delete_check_ == true, "irregal destructor call without GameObject permission");
	while (!handles_.empty()) {
		auto itr = handles_.begin();
		auto handle = static_cast<ComponentHandle<Component>*>(*itr);
		handle->Reset();
	}
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