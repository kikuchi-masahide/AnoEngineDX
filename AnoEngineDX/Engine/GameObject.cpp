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
	std::erase_if(comps_, [](const std::weak_ptr<Component>& comp) {
		return comp.expired();
	});
}

void GameObject::SetAllCompsFlag()
{
	std::for_each(comps_.begin(), comps_.end(), [](std::weak_ptr<Component>& comp) {
		if (!comp.expired()) {
			comp.lock()->SetDeleteFlag();
		}
	});
}

void GameObject::SetDeleteFlag()
{
	if (!delete_flag_) {
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
	assert(this_wk_.expired());
	this_wk_ = obj;
}
