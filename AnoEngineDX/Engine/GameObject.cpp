//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "GameObject.h"

#include "Component.h"
#include "Scene.h"

GameObject::GameObject(Scene* scene)
	:scene_(scene),delete_flag_(false)
{
}

GameObject::~GameObject()
{
}

void GameObject::UnregisterInvalidChilds()
{
	std::erase_if(comps_, [](const ComponentHandle<Component>& comp) {
		return !comp;
	});
}

void GameObject::SetAllCompsFlag()
{
	std::for_each(comps_.begin(), comps_.end(), [](ComponentHandle<Component>& comp) {
		comp->SetDeleteFlag();
	});
}

void GameObject::SetDeleteFlag()
{
	if (!delete_flag_) {
		scene_->Erase(this_sh_);
		SetAllCompsFlag();
		delete_flag_ = true;
	}
}

bool GameObject::GetDeleteFlag() const
{
	return delete_flag_;
}

void GameObject::SetSharedPtr(std::shared_ptr<GameObject> obj)
{
	assert(!this_sh_);
	this_sh_ = obj;
}

void GameObject::ResetSharedPtr(std::shared_ptr<GameObject> obj)
{
	assert(obj == this_sh_);
	this_sh_.reset();
}
